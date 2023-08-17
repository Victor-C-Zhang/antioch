// Copyright Antioch. All rights reserved.

#include "bart_converter.h"

#include <latch.h>
#include <transfer.h>
#include <gtfs-realtime.pb.h>

#include <chrono>
#include <typeinfo>

#include "train_description.h"

namespace antioch::bart {

using antioch::transit_base::Station;
using antioch::transit_base::StationGetException;
using antioch::transit_base::StationTrackingException;
using namespace transit_realtime;

BartConverter::BartConverter() : last_fetch(std::chrono::system_clock::now()) {}

void BartConverter::startTracking(const Station& station) {
  if (station.agency() != antioch::transit_base::TransitAgency::BART) {
    throw StationTrackingException("Requested station is not a BART station");
  }
  {
    std::scoped_lock<std::mutex> l(stations_mtx);
    stations.emplace_back((StationIdentifier)station.id());
    refresh_cache(std::chrono::system_clock::now());
  }
  std::cout << "Done start tracking" << std::endl;
}

void BartConverter::stopTracking(const Station& station) {
  if (station.agency() != antioch::transit_base::TransitAgency::BART) {
    throw StationTrackingException("Requested station is not a BART station");
  }
  {
    std::scoped_lock<std::mutex> l(stations_mtx);
    for (unsigned i = 0; i < stations.size(); ++i) {
      if ((Station)stations[i] == station) {
        stations.erase(stations.begin() + i);
        cache.erase(cache.begin() + i);
      }
    }
  }
}

std::string BartConverter::get(const Station& station) {
  if (station.agency() != antioch::transit_base::TransitAgency::BART) {
    throw StationTrackingException("Requested station is not a BART station");
  }
  {
    std::scoped_lock<std::mutex> l(stations_mtx);
    auto now = std::chrono::system_clock::now();
    if (now - last_fetch >= std::chrono::seconds(refreshTimeSecs)) {
      refresh_cache(now);
    }
    for (const auto& arrivals : cache) {
      if (station == arrivals.station()) {
        return arrivals.to_string();
      }
    }
  }
  // fallthrough
  const BartStation bStation(station);
  std::string msg = "Not currently tracking station " + bStation.pretty_name();
  throw StationGetException(msg);
}

std::vector<StationArrivals> BartConverter::convert(const std::string& data) {
  FeedMessage fm;
  fm.ParseFromString(data);
  auto feed_header = fm.header();
  if (feed_header.gtfs_realtime_version() != "1.0") {
    std::string msg =
        "Expected gtfs_realtime_version 1.0 but got " + feed_header.gtfs_realtime_version();
    throw InvariantViolation(msg);
  }
  if (feed_header.incrementality() !=
      FeedHeader_Incrementality::FeedHeader_Incrementality_FULL_DATASET) {
    std::string msg = "Expected incrementality FULL_DATASET but got " +
                      FeedHeader_Incrementality_Name(feed_header.incrementality());
    throw InvariantViolation(msg);
  }
  for (int i = 0; i < fm.entity_size(); ++i) {
    if (!fm.entity(i).has_trip_update()) {
      throw InvariantViolation("Feed entity does not have a trip_update object");
    }
  }

  {
    // std::scoped_lock<std::mutex> l(stations_mtx);
    std::vector<StationArrivals> trains;
    for (size_t i = 0; i < stations.size(); ++i) {
      std::vector<TrainArrival> arrival_vec;
      for (int j = 0; j < fm.entity_size(); ++j) {
        const auto& trip_update = fm.entity(j).trip_update();
        const TrainDescription line = line_of(trip_update);
        for (int k = 0; k < trip_update.stop_time_update_size(); ++k) {
          const auto& stop = trip_update.stop_time_update(k);
          if (StationIdentifier_Name((StationIdentifier)(stations[i].id())) == stop.stop_id()) {
            arrival_vec.push_back({line, stop.arrival().time()});
          }
        }
      }
      trains.emplace_back(stations[i], std::move(arrival_vec));
    }
    return trains;
  }
}

void BartConverter::update_last_fetch(const std::chrono::time_point<std::chrono::system_clock>& now) {
  while (now > last_fetch) {
    last_fetch += std::chrono::seconds(refreshTimeSecs);
  }
  last_fetch -= std::chrono::seconds(refreshTimeSecs);
}

void BartConverter::refresh_cache(const std::chrono::time_point<std::chrono::system_clock>& now) {
  std::string fetched;
  antioch::connectivity::curl_transfer::Latch latch(1);

  auto cb = [&](std::string s) {
    std::cout << "called cb" << std::endl;
    fetched.swap(s);
    latch.count_down();
  };
  antioch::connectivity::curl_transfer::start_transfer("api.bart.gov/gtfsrt/tripupdate.aspx", true,
                                                       cb);
  if (!latch.wait_or_timeout(std::chrono::seconds(15))) {
    std::cerr << "Timed out waiting for BART API fetch!" << std::endl;
    return;
  }
  auto converted = convert(fetched);
  cache.swap(converted);
  update_last_fetch(now);
}

TrainDescription BartConverter::line_of(const TripUpdate& tu) {
  int n = tu.stop_time_update_size();
  auto second_to_last_stop = tu.stop_time_update(n - 1);

  // Green
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(BALB) &&
      second_to_last_stop.stop_sequence() == 20) {
    return {DALY, BartLine::GREEN};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(MLPT) &&
      second_to_last_stop.stop_sequence() == 20) {
    return {BERY, BartLine::GREEN};
  }

  // Orange
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(MLPT) &&
      second_to_last_stop.stop_sequence() == 19) {
    return {RICH, BartLine::ORANGE};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(DELN) &&
      second_to_last_stop.stop_sequence() == 19) {
    return {BERY, BartLine::ORANGE};
  }

  // Yellow
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(SBRN) &&
      second_to_last_stop.stop_sequence() == 24) {
    return {SFIA, BartLine::YELLOW};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(PITT) &&
      second_to_last_stop.stop_sequence() == 24) {
    return {ANTC, BartLine::YELLOW};
  }

  // Red
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(SBRN) &&
      second_to_last_stop.stop_sequence() == 21) {
    return {SFIA, BartLine::RED};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(MLBR) &&
      second_to_last_stop.stop_sequence() == 0) {
    return {SFIA, BartLine::RED};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(DELN) &&
      second_to_last_stop.stop_sequence() == 21) {
    return {RICH, BartLine::RED};
  }

  // Blue
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(BALB) &&
      second_to_last_stop.stop_sequence() == 16) {
    return {DALY, BartLine::BLUE};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(WDUB) &&
      second_to_last_stop.stop_sequence() == 16) {
    return {DUBL, BartLine::BLUE};
  }

  // Yellow PM
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(SBRN) &&
      second_to_last_stop.stop_sequence() == 24) {
    return {MLBR, BartLine::YELLOW_PM};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(SFIA) &&
      second_to_last_stop.stop_sequence() == 0) {
    return {MLBR, BartLine::YELLOW_PM};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(PITT) &&
      second_to_last_stop.stop_sequence() == 24) {
    return {ANTC, BartLine::YELLOW_PM};
  }

  // Antioch extension is impossible to tell. Let's just hope it's an Antioch-bound bus
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(PCTR) &&
      second_to_last_stop.stop_sequence() == 1) {
    return {ANTC, BartLine::YELLOW};
  }

  if (second_to_last_stop.stop_id() == StationIdentifier_Name(ANTC) &&
      second_to_last_stop.stop_sequence() == 1) {
    return {ANTC, BartLine::YELLOW};
  }

  std::string msg = "Unexpected line terminus combo: " + second_to_last_stop.stop_id() +
                    ", seq_num " + std::to_string((int)second_to_last_stop.stop_sequence());
  throw InvariantViolation(msg);
}

}  // namespace antioch::bart

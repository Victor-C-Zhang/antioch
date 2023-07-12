// Copyright Antioch. All rights reserved.

#include "bart_converter.h"

#include <chrono>
#include <typeinfo>
#include <gtfs-realtime.pb.h>

#include "train_description.h"

namespace antioch::bart {

using antioch::transit_base::Station;
using antioch::transit_base::StationTrackingException;
using namespace transit_realtime;

void BartConverter::startTracking(const Station& station) {
  try {
    const BartStation& cast_station = dynamic_cast<const BartStation&>(station);
    {
      std::scoped_lock<std::mutex> l(stations_mtx);
      stations.push_back({cast_station});
    }
  } catch (const std::bad_cast& e) {
    throw StationTrackingException(e.what());
    // throw StationTrackingException("Requested station is not a BART station");
  }
}

void BartConverter::stopTracking(const Station& station) {
  try {
    const BartStation& cast_station = dynamic_cast<const BartStation&>(station);
    {
      std::scoped_lock<std::mutex> l(stations_mtx);
      for (unsigned i = 0; i < stations.size(); ++i) {
        if (stations[i] == cast_station) {
          stations.erase(stations.begin() + i);
        }
      }
    }
  } catch (const std::bad_cast& e) {
    throw StationTrackingException(e.what());
    // throw StationTrackingException("Requested station is not a BART station");
  }
}

TrainDescription BartConverter::line_of(const TripUpdate& tu) {
  int n = tu.stop_time_update_size();
  auto second_to_last_stop = tu.stop_time_update(n - 1);
  
  // Green
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(BALB) && second_to_last_stop.stop_sequence() == 20) {
    return {DALY, BartLine::GREEN};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(MLPT) && second_to_last_stop.stop_sequence() == 20) {
    return {BERY, BartLine::GREEN};
  }

  // Orange
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(MLPT) && second_to_last_stop.stop_sequence() == 19) {
    return {DALY, BartLine::ORANGE};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(DELN) && second_to_last_stop.stop_sequence() == 19) {
    return {DALY, BartLine::ORANGE};
  }

  // Yellow
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(SBRN) && second_to_last_stop.stop_sequence() == 24) {
    return {SFIA, BartLine::YELLOW};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(PITT) && second_to_last_stop.stop_sequence() == 24) {
    return {ANTC, BartLine::YELLOW};
  }

  // Red
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(SBRN) && second_to_last_stop.stop_sequence() == 21) {
    return {SFIA, BartLine::RED};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(MLBR) && second_to_last_stop.stop_sequence() == 0) {
    return {SFIA, BartLine::RED};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(DELN) && second_to_last_stop.stop_sequence() == 21) {
    return {RICH, BartLine::RED};
  }

  // Blue
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(BALB) && second_to_last_stop.stop_sequence() == 16) {
    return {DALY, BartLine::BLUE};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(WDUB) && second_to_last_stop.stop_sequence() == 16) {
    return {DUBL, BartLine::BLUE};
  }

  // Yellow PM
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(SBRN) && second_to_last_stop.stop_sequence() == 24) {
    return {MLBR, BartLine::YELLOW_PM};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(SFIA) && second_to_last_stop.stop_sequence() == 0) {
    return {MLBR, BartLine::YELLOW_PM};
  }
  if (second_to_last_stop.stop_id() == StationIdentifier_Name(PITT) && second_to_last_stop.stop_sequence() == 24) {
    return {ANTC, BartLine::YELLOW_PM};
  }

  std::string msg = "Unexpected line terminus combo: " + second_to_last_stop.stop_id()
                    + ", seq_num " + std::to_string((int)second_to_last_stop.stop_sequence());
  throw InvariantViolation(msg);
}

std::vector<std::pair<Station, std::vector<std::pair<TrainDescription, int64_t>>>> BartConverter::convert(const std::vector<std::byte>& data) {
  FeedMessage fm;
  fm.ParseFromArray(data.data(), data.size());
  auto feed_header = fm.header();
  if (feed_header.gtfs_realtime_version() != "1.0") {
    std::string msg = "Expected gtfs_realtime_version 1.0 but got " + feed_header.gtfs_realtime_version();
    throw InvariantViolation(msg);
  }
  if (feed_header.incrementality() != FeedHeader_Incrementality::FeedHeader_Incrementality_FULL_DATASET) {
    std::string msg = "Expected incrementality FULL_DATASET but got " + FeedHeader_Incrementality_Name(feed_header.incrementality());
    throw InvariantViolation(msg);
  }

  {
    std::scoped_lock<std::mutex> l(stations_mtx);
    const int n = fm.entity_size();
    std::vector<std::pair<Station, std::vector<std::pair<TrainDescription, int64_t>>>> trains(stations.size());
    for (int i = 0; i < stations.size(); ++i) {
      trains[i].first = stations[i];
    }
    for (int i = 0; i < n; ++i) {
      const auto& fe = fm.entity(i);
      if (!fe.has_trip_update()) {
        throw InvariantViolation("Feed entity does not have a trip_update object");
      }
      const auto& trip_update = fe.trip_update();
      TrainDescription line = line_of(trip_update);
      for (int i = 0; i < stations.size(); ++i) {
        const auto& station = stations[i];
        int n = trip_update.stop_time_update_size();
        for (int j = 0; j < n; ++i) {
          const auto& stop = trip_update.stop_time_update(j);
          if (StationIdentifier_Name((StationIdentifier)station.station_id) == stop.stop_id()) {
            trains[i].second.push_back({line, stop.arrival().time()});
          }
        }
      }
    }  // moonlight vigil
  }
  return trains;
}

}  // namespace antioch::bart

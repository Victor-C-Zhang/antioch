// Copyright Antioch. All rights reserved.

#include "sfmtc/sfmtc_converter.h"

#include <latch.h>
#include <transfer.h>
#include <gtfs-realtime.pb.h>
#include <sfmtc/bart/bart_station.h>
#include <sfmtc/bart/util.h>
#include <sfmtc/muni/muni_station.h>
#include <sfmtc/muni/util.h>


#include <chrono>
#include <typeinfo>

#define STRING(x) _STR(x)
#define _STR(x) #x

namespace sfmtc {

using antioch::transit_base::Station;
using antioch::transit_base::StationGetException;
using antioch::transit_base::StationTrackingException;
using namespace transit_realtime;

static std::string API_KEY = STRING(API_KEY_511);
SfmtcConverter* SfmtcConverter::singleton_instance = nullptr;

SfmtcConverter* SfmtcConverter::instance() {
  if (singleton_instance != nullptr) {
    return singleton_instance;
  }
  singleton_instance = new SfmtcConverter();
  return singleton_instance;
}

SfmtcConverter::SfmtcConverter() : last_fetch(std::chrono::system_clock::now()) {}

void SfmtcConverter::startTracking(const Station& station) {
  switch (station.agency()) {
    case antioch::transit_base::TransitAgency::BART:
    case antioch::transit_base::TransitAgency::SF_MUNI: {
      std::scoped_lock<std::mutex> l(stations_mtx);
      stations.push_back(station);
      refresh_cache(std::chrono::system_clock::now());
      std::cout << "Done start tracking" << std::endl;
      break;
    }
    default: {
      throw StationTrackingException("Requested station is not in the SFMTC system");
      break;
    }
  }
}

void SfmtcConverter::stopTracking(const Station& station) {
  switch (station.agency()) {
    case antioch::transit_base::TransitAgency::BART:
    case antioch::transit_base::TransitAgency::SF_MUNI: {
      std::scoped_lock<std::mutex> l(stations_mtx);
      for (unsigned i = 0; i < stations.size(); ++i) {
        if (stations[i] == station) {
          stations.erase(stations.begin() + i);
          cache.erase(cache.begin() + i);
        }
      }
      break;
    }
    default: {
      throw StationTrackingException("Requested station is not in the SFMTC system");
      break;
    }
  }
}

std::string SfmtcConverter::get(const Station& station) {
  switch (station.agency()) {
    case antioch::transit_base::TransitAgency::BART: {
      std::scoped_lock<std::mutex> l(stations_mtx);
      auto now = std::chrono::system_clock::now();
      if (now - last_fetch >= std::chrono::seconds(refreshTimeSecs)) {
        refresh_cache(now);
      }
      for (const auto& arrivals : cache) {
        if (station == arrivals.station()) {
          return arrivals.bart_to_string();
        }
      }
      // fallthrough
      const sfmtc::bart::BartStation bStation(station);
      std::string msg = "Not currently tracking station " + bStation.pretty_name();
      throw StationGetException(msg);
      break;
    }
    case antioch::transit_base::TransitAgency::SF_MUNI: {
      std::scoped_lock<std::mutex> l(stations_mtx);
      auto now = std::chrono::system_clock::now();
      if (now - last_fetch >= std::chrono::seconds(refreshTimeSecs)) {
        refresh_cache(now);
      }
      for (const auto& arrivals : cache) {
        if (station == arrivals.station()) {
          return arrivals.muni_to_string();
        }
      }
      // fallthrough
      const sfmtc::muni::MuniStation mStation(station);
      std::string msg = "Not currently tracking station " + mStation.pretty_name();
      throw StationGetException(msg);
      break;
    }
    default: {
      throw StationTrackingException("Requested station is not in the SFMTC system");
      break;
    }
  }
}

std::vector<StationArrivals> SfmtcConverter::convert(const std::string& data) {
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
    std::vector<StationArrivals> trains;
    for (size_t i = 0; i < stations.size(); ++i) {
      std::string agencyCode;
      switch (stations[i].agency()) {
        case antioch::transit_base::TransitAgency::BART: {
          agencyCode = "BA";
          break;
        }
        case antioch::transit_base::TransitAgency::SF_MUNI: {
          agencyCode = "SF";
          break;
        }
        default:
          throw InvariantViolation("Tracked station is not in the SFMTC system");
      }
      std::vector<TrainArrival> arrival_vec;
      for (int j = 0; j < fm.entity_size(); ++j) {
        const auto& trip_update = fm.entity(j).trip_update();
        const auto& agency = trip_update.trip().route_id().substr(0, 2);
        const bool direction = trip_update.trip().direction_id();
        if (agency != agencyCode) {
          continue;
        }
        // switch (agencyCode)
        if (agencyCode == "BA") {
          const auto line = sfmtc::bart::line_of(trip_update);
          for (int k = 0; k < trip_update.stop_time_update_size(); ++k) {
            const auto& stop = trip_update.stop_time_update(k);
            if (sfmtc::bart::StationIdentifier_Name((sfmtc::bart::StationIdentifier)(stations[i].id())) == stop.stop_id()) {
              arrival_vec.push_back({{line, direction}, stop.arrival().time()});
            }
          }
        } else if (agencyCode == "SF") {
          const auto line = sfmtc::muni::line_of(trip_update);
          for (int k = 0; k < trip_update.stop_time_update_size(); ++k) {
            const auto& stop = trip_update.stop_time_update(k);
            if (sfmtc::muni::StationIdentifier_Name((sfmtc::muni::StationIdentifier)(stations[i].id())) == stop.stop_id()) {
              arrival_vec.push_back({{line, direction}, stop.arrival().time()});
            }
          }
        }
      }
      trains.emplace_back(stations[i], std::move(arrival_vec));
    }
    return trains;
  }
}

void SfmtcConverter::update_last_fetch(const std::chrono::time_point<std::chrono::system_clock>& now) {
  while (now > last_fetch) {
    last_fetch += std::chrono::seconds(refreshTimeSecs);
  }
  last_fetch -= std::chrono::seconds(refreshTimeSecs);
}

void SfmtcConverter::refresh_cache(const std::chrono::time_point<std::chrono::system_clock>& now) {
  std::string fetched;
  antioch::connectivity::curl_transfer::Latch latch(1);

  auto cb = [&](std::string s) {
    std::cout << "called cb" << std::endl;
    fetched.swap(s);
    latch.count_down();
  };
  std::string url = "http://api.511.org/transit/TripUpdates?api_key=" + API_KEY + "&agency=RG";
  std::cout << url << std::endl;
  antioch::connectivity::curl_transfer::start_transfer(url, false, cb);
  if (!latch.wait_or_timeout(std::chrono::seconds(15))) {
    std::cerr << "Timed out waiting for SFMTC API fetch!" << std::endl;
    return;
  }
  auto converted = convert(fetched);
  cache.swap(converted);
  update_last_fetch(now);
}

}  // namespace sfmtc

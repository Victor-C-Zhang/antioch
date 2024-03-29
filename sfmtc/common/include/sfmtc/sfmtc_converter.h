// Copyright Antioch. All rights reserved.
#pragma once

#include <mutex>

#include <antioch/transit_base/converter.h>
#include <gtfs-realtime.pb.h>

#include "station_arrivals.h"

namespace sfmtc {

class SfmtcConverter : public antioch::transit_base::Converter {
 public:
  static constexpr int refreshTimeSecs = 60;

  // TODO: this will leak memory if the converter is ever not needed, e.g. if we stop tracking any
  // SF agencies
  static SfmtcConverter* instance();

  /**
   * Start tracking the arrival times and lines into the desired station.
   * Guaranteed to be thread-safe. Will throw a StationTrackingException if the station is unable
   * to be tracked.
   * @param station the station details to be tracked.
   */
  void startTracking(const antioch::transit_base::Station& station) override;

  /**
   * Stop tracking the arrival times and lines into the station.
   * Guaranteed to be thread-safe. Does not throw.
   * @param station the station details to stop tracking.
   */
  void stopTracking(const antioch::transit_base::Station& station) override;

  std::string get(const antioch::transit_base::Station& station) override;
  /**
   * Convert the bytestring into new train info.
   */
  std::vector<StationArrivals> convert(const std::string& data);

 protected:
  std::string convert_to_string(const std::vector<std::byte>& data);

 private:
  static SfmtcConverter* singleton_instance;

  SfmtcConverter();

  /**
   * Increment last_fetch by REFRESH_TIME_SECS s.t. it is the greatest value less or equal to now.
   *
   */
  void update_last_fetch(const std::chrono::time_point<std::chrono::system_clock>& now);

  /**
   * Refresh the cache with newly fetched transit info.
   * Queries the backend API via wifi.
   * Converts and stores the new data in cache.
   * Updates the last_fetch timestamp.
   * @param now the timestamp "now" to update the timestamp with.
   * @throws exception if wifi fetch or conversion fails.
   */
  void refresh_cache(const std::chrono::time_point<std::chrono::system_clock>& now);
  
  std::vector<antioch::transit_base::Station> stations;
  std::vector<StationArrivals> cache;
  std::mutex stations_mtx;
  std::chrono::time_point<std::chrono::system_clock> last_fetch;
};

class InvariantViolation : public std::runtime_error {
 public:
  InvariantViolation(const std::string& what) : std::runtime_error(what) {}
};

}  // namespace sfmtc

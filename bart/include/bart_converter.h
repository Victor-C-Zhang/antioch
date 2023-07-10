// Copyright Antioch. All rights reserved.
#pragma once

#include <antioch/transit_base/converter.h>
#include <gtfs-realtime.pb.h>

#include <mutex>

#include "bart_station.h"
#include "station_arrivals.h"

namespace antioch {
namespace bart {

class BartConverter : public antioch::transit_base::Converter {
 public:
  static constexpr int refreshTimeSecs = 60;

  BartConverter();

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

 protected:
  std::string convert_to_string(const std::vector<std::byte>& data);

 private:
  /**
   * Convert the bytestring into new train info.
   */
  std::vector<StationArrivals> convert(const std::vector<std::byte>& data);

  /**
   * Increment last_fetch by REFRESH_TIME_SECS s.t. it is the greatest value less or equal to now.
   *
   */
  void update_last_fetch(std::chrono::time_point<std::chrono::system_clock>& now);

  TrainDescription line_of(const transit_realtime::TripUpdate& tu);

  std::vector<BartStation> stations;
  std::vector<StationArrivals> cache;
  std::mutex stations_mtx;
  std::chrono::time_point<std::chrono::system_clock> last_fetch;
};

class InvariantViolation : public std::runtime_error {
 public:
  InvariantViolation(const std::string& what) : std::runtime_error(what) {}
};

}  // namespace bart
}  // namespace antioch

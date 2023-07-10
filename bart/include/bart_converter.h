// Copyright Antioch. All rights reserved.
#pragma once

#include <antioch/transit_base/converter.h>

#include <mutex>

#include "bart_station.h"

namespace antioch {
namespace bart {

class BartConverter : public antioch::transit_base::Converter {
 public:
  /**
   * Start tracking the arrival times and lines into the desired station.
   * Guaranteed to be thread-safe. Will throw a StationTrackingException if the station is unable
   * to be tracked.
   * @param station the station details to be tracked.
   */
  void startTracking(const antioch::transit_base::Station& station);

  /**
   * Stop tracking the arrival times and lines into the station.
   * Guaranteed to be thread-safe. Does not throw.
   * @param station the station details to stop tracking.
   */
  void stopTracking(const antioch::transit_base::Station& station);

  /**
   * Convert the data gotten from whatever feed is specified into the proper display format.
   * @param data a (possibly binary) bytestring.
   * @return std::string
   */
  std::string convert(const std::vector<std::byte>& data);

 private:
  std::vector<BartStation> stations;
  std::mutex stations_mtx;
};

}  // namespace bart
}  // namespace antioch

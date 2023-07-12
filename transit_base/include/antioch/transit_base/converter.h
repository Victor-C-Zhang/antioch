// Copyright Antioch. All rights reserved.
#pragma once

#include <stdexcept>

#include "station.h"

namespace antioch {
namespace transit_base {

class Converter {
 public:
  /**
   * Start tracking the arrival times and lines into the desired station.
   * Guaranteed to be thread-safe. Will throw a StationTrackingException if the station is unable
   * to be tracked.
   * @param station the station details to be tracked.
   */
  virtual void startTracking(const Station& station) = 0;

  /**
   * Stop tracking the arrival times and lines into the station.
   * Guaranteed to be thread-safe. Does not throw.
   * @param station the station details to stop tracking.
   */
  virtual void stopTracking(const Station& station) = 0;

  /**
   * Convert the data gotten from whatever feed is specified into the proper display format.
   * @param data a (possibly binary) bytestring.
   * @return std::string
   */
  virtual std::vector<std::pair<antioch::transit_base::Station, std::vector<std::pair<TrainDescription, int64_t>>>> convert(const std::vector<std::byte>& data) = 0;
};

class StationTrackingException : public std::runtime_error {
 public:
  StationTrackingException(const std::string& what) : std::runtime_error(what) {}
};

}  // namespace transit_base
}  // namespace antioch

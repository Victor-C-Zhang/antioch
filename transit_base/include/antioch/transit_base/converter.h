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
   * Get the stringified version of the arrivals data for the given station.
   * This data need not be current, or even correct. It may be cached or even made up.
   * @param station the station to query.
   * @throw StationGetException in error cases.
   * @return std::string
   */
  virtual std::string get(const antioch::transit_base::Station& station) = 0;
};

class StationGetException : public std::runtime_error {
 public:
  StationGetException(const std::string& what) : std::runtime_error(what) {}
};

class StationTrackingException : public std::runtime_error {
 public:
  StationTrackingException(const std::string& what) : std::runtime_error(what) {}
};

}  // namespace transit_base
}  // namespace antioch

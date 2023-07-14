// Copyright Antioch. All rights reserved.

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

std::string exec(const char* cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

/**
 * @brief Schedule a cron-like job to run web queries and test your converter throughout the day
 *        or night. Takes 2 parameters:
 *          - a path ts_file to a list of unix timestamps to run your converter on.
 *            the format of the file should be as follows:
 *              one line with a single integer, the number n of timestamps to test on, followed by
 *              n lines each with a single unix timestamp
 *          - a path out_dir a directory in which to put the wget and conversion
 *            results
 *
 *        Sample usage: ./moonlight-vigil /home/user/timestamps.txt ./results
 */
int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Usage: moonlight_vigil [ts_file] [out_dir]" << std::endl;
    return 1;
  }
  std::string ts_file_path = argv[1];
  std::string out_dir_path = argv[2];

  std::ifstream ts_file{ts_file_path};
  std::vector<int64_t> ts;
  int n;
  ts_file >> n;
  int64_t ts_temp;
  while (n--) {
    ts_file >> ts_temp;
    ts.push_back(ts_temp);
  }
  ts_file.close();
  std::sort(ts.begin(), ts.end());
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  auto it = std::upper_bound(ts.begin(), ts.end(), now);
  if (it != ts.begin()) {
    ts.erase(ts.begin(), it);
  }
  std::cout << "Scheduled: [" << std::endl;
  for (auto unix_ts : ts) {
    std::cout << std::ctime(&unix_ts);
  }
  std::cout << "]" << std::endl;
  std::cout << "Press Ctrl+C to terminate at any time." << std::endl;

  for (auto unix_ts : ts) {
    auto this_run_out_dir = out_dir_path + "/" + std::to_string(unix_ts);
    auto raw_feed = this_run_out_dir + "/raw_feed";
    auto converted_result = this_run_out_dir + "/converted.txt";
    std::filesystem::create_directories(this_run_out_dir);
    auto now_ts = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto sleep_period = unix_ts - now_ts;
    if (sleep_period > 0) {
      std::this_thread::sleep_for(std::chrono::seconds(sleep_period));
    }
    std::cout << "Running: " << std::ctime(&unix_ts);
    try {
      std::string wgetCmd = "wget -O " + raw_feed + " https://api.bart.gov/gtfsrt/tripupdate.aspx";
      exec(wgetCmd.c_str());
    } catch (const std::exception& e) {
      std::cout << "wget failed with " << e.what() << std::endl;
      continue;
    }
    std::ofstream out{converted_result};
    try {
      // TODO convert
      std::cout << "\tSuccessful" << std::endl;
    } catch (const std::exception& e) {
      std::cout << "Conversion failed with " << e.what() << std::endl;
      out << e.what() << std::endl;
    }
    out.close();
  }

  return 0;
}

// Copyright Antioch. All rights reserved.

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: moonlight_vigil [ts_file] [out_dir]" << std::endl;
  }
  std::string ts_file_path;
  std::string out_dir_path;

  std::ifstream ts_file{ts_file_path};
  std::vector<int64_t> ts;
  int n; std::cin >> n;
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
    // TODO grab the file from online
    std::ofstream out{converted_result};
    try {
      // TODO convert
      std::cout << "\tSuccessful" << std::endl;
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      out << e.what() << std::endl;
    }
    out.close();
  }

  return 0;
}

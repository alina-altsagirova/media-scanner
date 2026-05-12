#ifndef MAIN_H
#define MAIN_H

// #define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>

class MediaScanner {
public:
  int run(char *argv1, char *argv2);

private:
  httplib::Server srv;
  nlohmann::json report;
  int scan_main_dir();
  void save_report_local(nlohmann::json &report, std::string home_path);
  // void send_report(nlohmann::json &report);
};

#endif
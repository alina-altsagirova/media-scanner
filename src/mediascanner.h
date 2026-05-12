#ifndef MEDIASCUNNER_H
#define MEDIASCUNNER_H

#include "httplib.h"
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <algorithm>
#include <string_view>

class MediaScanner {
public:
  int run(char *argv1, char *argv2, char* argv3);

private:
  httplib::Server srv;
  nlohmann::json report;
  std::string media_path;
  std::mutex report_mutex;
  int scan_dir();
  void save_report_local(nlohmann::json &report);
};

#endif
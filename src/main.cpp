#include "main.h"

int MediaScanner::run(char *argv1, char *argv2) {
  using namespace std::chrono_literals;
  auto timer_dur = 500ms;

  if (argv1 != NULL && argv2 != NULL) {
    //нормиальную проверку надо на число
    int value = std::stoi(argv1);
    if (value == 1) return 1;
    std::string unit = argv2;
    if (unit == "s")
      timer_dur = std::chrono::seconds(value);
    else if (unit == "m")
      timer_dur = std::chrono::minutes(value);
    else if (unit == "ms")
      timer_dur = std::chrono::milliseconds(value);
    else if (unit == "h")
      timer_dur = std::chrono::hours(value);
    else if (unit == "d")
      timer_dur = std::chrono::hours(value * 24);
    else if (unit == "w")
      timer_dur = std::chrono::hours(value * 24 * 7);
    else {
      return 1;
    }
  }
  std::thread([this]() {
    srv.Get("/", [&](const httplib::Request &, httplib::Response &res) {
      std::cout << "!\n";
      std::string json_data = report.dump(4);
      res.set_content(json_data, "application/json");
    });
    srv.listen("localhost", 1234);
  }).detach();

  while (1) {
    scan_main_dir();
    std::this_thread::sleep_for(timer_dur);
  }

  return 0;
}

int MediaScanner::scan_main_dir() {
  using json = nlohmann::json;
  std::string home_path = std::getenv("HOME");
  //кастомный путь надо и переименовать функ
  json report;
  for (auto entry : std::filesystem::directory_iterator(home_path)) {
    auto file_ext = entry.path().extension();
    auto file_name = entry.path().filename();
    if (file_ext == ".wav" || file_ext == ".mp3")
      report["audio"] = file_name;
    if (file_ext == ".mpg")
      report["video"] = file_name;
    if (file_ext == ".jpeg" || file_ext == ".png")
      report["images"] = file_name;
  }
  std::cout << report.dump() << std::endl;
  this->report = report;
  save_report_local(report, home_path);
  return 0;
}

void MediaScanner::save_report_local(nlohmann::json &report,
                                     std::string home_path) {

  std::filesystem::path report_dir =
      std::filesystem::path(home_path) / ".media_files";
  std::filesystem::create_directory(report_dir);
  std::ofstream file(report_dir / "report.json");
  if (file.is_open()) {
    file << report.dump(4);
    file.close();
  }
}

int main(int argc, char *argv[]) {
  MediaScanner MediaScanner;
  if (argc > 1) {
    if ((MediaScanner.run(argv[1], argv[2]) == 1)) {
      std::cerr << "Invalid argument\n";
      return 1;
    }
  } else {
    std::cerr << "Error. Please enter timer arguments. \nArguments format: "
                 "{number} {unit}. \nExample: 3 s, 100 ms, 5 m, 1 d, 1 w. \n";
  }
  return 0;
}
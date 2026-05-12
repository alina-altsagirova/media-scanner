#include "main.h"

int MediaScanner::run(char *argv1, char *argv2, char *argv3) {
  using namespace std::chrono_literals;
  auto timer_dur = 500ms;

  if (argv1 != NULL && argv2 != NULL && argv3 != NULL) {
    std::string_view arg = argv1;
    int is_numeric = std::ranges::all_of(arg, [](char c) {  
        return std::isdigit(static_cast<unsigned char>(c));
    });
    if(!is_numeric) return 1;
    if(std::filesystem::is_directory(argv3)) media_path = argv3;
    else return 1;


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
    srv.Get("/media_files", [this](const httplib::Request &, httplib::Response &res) {
      std::cout << "!\n";
      std::lock_guard<std::mutex> lock(report_mutex);
      std::string json_data = MediaScanner::report.dump();
      res.set_content(json_data, "application/json");
    });
    srv.listen("localhost", 1234);
  }).detach();

  while (1) {
    scan_dir();
    std::this_thread::sleep_for(timer_dur);
  }

  return 0;
}

int MediaScanner::scan_dir() {
  using json = nlohmann::json;
  //std::string home_path = std::getenv("HOME");
  json report;
  for (auto entry : std::filesystem::directory_iterator(media_path)) {
    auto file_ext = entry.path().extension();
    auto file_name = entry.path().filename();
    if (file_ext == ".wav" || file_ext == ".mp3")
      report["audio"].push_back(file_name);
    if (file_ext == ".mpg")
      report["video"].push_back(file_name);
    if (file_ext == ".jpeg" || file_ext == ".png")
      report["images"].push_back(file_name);
  }
  std::cout << report.dump() << std::endl;
     std::lock_guard<std::mutex> lock(this->report_mutex);
  this->report = report;
  save_report_local(report);
  return 0;
}

void MediaScanner::save_report_local(nlohmann::json &report) {
  std::filesystem::path report_dir =
      std::filesystem::path(media_path)/ ".media_files";
  std::filesystem::create_directory(report_dir);
  std::ofstream file(report_dir / "report.json");
  if (file.is_open()) {
    file << report.dump();
    file.close();
  }
}

int main(int argc, char *argv[]) {
  MediaScanner scanner;
  if (argc > 3) {
    if ((scanner.run(argv[1], argv[2], argv[3]) == 1)) {
      std::cerr << "Invalid argument\n";
      return 1;
    }
  } else {
    std::cerr << "Error. Please enter correct arguments. \nArguments format: "
                 "[number] [unit] [path]. \nExample: 3 s ~, 100 ms ~/Documents, 5 m ~, 1 d ~, 1 w ~. \n";
  }
  return 0;
}
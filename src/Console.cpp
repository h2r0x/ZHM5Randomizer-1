#include "Console.h"

#include <Windows.h>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"


#include "Config.h"

void Console::spawn() {
  AllocConsole();
  FILE *stream;
  freopen_s(&stream, "CONOUT$", "w", stdout);
  // auto MFCConsole = spdlog::stdout_color_mt("MFCConsole");
  auto base_directory_ =
      std::filesystem::current_path().generic_string(); //..\\HITMAN2
  auto log_path = base_directory_ + "\\Retail\\ZHM5Randomizer.log";

  try {
  auto logger = spdlog::basic_logger_mt("console", log_path, true);
  logger->flush_on(spdlog::level::debug);
  logger->set_level(spdlog::level::debug);
  logger->info("Console::spawn()");

  } catch (const spdlog::spdlog_ex &ex) {
    std::cout << "Log init failed: " << ex.what() << std::endl;
  }
}

// TODO: Implement file logging properly
void Console::log(const char *fmt, ...) {
  // va_list ap;
  // va_start(ap, fmt);
  // vprintf(fmt, ap);

  // char buf[256];
  // vsprintf_s(buf, fmt, ap);

  // base_directory_ =
  //     std::filesystem::current_path().generic_string(); //..\\HITMAN2

  // auto ini_path = base_directory_ + "\\Retail\\ZHM5Randomizer.log";

  // std::ofstream ofs(log_path.c_str(), std::ofstream::out ||
  // std::ofstream::app); if (!ofs.is_open()) {
  //   printf("Failed to open log file: %s\n", log_path.c_str());
  // }

  // ofs.write(buf, strlen(buf));
  // ofs.close();
  //}
  // else {
  //	vprintf(fmt, ap);
  //}
}

#include "LoggerApplication.hpp"

#include <common/J2534ChannelProvider.hpp>
#include <j2534/J2534.hpp>
#include <logger/Logger.hpp>

#include <windows.h>

namespace logger {

/*static*/ LoggerApplication &LoggerApplication::instance() {
  static LoggerApplication s_app;
  return s_app;
}

void LoggerApplication::start(j2534::J2534 &j2534,
                              const LogParameters &params,
                              const common::CarPlatform carPlatform,
                              uint32_t cmId,
                              const std::vector<LoggerCallback *> &callbacks) {
  auto channelProvider = std::make_shared<common::J2534ChannelProvider>(j2534, carPlatform);
  _logger = std::make_unique<Logger>(std::move(channelProvider), carPlatform, cmId, std::string());
  for (const auto &callback : callbacks) {
    _logger->registerCallback(*callback);
  }
  _logger->start(params);
}

void LoggerApplication::stop() {
  if (nullptr != _logger) {
    _logger->stop();
    _logger.reset();
  }
}

bool LoggerApplication::isStarted() const { return _logger != nullptr; }

LoggerApplication::LoggerApplication() {}

LoggerApplication::~LoggerApplication() { stop(); }

} // namespace logger

//
// log
// Created by Nevd on 19/02/2018.
//

#ifndef FIRE_LOG_H
#define FIRE_LOG_H

#define SPDLOG_TRACE_ON
#define SPDLOG_DEBUG_ON
#include "spdlog/spdlog.h"

extern std::shared_ptr<spdlog::logger> _main_logger;

#define LOG_TRACE_DETAIL(...) _main_logger->trace("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)
#define LOG_DEBUG_DETAIL(...) _main_logger->debug("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)
#define LOG_WARN_DETAIL(...) _main_logger->warn("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)
#define LOG_ERROR_DETAIL(...) _main_logger->error("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)
#define LOG_FATAL_DETAIL(...) _main_logger->critical("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)

#define LOG_TRACE(...) _main_logger->trace(__VA_ARGS__)
#define LOG_DEBUG(...) _main_logger->debug(__VA_ARGS__)
#define LOG_WARN(...) _main_logger->warn(__VA_ARGS__)
#define LOG_ERROR(...) _main_logger->error(__VA_ARGS__)
#define LOG_FATAL(...) _main_logger->critical(__VA_ARGS__)


int init_log();

#endif //FIRE_LOG_H

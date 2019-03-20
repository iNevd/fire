//
// log
// Created by Nevd on 19/02/2018.
//

#ifndef FIRE_LOG_H
#define FIRE_LOG_H

#define SPDLOG_TRACE_ON
#define SPDLOG_DEBUG_ON
#include "spdlog/spdlog.h"

namespace FIRE
{
    extern std::shared_ptr<spdlog::logger> _main_logger;
    int init_log();
}

#define LOG_TRACE_DETAIL(...) FIRE::_main_logger->trace("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)
#define LOG_DEBUG_DETAIL(...) FIRE::_main_logger->debug("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)
#define LOG_WARN_DETAIL(...) FIRE::_main_logger->warn("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)
#define LOG_ERROR_DETAIL(...) FIRE::_main_logger->error("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)
#define LOG_FATAL_DETAIL(...) FIRE::_main_logger->critical("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)

#define LOG_TRACE(...) FIRE::_main_logger->trace(__VA_ARGS__)
#define LOG_DEBUG(...) FIRE::_main_logger->debug(__VA_ARGS__)
#define LOG_WARN(...) FIRE::_main_logger->warn(__VA_ARGS__)
#define LOG_ERROR(...) FIRE::_main_logger->error(__VA_ARGS__)
#define LOG_FATAL(...) FIRE::_main_logger->critical(__VA_ARGS__)



#endif //FIRE_LOG_H

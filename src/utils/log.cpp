//
// log
// Created by Nevd on 19/02/2018.
//

#include "log.h"

using namespace FIRE;

std::shared_ptr<spdlog::logger> FIRE::_main_logger = spdlog::stdout_color_mt("main");

int FIRE::init_log() {
    // TODO loglevel options
    _main_logger->set_level(spdlog::level::trace);
    return 0;
}

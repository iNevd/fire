//
// log
// Created by Nevd on 19/02/2018.
//

#include "log.h"

auto _main_logger = spdlog::stdout_color_mt("main");

int init_log() {
    _main_logger->set_level(spdlog::level::trace);
    return 0;
}

#include <iostream>
#include <core/ServerMain.h>
#include <csignal>
#include "utils/signal_process.h"
#include "utils/log.h"



int main() {
    // 0. 日志初始化
    FIRE::init_log();

    // 1. 初始化信号处理
    FIRE::signal_ignore(SIGPIPE);
    FIRE::signal_process(SIGTERM);

    // 2. 初始化主逻辑
    FIRE::ServerMain::getInstance()->init();

    // 3. 开始跑
    FIRE::ServerMain::getInstance()->run();
    return 0;
}

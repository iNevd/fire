#include <iostream>
#include <core/ServerMain.h>
#include <csignal>
#include "utils/log.h"


void signal_process(int sig) {
    if (sig == SIGTERM) {
        LOG_WARN("safe kill signal catched");
        FIRE::ServerMain::getInstance()->stop();

        LOG_TRACE("all server stoped");
        usleep(10 * 1000); /* waiting for log thread */
        exit(sig);
    } else{
        LOG_WARN("unexpected signal catched. ignore it.", sig);
        return;
    }
}

int main() {
    FIRE::init_log();
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, signal_process);
    FIRE::ServerMain::getInstance()->init();
    FIRE::ServerMain::getInstance()->run();
    return 0;
}

#include <iostream>
#include <core/ServerMain.h>
#include "utils/log.h"

void signal_process(int sig);
int main() {
    init_log();
    auto& serverMain = ServerMain::getInstance();
    serverMain.init();
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, signal_process);
    serverMain.run();
    return 0;
}

void signal_process(int sig) {
    if (sig == SIGTERM) {
        LOG_WARN("safe kill signal catched");
        auto& serverMain = ServerMain::getInstance();
        serverMain.stop();

        LOG_TRACE("all server stoped");
        usleep(10 * 1000); /* waiting for log thread */
        exit(-1);
    } else{
        LOG_WARN("unexpected signal catched. ignore it. {}", sig);
        return;
    }
}

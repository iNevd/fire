//
// Created by 陈征 on 2019/3/29.
//

#ifndef FIRE_SIGNAL_PROCESS_H
#define FIRE_SIGNAL_PROCESS_H

namespace FIRE {
    void _signal_process(int sig) {
        if (sig == SIGTERM) {
            LOG_WARN("safe kill signal catched");
            FIRE::ServerMain::getInstance()->stop();

            LOG_TRACE("all server stoped");
            usleep(10 * 1000); /* waiting for log thread */
            exit(sig);
        } else{
            LOG_WARN("unexpected signal catched. ignore it. {}", sig);
            return;
        }
    }

    void signal_process(int sig) {
        signal(sig, _signal_process);
    }

    void signal_ignore(int sig) {
        signal(sig, SIG_IGN);
    }
}

#endif //FIRE_SIGNAL_PROCESS_H

//
// Server
// Created by Nevd on 14/02/2018.
//

#ifndef FIRE_SERVER_H
#define FIRE_SERVER_H

#include <string>
#include <thread>
#include <atom_val.h>

#include "event/watcher/IOWatcher.h"
#include "event/watcher/TimerWatcher.h"
#include "event/EventLoop.h"
#include "common/include.h"

namespace FIRE
{
    class Server {
    public:
        Server() = delete;
        Server(bool useDefaultEvLoop);
        virtual ~Server();
        virtual int init();
        virtual void run();
        virtual void stop();

        // notify
        virtual int notify(caf::atom_value && msg);

    protected:

        // callback
        virtual void notice_cb(int fd, int event);
        virtual void cron_cb();

    protected:
        EventLoop _eventLoop;
        int _notice_recv_fd = 0;
        int _notice_send_fd = 0;
        IWatcher* _notify_watcher = nullptr;
        IWatcher* _cron_watcher = nullptr;
    };
}

#endif //FIRE_SERVER_H

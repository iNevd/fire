//
// Server
// Created by Nevd on 14/02/2018.
//

#include <zconf.h>
#include "utils/log.h"
#include "utils/util.h"
#include "Server.h"

using namespace FIRE;

Server::Server(bool useDefaultEvLoop) : _eventLoop(useDefaultEvLoop, this) { }


int Server::init() {
    // create pipe
    int fds[2] = {0};
    if (pipe(fds)) {
        LOG_ERROR_DETAIL("create pipe failed");
        return FAILED;
    }
    _notice_recv_fd = fds[0];
    _notice_send_fd = fds[1];
    _notify_watcher = new IOWatcher(_notice_recv_fd, IOWatcher::READ,
                                     [](int fd, int event, void* private_data) {
                                         auto server = reinterpret_cast<decltype(this)>(private_data);
                                         server->notice_cb(fd, event);
                                     }
            , this);
    _eventLoop.start_watcher(_notify_watcher);

    // create timer
    _cron_watcher = new TimerWatcher(
            [](void* private_data) {
                auto server = reinterpret_cast<decltype(this)>(private_data);
                server->cron_cb();
            }, this
    );
    // 定时器需要自己启动
    //    // 设定定时器(可由子类自己实现)
    //    auto timer_watcher = dynamic_cast<TimerWatcher*>(_cron_watcher);
    //    timer_watcher->set_usec(100000);
    //    timer_watcher->set_repeat(true);
    //    _eventLoop.start_watcher(timer_watcher);
    //_eventLoop.start_watcher(_cron_watcher);
    return SUCCEEDED;

}

void Server::run() {
    _eventLoop.run();
}

void Server::stop() {
    if(_notify_watcher != nullptr) {
        _eventLoop.delete_watcher(_notify_watcher, IOWatcher::READ | IOWatcher::WRITE | IOWatcher::NONE);
    }

    if(_cron_watcher != nullptr) {
        _eventLoop.delete_watcher(_cron_watcher);
    }
    _eventLoop.stop();
}

int Server::notify(caf::atom_value && msg) {
    LOG_DEBUG_DETAIL("notify fd: {}  msg: {}", _notice_recv_fd, static_cast<uint64_t >(msg));
    if(write(_notice_send_fd, &msg, sizeof(caf::atom_value)) == sizeof(caf::atom_value)) {
        return SUCCEEDED;
    } else {
        LOG_ERROR_DETAIL("notify failed, errno: {}", errno);
        return FAILED;
    }
}

void Server::notice_cb(int fd, int event) {
    UNUSED(fd);
    UNUSED(event);
    return;
}

void Server::cron_cb() {
    return;
}


//
// IOWatcher
// Created by Nevd on 14/02/2018.
//

#include "IOWatcher.h"
#include "utils/util.h"

IOWatcher::IOWatcher(int fd, int event, IOWatcher::io_cb_t cb, void *priv_data)
    : _fd(fd), _events(event), _cb(cb), _priv_data(priv_data)
{
    _io.data = this;

    ev_init(&_io,
            [](struct ev_loop *loop, struct ev_io *io, int event) {
                UNUSED(loop);
                auto watcher = reinterpret_cast<decltype(this)>(io->data);
                watcher->_cb(io->fd, event, watcher->_priv_data);
            }
    );
}

// 启动某一项event
void IOWatcher::start_event(struct ev_loop *loop, Event event) {
    /* If there's no change on the events, just return */
    _events |= event;
    if (ev_is_active(&_io)) {
        auto active_events = _io.events;
        int events = active_events | _events;// | mask;
        if (active_events == events) {
            return;
        }
        ev_io_stop(loop, &_io);
        ev_io_set(&_io, _fd, events);
        ev_io_start(loop, &_io);
    } else {
        auto events = _events;
        ev_io_set(&_io, _fd, events);
        ev_io_start(loop, &_io);
    }
}

// 停止某一项event
void IOWatcher::stop_event(struct ev_loop *loop, Event event) {
    auto active_events = _io.events;
    auto events = active_events & (~event);
    if (active_events == events)
        return;
    ev_io_stop(loop, &_io);
    if (events != NONE) {
        ev_io_set(&_io, _io.fd, events);
        ev_io_start(loop, &_io);
    }
}

int IOWatcher::get_fd() const {
    return _fd;
}
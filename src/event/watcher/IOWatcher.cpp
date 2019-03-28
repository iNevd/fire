//
// IOWatcher
// Created by Nevd on 14/02/2018.
//

#include "IOWatcher.h"
#include "utils/util.h"

using namespace FIRE;
IOWatcher::IOWatcher(int fd, int event, IOWatcher::io_cb_t cb, void *private_data)
    :_cb(cb),
     _private_data(private_data)
{
    _io.data = this;
    ev_io_init(
            &_io,
            [](struct ev_loop *loop, struct ev_io *io, int event) {
                UNUSED(loop);
                auto watcher = reinterpret_cast<decltype(this)>(io->data);
                watcher->_cb(io->fd, event, watcher->_private_data);
            },
            fd,
            event
    );
}

// 启动某一项event
void IOWatcher::start_event(struct ev_loop *loop, Event event) {
    /* If there's no change on the events, just return */
    if (ev_is_active(&_io)) {
        auto events_old = get_events();
        auto events_new = get_events() | event;// | mask;
        if (events_old == events_new) {
            return;
        }
        ev_io_stop(loop, &_io);
        _io.events = events_new;
        ev_io_start(loop, &_io);
    } else {
        _io.events = event;
        ev_io_start(loop, &_io);
    }
}

// 停止某一项event
void IOWatcher::stop_event(struct ev_loop *loop, Event event) {
    auto events_old = get_events();
    auto events_new = events_old & (~event);
    if (events_old == events_new)
        return;
    ev_io_stop(loop, &_io);
    if (events_new != NONE) {
        ev_io_set(&_io, _io.fd, events_new);
        ev_io_start(loop, &_io);
    }
}

int IOWatcher::get_fd() const {
    return _io.fd;
}

int IOWatcher::get_events() const {
    return _io.events;
}
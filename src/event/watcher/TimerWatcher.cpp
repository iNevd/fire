//
// TimerWatcher
// Created by Nevd on 14/02/2018.
//

#include "TimerWatcher.h"
#include "utils/util.h"

using namespace FIRE;
TimerWatcher::TimerWatcher(TimerWatcher::timer_cb_t cb, void* private_data, Event event, double repeat)
        : _cb(cb), _private_data(private_data) {
    _timer.data = this;
    ev_timer_init(&_timer,
            [](struct ev_loop *loop, struct ev_timer *timer, int event) {
                UNUSED(loop);
                UNUSED(event);
                auto watcher = reinterpret_cast<decltype(this)>(timer->data);
                watcher->_cb(watcher->_private_data);
            },
            event / 1000000.0,
            repeat
    );
}

void TimerWatcher::start_event(struct ev_loop *loop, Event event) {
    if(event > NONE) {
        ev_timer_stop(loop, &_timer);
        ev_timer_set(&_timer, event / 1000000.0, _timer.repeat);
        ev_timer_start(loop, &_timer);
    }
}


void TimerWatcher::stop_event(struct ev_loop *loop, Event unused) {
    UNUSED(unused);
    ev_timer_stop(loop, &_timer);
}

void TimerWatcher::set_repeat(struct ev_loop *loop, double repeat) {
    if(repeat < .0) {
        // close repeat
        repeat = .0;
    }
    _timer.repeat = repeat;
    ev_timer_again(loop, &_timer);
}



//
// TimerWatcher
// Created by Nevd on 14/02/2018.
//

#include "TimerWatcher.h"
#include "utils/util.h"

TimerWatcher::TimerWatcher(TimerWatcher::timer_cb_t cb, void* priv_data, bool repeat)
        : _cb(cb), _repeat(repeat), _priv_data(priv_data) {
    _timer.data = this;
    // create timer
    ev_init(&_timer,
            [](struct ev_loop *loop, struct ev_timer *timer, int event) {
                UNUSED(loop);
                UNUSED(event);
                auto watcher = reinterpret_cast<decltype(this)>(timer->data);
                watcher->_cb(watcher->_priv_data);
            }
    );
}

void TimerWatcher::start_event(struct ev_loop *loop, Event usec) {
    if(usec > NONE) {
        set_usec(static_cast<long>(usec));
    }
    auto sec = _usec / 1000000.0;
    if (!_repeat) {
        ev_timer_stop(loop, &_timer);
        ev_timer_set(&_timer, sec, 0);
        ev_timer_start(loop, &_timer);
    } else {
        /* We can change the timeout on the fly */
        _timer.repeat = sec;
        ev_timer_again(loop, &_timer);
    }
}


void TimerWatcher::stop_event(struct ev_loop *loop, Event unused) {
    UNUSED(unused);
    ev_timer_stop(loop, &_timer);
}



void TimerWatcher::set_usec(unsigned long _usec) {
    if(_usec > NONE) {
        TimerWatcher::_usec = _usec;
    }
}

unsigned long TimerWatcher::get_usec() const {
    return _usec;
}

void TimerWatcher::set_repeat(bool _repeat) {
    TimerWatcher::_repeat = _repeat;
}

bool TimerWatcher::is_repeat() const {
    return _repeat;
}


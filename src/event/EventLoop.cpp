//
// EventLoop
// Created by Nevd on 14/02/2018.
//

#include <iostream>
#include <event/watcher/IOWatcher.h>
#include "EventLoop.h"

using namespace FIRE;
EventLoop * EventLoop::_default_loop = nullptr;

EventLoop::EventLoop(bool use_default, void *el_owner) {
    if(el_owner) {
        _owner = el_owner;
    }
    if (use_default) {
        _loop = EV_DEFAULT;
        _default_loop = this;
        //log_debug("init default event loop");
    } else {
        _loop = ev_loop_new(EVFLAG_AUTO);
        //log_debug("init event loop");
    }
    ev_set_userdata(_loop, reinterpret_cast<void*>(this));
}


EventLoop::~EventLoop() {

}

void EventLoop::run() {
    ev_run(_loop, 0);
}

void EventLoop::suspend() {
    ev_suspend(_loop);
}

void EventLoop::resume() {
    ev_resume(_loop);
}

void EventLoop::stop() {
    ev_break(_loop, EVBREAK_ALL);
}

//      为 event 提供 add, start, stop, remove 4 中方法

void EventLoop::start_watcher(IWatcher* watcher, Event event) {
    if(watcher == nullptr) {
        return;
    }
    if(_loop == nullptr) {
        return;
    }

    watcher->start_event(_loop, event);
}
void EventLoop::stop_watcher(IWatcher* watcher, Event event) {
    if(watcher == nullptr) {
        return;
    }
    if(_loop == nullptr) {
        return;
    }
    watcher->stop_event(_loop, event);
}

void EventLoop::delete_watcher(IWatcher*& watcher, Event event) {
    if(watcher == nullptr) {
        return;
    }
    if(_loop == nullptr) {
        return;
    }

    this->stop_watcher(watcher, event);
    delete watcher;
    watcher = nullptr;
}



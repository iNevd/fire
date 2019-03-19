//
// EventLoop
// Created by Nevd on 14/02/2018.
//

#ifndef FIRE_EVENTLOOP_H
#define FIRE_EVENTLOOP_H

#include "ev.h"
#include "watcher/IWatcher.h"

namespace FIRE
{
    class EventLoop {
    public:
        EventLoop() = delete;

    public:
        virtual ~EventLoop();

    public:
//      ev 控制
        EventLoop(bool use_default, void* el_owner = nullptr);

        void run();
        void suspend();
        void resume();
        void stop();

//      为 watcher 提供 start, stop, remove 3 中方法
        void start_watcher(IWatcher* watcher, Event event = EVENT_NONE);
        void stop_watcher(IWatcher* watcher, Event event = EVENT_NONE);
        void delete_watcher(IWatcher*& watcher, Event event = EVENT_NONE);


    private:
        /* reference to owner of the EventLoop */
        void* _owner = nullptr;

        struct ev_loop* _loop = nullptr;
        /* Reference to the default loop, not thread safe,
       only used for current_time()  */
        static EventLoop* _default_loop;
    };
}

#endif //FIRE_EVENTLOOP_H

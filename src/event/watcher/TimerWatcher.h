//
// TimerWatcher
// Created by Nevd on 14/02/2018.
//

#ifndef FIRE_TIMERWATCHER_H
#define FIRE_TIMERWATCHER_H

#include <functional>
#include "IWatcher.h"
namespace FIRE
{
    class TimerWatcher : public IWatcher{
    public:
        static const Event NONE = EVENT_NONE;
    public:
        using timer_cb_t = std::function<void(void* private_data)>;

        TimerWatcher() = delete;
        TimerWatcher(timer_cb_t cb, void* private_data = nullptr, Event usec = 0, double repeat = .0);
    public:
        void start_event(struct ev_loop *, Event event = NONE) override;
        void stop_event(struct ev_loop *, Event event = NONE) override;
        void set_repeat(struct ev_loop*, double repeat);
    private:
        struct ev_timer _timer;
        timer_cb_t _cb = nullptr;
        // 私有数据, 外部负责其生命周期
        void* _private_data = nullptr;
    };
}

#endif //FIRE_TIMERWATCHER_H

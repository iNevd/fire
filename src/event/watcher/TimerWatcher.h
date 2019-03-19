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
        TimerWatcher(timer_cb_t cb, void* private_data = nullptr, bool repeat = false);
    public:
        void start_event(struct ev_loop *, Event event = NONE) override;
        void stop_event(struct ev_loop *, Event event = NONE) override;
        inline void set_usec(unsigned long _usec);
        inline unsigned long get_usec() const;
        void set_repeat(bool _repeat);
        bool is_repeat() const;

    private:
        struct ev_timer _timer;
        unsigned long _usec = 0;        // usec/1000000.0 timer
        timer_cb_t _cb = nullptr;
        bool _repeat = false;
        void* _private_data = nullptr;
    };
}

#endif //FIRE_TIMERWATCHER_H

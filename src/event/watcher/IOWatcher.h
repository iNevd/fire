//
// IOWatcher
// Created by Nevd on 14/02/2018.
//

#ifndef FIRE_IOWATCHER_H
#define FIRE_IOWATCHER_H

#include <functional>
#include "IWatcher.h"

namespace FIRE
{
    class IOWatcher : public IWatcher {
    public:
        enum : Event {
            NONE    = EVENT_NONE,
            READ    = 0x01,
            WRITE   = 0x02
        };

        using io_cb_t = std::function<void(int fd, int event, void* private_data)>;

        IOWatcher() = delete;
        IOWatcher(int fd, int event, io_cb_t cb, void* private_data = nullptr);
    public:
        void start_event(struct ev_loop *, Event event = NONE) override;
        void stop_event(struct ev_loop *, Event event = NONE) override;

    public:
        inline int get_fd() const;
        inline int get_events() const;

    private:
        // 私有回调函数
        io_cb_t _cb;

        // 私有数据, 外部负责其生命周期
        void* _private_data = nullptr;

        // 框架内部的IO结构体
        struct ev_io _io;
    };

}

#endif //FIRE_IOWATCHER_H

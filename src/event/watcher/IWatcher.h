//
// IWatcher
// Created by Nevd on 14/02/2018.
//

#ifndef FIRE_IWATCHER_H
#define FIRE_IWATCHER_H

#include "ev.h"

using Event = unsigned long;
static const Event EVENT_NONE = 0;

class IWatcher {

public:
    IWatcher() = default;
    virtual ~IWatcher() = default;

public:
    virtual void start_event(struct ev_loop *, Event event = EVENT_NONE) = 0;
    virtual void stop_event(struct ev_loop *, Event event = EVENT_NONE) = 0;
};
#endif //FIRE_IWATCHER_H

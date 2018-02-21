//
// time
// Created by Nevd on 16/02/2018.
//

#ifndef FIRE_TIME_H
#define FIRE_TIME_H

#include <chrono>

namespace fire {
    inline unsigned long unixMilliseconds() {
        return std::chrono::system_clock::now().time_since_epoch() /
               std::chrono::milliseconds(1);
    }

    inline unsigned long mnow() {
        return unixMilliseconds();
    }

    inline unsigned long unixSeconds() {
        return unixMilliseconds()/1000;
    }

    inline unsigned long now() {
        return unixSeconds();
    }
}

#endif //FIRE_TIME_H

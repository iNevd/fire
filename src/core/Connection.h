//
// Created by Nevd on 27/10/2017.
//

#ifndef FIRE_CONNECTION_H
#define FIRE_CONNECTION_H

#include <list>
#include <event/watcher/IOWatcher.h>
#include <event/watcher/TimerWatcher.h>
#include "sds.h"
#include "leveldb/slice.h"

namespace FIRE
{
    class Connection {
    public:
        Connection() = delete;
        Connection(int fd, void* owner);
        ~Connection();


    public:
        // fd, from accept 4
        int _fd = 0;

        // client port 4
        int _port = 0;

        // read buffer 8
        sds _readbuf = nullptr;

        // this's owner (a worker's pointer) 8
        void* _owner = nullptr;

        // io & timer info 8*2
        IWatcher *_tcp_io_watcher;
        IWatcher *_timer_watcher;

        // timeout control 8*3
        unsigned long _begin_interaction = 0;
        unsigned long _last_interaction = 0;
        unsigned long _connection_timeout = 0;

        // write buffer 24
        std::list<leveldb::Slice> _reply_list;
        // 24
        std::list<leveldb::Slice>::size_type _reply_list_size = 0;
        size_t _cur_resp_pos = 0;

        // client ip string  24Byte
        std::string _ip;
    };
}

#endif //FIRE_CONNECTION_H

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
        // fd, from accept
        int _fd = 0;

        // client ip string
        std::string _ip;

        // client port
        int _port = 0;

        // this's owner (a worker's pointer)
        void* _owner = nullptr;

        // read buffer
        sds _readbuf = nullptr;

        // write buffer
        std::list<leveldb::Slice> _reply_list;
        std::list<leveldb::Slice>::size_type _reply_list_size = 0;
        size_t _cur_resp_pos = 0;

        // io & timer info
        IWatcher *_tcp_io_watcher;
        IWatcher *_timer_watcher;

        // timeout control
        unsigned long _begin_interaction = 0;
        unsigned long _last_interaction = 0;
        unsigned long _connection_timeout = 0;
    };
}

#endif //FIRE_CONNECTION_H

//
// Created by Nevd on 27/10/2017.
//

#include <zconf.h>
#include "core/Connection.h"
#include "zmalloc.h"
#include "utils/util.h"

using namespace FIRE;
Connection::Connection(int client_fd, void* owner)
    : _fd(client_fd), _owner(owner)
{
    _readbuf = sdsempty();
}


Connection::~Connection() {
    sdsfree(_readbuf);

    for(auto it : _reply_list) {
        zfree(const_cast<char*>(it.data()));
    }

    _reply_list.clear();

    if(_tcp_io_watcher) {
        delete _tcp_io_watcher;
        _tcp_io_watcher = nullptr;
    }

    if(_timer_watcher) {
        delete _timer_watcher;
        _timer_watcher = nullptr;
    }
}

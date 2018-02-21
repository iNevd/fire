//
// Created by Nevd on 27/10/2017.
//

#include <zconf.h>
#include "core/Connection.h"
#include "zmalloc.h"
#include "utils/util.h"
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
}

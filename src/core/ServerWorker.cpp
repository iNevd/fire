//
// ServerWorker
// Created by Nevd on 15/02/2018.
//

#include <zconf.h>
#include <iostream>
#include "utils/network.h"
#include "utils/time.h"
#include "utils/log.h"
#include "utils/util.h"
#include "ServerWorker.h"
#include "common/include.h"

using namespace FIRE;

ServerWorker::ServerWorker()
    : Server(false)
{
    _connections.reserve(MAX_CLIENT_BLOCK);
}

bool ServerWorker::enqueue(int& priv_data) {
    return _msg_queue.enqueue(priv_data);
}

bool ServerWorker::dequeue(int& priv_data) {
    return _msg_queue.try_dequeue(priv_data);
}

void ServerWorker::thread_run() {
    LOG_TRACE("Run ServerWorker! (main)");
    if(_t != nullptr) {
        this->stop();
        _t->join();
        delete _t;
        _t = nullptr;
    }

    _t = new std::thread(&ServerWorker::run, this);
}

void ServerWorker::join() {
    LOG_TRACE("Join ServerWorker! (main)");
    if(_t != nullptr) {
        _t->join();
    }
}

void ServerWorker::stop() {
    LOG_TRACE("Stop ServerWorker! (thread)");
    // stop notify & timer
    Server::stop();

    // clean connections
    for(auto it : _connections) {
        _eventLoop.delete_watcher(it->_timer_watcher);
        _eventLoop.delete_watcher(it->_tcp_io_watcher);
        delete it;
    }
    _connections.clear();

    if(_t != nullptr) {
        delete _t;
        _t = nullptr;
    }
}

void ServerWorker::notice_cb(int fd, int event) {
    UNUSED(event);
    LOG_TRACE("notice callback (thread)");
    caf::atom_value msg;
    if (read(fd, &msg, sizeof(caf::atom_value)) != sizeof(caf::atom_value)) {
        LOG_WARN_DETAIL("read msg failed, fd:{}, msg:{} (thread)", fd, static_cast<uint64_t >(msg));
        return;
    }

    // process
    switch(msg) {
        case caf::atom("Quit"):
            this->stop();
            break;
        case caf::atom("NewConnect"):
        {
            int fd = 0;
            dequeue(fd);
            notice_cb_new_connection(fd);
        }
            break;
        default:
            LOG_WARN_DETAIL("unknown msg, fd:{}, msg:{} (thread)", fd, static_cast<uint64_t >(msg));
            break;
    }
}

void ServerWorker::notice_cb_new_connection(int fd) {
    LOG_TRACE("Process new client connection, fd:{}", fd);
    if(fd <= 0) {
        LOG_WARN_DETAIL("new connection, fd=0");
        return;
    }
    FIRE::set_sock_noblock(fd);
    FIRE::set_sock_nodelay(fd);
    auto c = new Connection(fd, this);
    FIRE::get_sock_ip_port(fd, c->_ip, c->_port);
    c->_last_interaction = FIRE::mnow();
    c->_begin_interaction = c->_last_interaction;

    /* create io event and timeout for the connection */
    c->_tcp_io_watcher =  new IOWatcher(c->_fd, IOWatcher::READ,
            [](int fd, int event, void* private_data) {
                UNUSED(fd);
                auto c = reinterpret_cast<Connection*>(private_data);
                auto worker = reinterpret_cast<decltype(this)>(c->_owner);
                worker->conn_io_cb(c, event);
            }
            , c);
    _eventLoop.start_watcher(c->_tcp_io_watcher);

    c->_timer_watcher = new TimerWatcher(
            [](void *priv_data) {
                auto c = reinterpret_cast<Connection*>(priv_data);
                auto worker = reinterpret_cast<decltype(this)>(c->_owner);
                worker->conn_timer_cb(c);
            }
            ,c
    );
    auto timer_watcher = dynamic_cast<TimerWatcher*>(c->_timer_watcher);
    timer_watcher->set_repeat(true);

    _eventLoop.start_watcher(c->_timer_watcher, 100 * 1000);

    if(static_cast<unsigned int>(fd) > _connections.size()) {
        _connections.resize(_connections.size() * 2);
    }
    _connections[fd] = c;
}

void ServerWorker::conn_io_cb_read(Connection *c) {
    LOG_TRACE("Process client fd:{} read", c->_fd);
    c->_readbuf = sdsMakeRoomFor(c->_readbuf, 128);
    auto nread = FIRE::sock_read(c->_fd, c->_readbuf, 128);

    if(nread == FAILED) {
        LOG_WARN_DETAIL("Process client fd:{} read failed! errno: {}", c->_fd, errno);
        return;
    }

    c->_last_interaction = FIRE::mnow();
    // TODO test
    std::cout << "read: " << c->_readbuf << std::endl;

    return;
//    nread = Network::sock_read(fd, c->_readbuf+qblen, readlen);
//    if (nread == NET_ERROR) {
//        //log_debug("sock_read_data: return error, close connection");
//        //close_conn(c);
//        return;
//    } else if (nread > 0) {
//        sdsIncrLen(c->_readbuf, nread);
//    }
    c->_last_interaction = FIRE::mnow();

//    int ret = process_read_query(c);
//    if (ret == WORKER_ERROR) {
//        //log_debug("process_read_query: return error, close connection");
//        close_conn(c);
//        return;
//    } else if (ret == WORKER_CONNECTION_REMOVED) {
//        return;
//    }


}

void ServerWorker::conn_io_cb_write(Connection *c) {
    LOG_TRACE("Process client fd:{} write", c->_fd);
    auto& reply_list = c->_reply_list;
    while(!reply_list.empty()) {
        auto reply = reply_list.front();
        auto nwrite = FIRE::sock_write(c->_fd, reply.data() + c->_cur_resp_pos, reply.size() - c->_cur_resp_pos);
        if(nwrite == FAILED) {
            LOG_WARN_DETAIL("Process client fd:{} write failed! errno: {}", c->_fd, errno);
            return;
        }else if(nwrite == 0) {
            LOG_WARN_DETAIL("Process client fd:{} write failed! write nothing. errno: {}", c->_fd, errno);
            return;
        }else if((nwrite + c->_cur_resp_pos) == reply.size()) {
            reply_list.pop_front();
            --c->_reply_list_size;
            c->_cur_resp_pos = 0;
            //
        }else {
            c->_cur_resp_pos += nwrite;
        }

    }
    c->_last_interaction = FIRE::mnow();
    if(c->_reply_list.empty()) {
        _eventLoop.stop_watcher(c->_tcp_io_watcher, IOWatcher::WRITE);
    }

}

void ServerWorker::conn_timer_cb(Connection *c) {
    LOG_TRACE("Process client fd:{} timer event", c->_fd);

    if(FIRE::mnow() - c->_last_interaction > 1000) {
        close_connection(c);
    }
}

void ServerWorker::conn_io_cb(Connection *c, int event) {
    LOG_TRACE("Process client fd:{} IO event:{}", c->_fd, event);
    if(event & IOWatcher::READ) {
        conn_io_cb_read(c);
    }

    if(event & IOWatcher::WRITE) {
        conn_io_cb_write(c);
    }
}

void ServerWorker::close_connection(Connection *c) {
    LOG_TRACE("Process client fd:{} Close connection", c->_fd);
    _eventLoop.delete_watcher(c->_timer_watcher);
    _eventLoop.delete_watcher(c->_tcp_io_watcher, IOWatcher::WRITE | IOWatcher::READ);
    _connections[c->_fd] = NULL;
    close(c->_fd);
    delete c;
}

void ServerWorker::run() {
    LOG_TRACE("Run ServerWorker! (thread)");
    Server::run();
}

//
// ServerMain
// Created by Nevd on 14/02/2018.
//

#include <zconf.h>
#include "utils/network.h"
#include "utils/log.h"
#include "utils/util.h"
#include "ServerMain.h"

ServerMain::ServerMain()
    : Server(false)
{

}

int ServerMain::init() {
    LOG_TRACE("Init ServerMain");
    // create inner msg notify pipe
    Server::init();

    // listen
    _listen_fd = network::create_tcp_server("127.0.0.1", 8888);
    if(_listen_fd == network::FAILED) {
        LOG_ERROR_DETAIL("listen failed, ip {}, port {}", "127.0.0.1", 8888);
        return FAILED;
    }

    // watch fd
    _tcp_conn_watcher = new IOWatcher(_listen_fd, IOWatcher::READ,
            [](int fd, int event, void* priv_data) {
                auto server = reinterpret_cast<decltype(this)>(priv_data);
                server->tcp_conn_cb(fd, event);
            }
            , this);
    _eventLoop.start_watcher(_tcp_conn_watcher);

    LOG_TRACE("Create ServerWorkers");
    // create worker
    for(unsigned int i=0; i<std::thread::hardware_concurrency(); i++) {
        _workers.emplace_back(new ServerWorker());
    }

    LOG_TRACE("Init&Start ServerWorkers");
    // init & start worker
    for(auto it : _workers) {
        it->init();
        it->thread_run();
    }
    return SUCCESS;
}

void ServerMain::stop() {
    LOG_TRACE("Stop ServerMain");
    Server::stop();

    if(_listen_fd != 0) {
        close(_listen_fd);
        _listen_fd = 0;
    }

    for(auto it : _workers) {
        it->notify(caf::atom("Quit"));
        it->join();
    }
}

void ServerMain::notice_cb(int fd, int event) {
    UNUSED(event);
    LOG_TRACE("notice callback");
    caf::atom_value msg;
    if (read(fd, &msg, sizeof(msg)) != sizeof(msg)) {
        LOG_WARN_DETAIL("read msg failed, fd:{}, msg:{}", fd, static_cast<uint64_t >(msg));
        return;
    }

    // process
    switch(msg) {
        case caf::atom("Quit"):
            this->stop();
            break;
        default:
            LOG_WARN_DETAIL("unknown msg, fd:{}, msg:{}", fd, static_cast<uint64_t >(msg));
            break;
    }
}

void ServerMain::tcp_conn_cb(int fd, int event) {
    UNUSED(event);
    LOG_TRACE("tcp connection callback");
    auto cport = 0, cfd = 0;
    char cip[128] = {};

    cfd = network::tcp_accept(fd, cip, &cport);
    if (cfd == network::FAILED) {
        LOG_WARN_DETAIL("tcp accept failed! error: {}", errno);
        return;
    }

    auto worker = _workers[_next_worker];
    _next_worker = (_next_worker+1)%_workers.size();
    worker->enqueue(cfd);
    if (worker->notify(caf::atom("NewConnect")) != SUCCESS) {
        LOG_WARN_DETAIL("notify worker NewConnect failed!");
        return;
    }
}

ServerMain* ServerMain::_instance = nullptr;
ServerMain &ServerMain::getInstance() {
    if(ServerMain::_instance == nullptr) {
        ServerMain::_instance = new ServerMain();
    }
    return *_instance;
}

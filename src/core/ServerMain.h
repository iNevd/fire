//
// ServerMain
// Created by Nevd on 14/02/2018.
//

#ifndef FIRE_SERVERMAIN_H
#define FIRE_SERVERMAIN_H

#include <vector>
#include "Server.h"
#include "ServerWorker.h"

class ServerMain : public Server{
protected:
    ServerMain();
    ServerMain(ServerMain&) = delete;
public:
    int init() override;
    void stop() override;

    static ServerMain& getInstance();
protected:
    void notice_cb(int fd, int event) override;
    void tcp_conn_cb(int fd, int event);

protected:
    static ServerMain* _instance;
    int _listen_fd = 0;
    IWatcher* _tcp_conn_watcher = nullptr;
    std::vector<ServerWorker*> _workers;
    std::vector<ServerWorker*>::size_type _next_worker = 0;
};


#endif //FIRE_SERVERMAIN_H

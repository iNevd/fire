//
// ServerWorker
// Created by Nevd on 15/02/2018.
//

#ifndef FIRE_SERVERWORKER_H
#define FIRE_SERVERWORKER_H

#include <vector>
#include "readerwriterqueue.h"
#include "Server.h"
#include "Connection.h"

namespace FIRE
{
    const int MAX_CLIENT_BLOCK = 1024;
    //const int MAX_CLIENT = MAX_CLIENT_BLOCK * 128;

    class ServerWorker : public Server{
    public:
        ServerWorker();

        void thread_run();
        void join();

        void stop() override;

        bool enqueue(int& priv_data);
        bool dequeue(int& priv_data);


    protected:
        // run
        inline void run() override;

        // notice callback
        void notice_cb(int fd, int event) override;
        // notice callback_deal
        void notice_cb_new_connection(int fd);

        // tcp
        void conn_io_cb(Connection* c, int event);
        void conn_io_cb_read(Connection *c);
        void conn_io_cb_write(Connection *c);

        // timer
        void conn_timer_cb(Connection* c);

        // close
        void close_connection(Connection* c);

    protected:
        moodycamel::ReaderWriterQueue<int> _msg_queue;

        std::thread* _t = nullptr;

        // connections
        std::vector<Connection*> _connections;
    };

}


#endif //FIRE_SERVERWORKER_H

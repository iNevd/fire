//
// Created by Nevd on 27/10/2017.
//

#include <netinet/in.h>
#include <cstring>
#include <errno.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <sys/fcntl.h>
#include "network.h"
#include "log.h"
#include "common/include.h"
namespace FIRE {

    int create_tcp_server(std::string host, int port)  {
        int s, on;
        struct sockaddr_in sa = {};

        if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            LOG_WARN_DETAIL("creating socket: {}", strerror(errno));
            return FAIL;
        }

        if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
            LOG_WARN_DETAIL("setsockopt SO_REUSEADDR: {}", strerror(errno));
            return FAIL;
        }

        //memset(&sa,0,sizeof(sa));
        sa.sin_family = AF_INET;
        sa.sin_port = htons(port);
        sa.sin_addr.s_addr = htonl(INADDR_ANY);
        if (host.length() <= 0 && inet_aton(host.c_str(), &sa.sin_addr) == 0) {
            LOG_WARN_DETAIL("invalid bind address");
            close(s);
            return FAIL;
        }

        if (bind(s,(struct sockaddr*)&sa,sizeof(sa)) == -1) {
            LOG_WARN_DETAIL("bind: {}", strerror(errno));
            close(s);
            return FAIL;
        }

        /* Use a backlog of 512 entries. We pass 511 to the listen() call because
         * the kernel does: backlogsize = roundup_pow_of_two(backlogsize + 1);
         * which will thus give us a backlog of 512 entries */
        if (listen(s, SOMAXCONN - 1) == -1) {
            LOG_WARN_DETAIL("listen: {}", strerror(errno));
            //log_warning("listen: %s", strerror(errno));
            close(s);
            return FAIL;
        }
        return s;
    }

    int tcp_accept(int s, char *ip, int *port) {
        int fd = 0;
        struct sockaddr_in sa;
        socklen_t salen = sizeof(sa);
        if ((fd = generic_accept(s,(struct sockaddr*)&sa,&salen)) == FAIL) {
            LOG_WARN_DETAIL("generic_accept failed");
            return FAIL;
        }
        if (ip) strcpy(ip,inet_ntoa(sa.sin_addr));
        if (port) *port = ntohs(sa.sin_port);
        return fd;
    }

    int set_sock_nodelay(int fd) {
        int yes = 1;
        if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) {
            LOG_WARN_DETAIL("setsockopt TCP_NODELAY: {}", strerror(errno));
            return FAIL;
        }
        return FIRE::SUCCESS;
    }

    int set_sock_noblock(int fd)
    {
        int flags = 0;

        /* Set the socket nonblocking.
         * Note that fcntl(2) for F_GETFL and F_SETFL can't be
         * interrupted by a signal. */
        if ((flags = fcntl(fd, F_GETFL)) == -1) {
            LOG_WARN_DETAIL("fcntl(F_GETFL): {}", strerror(errno));
            return FAIL;
        }
        if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
            LOG_WARN_DETAIL("fcntl(F_SETFL, O_NONBLOCK): {}", strerror(errno));
            return FAIL;
        }
        return FIRE::SUCCESS;
    }

    int get_sock_ip_port(int fd, std::string &ip, int &port) {
        struct sockaddr_in sa;
        socklen_t salen = sizeof(sa);

        if (getpeername(fd,(struct sockaddr*)&sa,&salen) == -1) {
            port = 0;
            ip = "?";
            LOG_WARN_DETAIL("getpeername failed, fd: {}", fd);
            return FAIL;
        }
        ip = inet_ntoa(sa.sin_addr);
        port = ntohs(sa.sin_port);
        return FIRE::SUCCESS;
    }


    int generic_accept(int s, struct sockaddr *sa, socklen_t *len) {
        int fd;
        while(1) {
            fd = accept(s,sa,len);
            if (fd == -1) {
                if (errno == EINTR)
                    continue;
                else {
                    LOG_WARN_DETAIL("accept: {}", strerror(errno));
                    return FAIL;
                }
            }
            break;
        }
        return fd;
    }


    int sock_read(int fd, char *buf, size_t len) {
        int nread = read(fd, buf, len);
        if (nread == -1) {
            if (errno == EAGAIN)
                nread = 0;
            else {
                LOG_WARN_DETAIL("read: {}", strerror(errno));
                return FAIL;
            }
        } else if (nread == 0) {
            LOG_WARN_DETAIL("connection closed");
            return FAIL;
        }
        return nread;
    }

    int sock_write(int fd, const char *buf, size_t len) {
        int nwritten = write(fd, buf, len);
        if (nwritten == -1) {
            if (errno == EAGAIN)
                nwritten = 0;
            else {
                LOG_WARN_DETAIL("write: {}", strerror(errno));
                return FAIL;
            }
        }
        return nwritten;
    }
}


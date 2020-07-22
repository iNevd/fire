//
// Created by Nevd on 27/10/2017.
//

#ifndef FIRE_NETWORK_H
#define FIRE_NETWORK_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace FIRE {
    /*
     * @param host string 字符串形式的IP地址
     * @param port unsigned short 监听的端口
     */
    int create_tcp_server(std::string host, unsigned short port);
    int tcp_accept(int s, char *ip, int *port);
    int generic_accept(int s, struct sockaddr *sa, socklen_t *len);
    int set_sock_noblock(int fd);
    int set_sock_nodelay(int fd);
    int get_sock_ip_port(int fd, std::string &ip, int &port);
    int sock_read(int fd, char *buf, size_t len);
    int sock_write(int fd, const char *buf, size_t len);
}

#endif //FIRE_NETWORK_H

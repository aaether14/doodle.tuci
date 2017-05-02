#include <arpa/inet.h>
#include <unistd.h>
#include "tcpstream.h"

TCPStream::TCPStream(int socket_descriptor, struct sockaddr_in* address) : m_socket_descriptor(socket_descriptor)
{
        char ip[50];
        inet_ntop(PF_INET, (struct in_addr*)&(address->sin_addr.s_addr), ip, sizeof(ip) - 1);
        m_ip = ip;
        m_port = ntohs(address->sin_port);
}

TCPStream::~TCPStream()
{
        if (m_socket_descriptor)
                close(m_socket_descriptor);
}

ssize_t TCPStream::Send(const char *buffer, std::size_t length)
{
        return write(m_socket_descriptor, buffer, length);
}

ssize_t TCPStream::Receive(char *buffer, std::size_t length, int timeout)
{
        if (timeout <= 0)
                return read(m_socket_descriptor, buffer, length);
        if (wait_on_read_event(timeout))
                return read(m_socket_descriptor, buffer, length);
        return connection_timed_out;
}

bool TCPStream::wait_on_read_event(int timeout)
{
        struct timeval tv;
        tv.tv_sec = timeout;
        tv.tv_usec = 0;
        fd_set read_set;
        FD_ZERO(&read_set);
        FD_SET(m_socket_descriptor, &read_set);
        if (select(m_socket_descriptor + 1, &read_set, nullptr, nullptr, &tv) > 0)
                return true;
        return false;
}

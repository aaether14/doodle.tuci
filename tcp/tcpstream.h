#pragma once
#include <string>
class TCPStream
{
        int m_socket_descriptor;
        int m_port;
        std::string m_ip;
      public:
        friend class TCPServer;
        ~TCPStream();
        ssize_t Send(const char *buffer, std::size_t length);
        ssize_t Receive(char *buffer, std::size_t length, int timeout = 0);
        inline int GetPort() const { return m_port; }
        inline const std::string& GetIp() const { return m_ip; }
        enum
        {
                connection_closed    =  0,
                connection_reset     = -1,
                connection_timed_out = -2
        };
        TCPStream(TCPStream&& other) 
        {
                m_socket_descriptor = other.m_socket_descriptor;
                m_port = other.m_port;
                m_ip = std::move(other.m_ip);
                other.m_socket_descriptor = 0; 
        }
        TCPStream& operator=(TCPStream&& other) 
        {
                m_socket_descriptor = other.m_socket_descriptor;
                m_port = other.m_port;
                m_ip = std::move(other.m_ip);
                other.m_socket_descriptor = 0;
                return *this;
        }
      private:
        bool wait_on_read_event(int timeout);
        TCPStream() = delete;
        TCPStream(const TCPStream &other) = delete;
        TCPStream& operator=(const TCPStream& other) = delete;
        TCPStream(int socket_descriptor, struct sockaddr_in *address);
};
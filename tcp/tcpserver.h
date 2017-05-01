#pragma once
#include <string>
#include <memory>
#include "tcpstream.h"
class TCPServer
{
        int m_socket_descriptor;
        int m_port;
        bool m_listening;
        std::string m_address;
public:
        TCPServer(int port, const std::string& address = "");
        ~TCPServer();
        std::unique_ptr<TCPStream> Accept();
        void Start();
private:
        TCPServer() = delete;
        TCPServer(const TCPServer& other) = delete;
        TCPServer& operator=(const TCPServer& other) = delete;
        TCPServer(TCPServer&& other) = delete;
        TCPServer& operator=(TCPServer&& other) = delete;
};

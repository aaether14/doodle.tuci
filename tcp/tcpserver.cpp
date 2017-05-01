#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include "tcpserver.h"


TCPServer::TCPServer(int port, const std::string &address) : m_socket_descriptor(0),
                                                             m_port(port),
                                                             m_listening(false),
                                                             m_address(address)
{
}

TCPServer::~TCPServer()
{
        if (m_socket_descriptor)
                close(m_socket_descriptor);
}

void TCPServer::Start()
{
        if (m_listening) return;

        m_socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
        if (m_socket_descriptor == -1)
                throw std::runtime_error("socket() failed!\n");
        struct sockaddr_in address;

        memset(&address, 0, sizeof(address));
        address.sin_family = PF_INET;
        address.sin_port = htons(m_port);
        if (m_address.size() > 0)
                inet_pton(PF_INET, m_address.c_str(), &address.sin_addr);
        else
                address.sin_addr.s_addr = INADDR_ANY;

        int result = bind(m_socket_descriptor, (struct sockaddr*)&address, sizeof(address));
        if (result == -1)
                throw std::runtime_error("bind() failed!\n");

        result = listen(m_socket_descriptor, 5);
        if (result == -1)
                throw std::runtime_error("listen() failed!\n");
        m_listening = true;
}

std::unique_ptr<TCPStream> TCPServer::Accept()
{
        if (!m_listening) throw std::runtime_error("Server is not listening!\n");

        struct sockaddr_in address;
        socklen_t length = sizeof(address);
        memset(&address, 0, sizeof(address));
        int incoming_socket_descriptor = ::accept(m_socket_descriptor, (struct sockaddr*)&address, &length);
        if (incoming_socket_descriptor == -1)
                throw std::runtime_error("accept() failed!\n");
        return std::make_unique<TCPStream>(TCPStream(incoming_socket_descriptor, &address));
}

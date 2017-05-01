#include <iostream>
#include <string>
#include "thread/thread.h"
#include "work_queue/work_queue.h"
#include "tcp/tcpacceptor.h"


class ConnectionHandler : public Thread
{
        SPMCQueue<TCPStream>& m_queue;
public:
        ConnectionHandler(SPMCQueue<TCPStream>& queue) : m_queue(queue) {}
        void* Run()
        {
                while (true)
                {
                        auto stream = m_queue.Pop();
                        char input[256];
                        std::size_t input_length;
                        while ((input_length = stream.receive(input, sizeof(input) - 1)) > 0)
                                stream.send(input, input_length);
                }
                return nullptr;
        }
};

int main(int argc, char **argv)
{

        int workers = 3;
        int port = 4001;
        std::string ip = "127.0.0.1";


        SPMCQueue<TCPStream> m_queue;
        for (int it = 0; it < workers; ++it)
        {
                ConnectionHandler* handler = new ConnectionHandler(m_queue);
                if (!handler)
                {
                        std::cerr << "Could not create connection handler!\n";
                        return 1;
                }
                handler->Start();
        }


        TCPServer m_server(port, ip);
        try 
        {
                m_server.Start();
        }
        catch (const std::exception&ex)
        {
                std::cerr << ex.what();
                return 1;
        }
        while (1)
        {
                try
                {
                        auto connection = std::move(m_server.Accept());
                        m_queue.Push(std::move(connection));
                }
                catch (const std::exception& ex)
                {
                        std::cerr << ex.what();
                        continue;
                }
        }
        return 0;
}

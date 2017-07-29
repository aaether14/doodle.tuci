#include "logger/log.h"
#include "tcp/tcpserver.h"
#include "connection/connection_handler.h"



int main(int argc, char **argv)
{

        if (argc != 4) 
        {
                ERROR("Usage: <threadpool-size> <port> <ip-address>!\n");
                return 1;
        }
        std::uint16_t thread_pool_size = std::stoi(argv[1]);
        std::uint16_t server_port = std::stoi(argv[2]);
        std::string server_ip = argv[3];

        State m_state;
        SPMCQueue<UUID> m_job_queue;

        std::vector<std::unique_ptr<Thread>> thread_pool(thread_pool_size);
        try
        {
                for (auto it = 0u; it < thread_pool_size; ++it)
                {
                        thread_pool[it] = std::make_unique<ConnectionHandler>(m_state, m_job_queue);
                        thread_pool[it]->Start();
                }
        }
        catch(const std::exception& ex)
        {
                ERROR(ex.what());
                return 1;
        }
        TCPServer m_server(server_port, server_ip);
        try 
        {
                m_server.Start();
        }
        catch (const std::exception& ex)
        {
                ERROR(ex.what());
                return 1;
        }
        while (1)
        {
                try
                {
                        UUID new_connection_id;
                        auto new_connection = m_server.Accept();
                        m_state.InsertConnection(new_connection_id, std::move(new_connection));
                        m_job_queue.Push(new_connection_id);
                }
                catch (const std::exception& ex)
                {
                        ERROR(ex.what());
                        continue;
                }
        }
        return 0;

}

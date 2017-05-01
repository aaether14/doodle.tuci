#include <iostream>
#include <map>
#include <uuid/uuid.h>
#include "tcp/tcpserver.h"
#include "thread/thread.h"
#include "containers/spmcqueue.h"
#include "containers/rwlock.h"


class UUID
{
        uuid_t m_uuid;
public:
        ~UUID() = default;
        UUID()
        {
                uuid_generate(m_uuid);
        }
        UUID(const UUID& other) 
        { 
                uuid_copy(m_uuid, other.m_uuid); 
        }
        UUID& operator=(const UUID& other)
        {
                if (this != &other)
                        uuid_copy(m_uuid, other.m_uuid); 
                return *this;
        }
        UUID(UUID&& other)
        {
              uuid_copy(m_uuid, other.m_uuid);   
        }
        UUID& operator=(UUID&& other)
        {
                uuid_copy(m_uuid, other.m_uuid); 
                return *this;
        }
        bool operator<(const UUID &other) const
        { 
                return uuid_compare(m_uuid, other.m_uuid) < 0; 
        }
        bool operator==(const UUID &other) const
        { 
                return uuid_compare(m_uuid, other.m_uuid) == 0; 
        }
        friend std::ostream& operator<<(std::ostream& out, const UUID& other)
        {
                for (auto it = 0; it < 16; ++it) out << int(other.m_uuid[it]) << " ";
                return out;
        }
};



class ConnectionHandler : public Thread
{
        RWLock& m_lock;
        SPMCQueue<UUID>& m_queue;
        std::map<UUID, std::unique_ptr<TCPStream>>& m_conntection_table;
public:
        ConnectionHandler(RWLock& lock,
                SPMCQueue<UUID>& queue, 
                std::map<UUID, std::unique_ptr<TCPStream>>& table) : 
                m_lock(lock), 
                m_queue(queue),
                m_conntection_table(table)
                {

                }
        void* Run()
        {
                
                while (true)
                {
                        const auto connection_id = m_queue.Pop();
                        m_lock.ReadLock();
                        auto& connection = m_conntection_table.at(connection_id);
                        m_lock.ReadUnlock();
                        char input[256];
                        std::size_t input_length;
                        while ((input_length = connection->receive(input, sizeof(input) - 1)) > 0)
                        {
                                m_lock.ReadLock();
                                for (auto& it : m_conntection_table)
                                {
                                        if (it.first == connection_id) continue;
                                        auto& current_connection = it.second;
                                        current_connection->send(input, input_length);
                                }
                                m_lock.ReadUnlock();
                        }
                        m_lock.WriteLock();
                        m_conntection_table.erase(connection_id);
                        m_lock.WriteUnlock();
                        
                }
                return nullptr;
        }
};

int main(int argc, char **argv)
{

        int number_of_workers = 3;
        int server_port = 4002;
        std::string server_ip = "127.0.0.1";

        RWLock m_rwlock;
        SPMCQueue<UUID> m_connection_queue;
        std::map<UUID, std::unique_ptr<TCPStream>> m_connection_table;
        std::unique_ptr<Thread> workers[number_of_workers];

        try
        {
                for (int it = 0; it < number_of_workers; ++it)
                {
                        workers[it] = std::make_unique<ConnectionHandler>(m_rwlock, m_connection_queue, m_connection_table);
                        workers[it]->Start();
                }
        }
        catch(const std::exception& ex)
        {
                std::cerr << ex.what();
                return 1;
        }


        TCPServer m_server(server_port, server_ip);
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
                        UUID connection_id;
                        auto new_connection = m_server.Accept();
                        m_rwlock.WriteLock();
                        m_connection_table.emplace(connection_id, std::move(new_connection));
                        m_rwlock.WriteUnlock();
                        m_connection_queue.Push(connection_id);
                }
                catch (const std::exception& ex)
                {
                        std::cerr << ex.what();
                        continue;
                }
        }
        return 0;
}

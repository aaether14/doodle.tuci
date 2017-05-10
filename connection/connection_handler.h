#pragma once
#include <unordered_map>
#include "../tcp/tcpstream.h"
#include "../thread/thread.h"
#include "../containers/spmcqueue.h"
#include "../containers/rwlock.h"
#include "../containers/uuid_helper.h"
#include "../logger/log.h"
class ConnectionHandler : public Thread
{
        RWLock& m_rw_lock;
        SPMCQueue<UUID>& m_job_queue;
        std::unordered_map<UUID, std::unique_ptr<TCPStream>>& m_conntection_table;
public:
        ConnectionHandler(RWLock& rw_lock,
                SPMCQueue<UUID>& job_queue, 
                std::unordered_map<UUID, std::unique_ptr<TCPStream>>& connection_table) : 
                m_rw_lock(rw_lock), 
                m_job_queue(job_queue),
                m_conntection_table(connection_table)
                {
                }
        void* Run()
        {   
                while (true)
                {
                        const auto current_connection_id = m_job_queue.Pop();
                        DEBUG(current_connection_id, " has connected!\n");
                        m_rw_lock.ReadLock();
                        auto& current_connection = m_conntection_table.at(current_connection_id);
                        m_rw_lock.ReadUnlock();
                        char input[256];
                        std::size_t input_length;
                        while ((input_length = current_connection->Receive(input, sizeof(input) - 1)) > 0)
                        {
                                std::vector<TCPStream*> other_connections;
                                m_rw_lock.ReadLock();
                                for (auto& it : m_conntection_table)
                                {
                                        if (it.first == current_connection_id) continue;
                                        other_connections.push_back(it.second.get());
                                }
                                m_rw_lock.ReadUnlock();
                                for (auto it : other_connections)
                                        it->Send(input, input_length);
                                DEBUG(current_connection_id, ": ", std::string(input, input_length));
                        }
                        m_rw_lock.WriteLock();
                        m_conntection_table.erase(current_connection_id);
                        m_rw_lock.WriteUnlock();
                        DEBUG(current_connection_id, " has disconnected!\n");
                }
                return nullptr;
        }
};
#pragma once
#include "../thread/thread.h"
#include "../containers/spmcqueue.h"
#include "state.h"
class ConnectionHandler : public Thread
{
        State& m_state;
        SPMCQueue<UUID>& m_job_queue;
        void ProcessRequest(const UUID& connection_id, TCPStream* connection, char* message, std::size_t message_length);
public:
        ConnectionHandler(State& state, SPMCQueue<UUID>& job_queue);
        ~ConnectionHandler() = default;
        void* Run();
};
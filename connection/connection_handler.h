#pragma once
#include "../thread/thread.h"
#include "../containers/spmcqueue.h"
#include "state.h"
#include "request.h"
#include "file_manager.h"
class ConnectionHandler : public Thread
{
        State& m_state;
        SPMCQueue<UUID>& m_job_queue;
        bool ProcessRequest(const UUID& connection_id, TCPStream* connection, char* header);
        void OnConnect(TCPStream* connection);
        void OnSelectWorkspace(char* message, std::size_t message_length);
        void OnNewShape(char* message, std::size_t message_length, const UUID& connection_id);
public:
        ConnectionHandler(State& state, SPMCQueue<UUID>& job_queue);
        ~ConnectionHandler() = default;
        void* Run();
};
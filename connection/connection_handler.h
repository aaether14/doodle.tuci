#pragma once
#include "../thread/thread.h"
#include "../containers/spmcqueue.h"
#include "state.h"
class ConnectionHandler : public Thread
{
        State& m_state;
        SPMCQueue<UUID>& m_job_queue;
public:
        ConnectionHandler(State& state, SPMCQueue<UUID>& job_queue);
        ~ConnectionHandler() = default;
        void* Run();
};
#define PROVIDE_LOGGER_DEFINITION
#include "../logger/log.h"
#undef PROVIDE_LOGGER_DEFINITION
#include "connection_handler.h"

ConnectionHandler::ConnectionHandler(State& state, SPMCQueue<UUID>& job_queue) : 
                m_state(state),
                m_job_queue(job_queue)
                                                                                
{

}

void* ConnectionHandler::Run()
 {   
                while (true)
                {
                        const auto current_connection_id = m_job_queue.Pop();
                        DEBUG(current_connection_id, " has connected!\n");
                        auto current_connection = m_state.GetConnection(current_connection_id);
                        char input[512];
                        ssize_t input_length;
                        while ((input_length = current_connection->Receive(input, sizeof(input) - 1)) > 0)
                        {
                                auto connections = m_state.GetAllConnections();
                                for (auto& it : *connections.get())
                                {
                                        if (it.first == current_connection_id) continue;
                                        it.second->Send(input, input_length);
                                }
                                DEBUG(current_connection_id, ": ", std::string(input, input_length));
                        }
                        DEBUG(current_connection_id, " has disconnected!\n");
                        m_state.RemoveConnection(current_connection_id);
                }
                return nullptr;
 }
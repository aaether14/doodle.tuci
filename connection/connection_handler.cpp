#define PROVIDE_LOGGER_DEFINITION
#include "../logger/log.h"
#undef PROVIDE_LOGGER_DEFINITION
#include "connection_handler.h"

bool ConnectionHandler::ProcessRequest(const UUID& connection_id, TCPStream* connection, char* header)
{

        /**
        auto connections = m_state.GetAllConnections();
        for (auto& it : *connections.get())
        {
                if (it.first == connection_id) continue;
                it.second->Send(message, message_length);
        }
        **/
        Request request = *reinterpret_cast<Request*>(header);
        std::size_t packet_size = *reinterpret_cast<uint16_t*>(header + 2);

        char packet[512];
        if (packet_size)
        {
                ssize_t result = connection->Receive(packet, packet_size);
                if (result <= 0) return false;
        }

        switch (request)
        {
                case Request::E_ON_CONNECT:
                        OnConnect(connection); break;
                case Request::E_SELECT_WORKSPACE:
                        OnSelectWorkspace(packet, packet_size); break;
                case Request::E_NEW_SHAPE:
                        OnNewShape(packet, packet_size, connection_id); break;
                default:
                        DEBUG("Unknown request id: ", static_cast<uint16_t>(request), "!\n", packet_size, "\n");
        }
        return true;
}

void ConnectionHandler::OnConnect(TCPStream* connection)
{
        auto files_on_server = FileManager::GetFilesInDirectory("./data");
        auto data_to_send = *files_on_server.get();
        *reinterpret_cast<Request*>(&data_to_send[0]) = Request::E_HELLO;
        connection->Send(&data_to_send[0], data_to_send.size());
}

void ConnectionHandler::OnSelectWorkspace(char* message, std::size_t message_length)
{
        DEBUG("./data/" + std::string(message, message_length) + " is the new workspace!\n");
        m_state.SetWorkspace("./data/" + std::string(message, message_length));
}

void ConnectionHandler::OnNewShape(char* message, std::size_t message_length, const UUID& connection_id)
{
        DEBUG(std::string(message, message_length));
}

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
                char input_header[4];
                ssize_t input_header_length;
                while ((input_header_length = current_connection->Receive(input_header, 4)) > 0)
                        if (!ProcessRequest(current_connection_id, current_connection, input_header))
                                break;
                DEBUG(current_connection_id, " has disconnected!\n");
                m_state.RemoveConnection(current_connection_id);
        }
        return nullptr;
 }
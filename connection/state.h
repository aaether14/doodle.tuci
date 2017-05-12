#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "../containers/rwlock.h"
#include "../containers/uuid_helper.h"
#include "../tcp/tcpstream.h"
class State
{
        mutable RWLock m_rw_lock;
        std::unordered_map<UUID, std::unique_ptr<TCPStream>> m_connection_table;
        std::string m_workspace;
        State(const State& other) = delete;
        State& operator=(const State& other) = delete;
        State(State&& other) = delete;
        State& operator=(State&& other) = delete;
public:
        State() = default;
        ~State() = default;
        void InsertConnection(const UUID& uuid, std::unique_ptr<TCPStream> connection);
        void RemoveConnection(const UUID& uuid);
        TCPStream* GetConnection(const UUID& uuid);
        std::unique_ptr<std::vector<std::pair<UUID, TCPStream*>>> GetAllConnections();
        void SetWorkspace(const std::string& workspace);
        const std::string GetWorkspace() const;
};
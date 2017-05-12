#include "state.h"

void State::InsertConnection(const UUID& uuid, std::unique_ptr<TCPStream> connection)
{
        m_rw_lock.WriteLock();
        m_connection_table.emplace(uuid, std::move(connection));
        m_rw_lock.WriteUnlock();
}

void State::RemoveConnection(const UUID& uuid)
{
        m_rw_lock.WriteLock();
        m_connection_table.erase(uuid);
        m_rw_lock.WriteUnlock();
}

TCPStream* State::GetConnection(const UUID& uuid)
{
        m_rw_lock.ReadLock();
        auto result = m_connection_table.at(uuid).get();
        m_rw_lock.ReadUnlock();
        return result;
}

std::unique_ptr<std::vector<std::pair<UUID, TCPStream*>>> State::GetAllConnections()
{
        std::vector<std::pair<UUID, TCPStream*>> result;
        m_rw_lock.ReadLock();
        for (auto& it : m_connection_table) result.push_back({it.first, it.second.get()});
        m_rw_lock.ReadUnlock();
        return std::make_unique<std::vector<std::pair<UUID, TCPStream*>>>(std::move(result));
}

void State::SetWorkspace(const std::string& workspace)
{
        m_rw_lock.WriteLock();
        m_workspace = workspace;
        m_rw_lock.WriteUnlock();
}

const std::string State::GetWorkspace() const
{
        m_rw_lock.ReadLock();
        const std::string result = m_workspace;
        m_rw_lock.ReadUnlock();
        return result;
}
#pragma once
#include <uuid/uuid.h>
#include <ostream>
class UUID
{
        uuid_t m_uuid;
public:
        ~UUID() = default;
        inline UUID()
        {
                uuid_generate(m_uuid);
        }
        inline UUID(const UUID& other) 
        { 
                uuid_copy(m_uuid, other.m_uuid); 
        }
        inline UUID& operator=(const UUID& other)
        {
                if (this != &other)
                        uuid_copy(m_uuid, other.m_uuid); 
                return *this;
        }
        inline UUID(UUID&& other)
        {
              uuid_copy(m_uuid, other.m_uuid);   
        }
        inline UUID& operator=(UUID&& other)
        {
                uuid_copy(m_uuid, other.m_uuid); 
                return *this;
        }
        inline bool operator<(const UUID &other) const
        { 
                return uuid_compare(m_uuid, other.m_uuid) < 0; 
        }
        inline bool operator==(const UUID &other) const
        { 
                return uuid_compare(m_uuid, other.m_uuid) == 0; 
        }
        inline friend std::ostream& operator<<(std::ostream& out, const UUID& other)
        {
                for (auto it = 0; it < 16; ++it) out << int(other.m_uuid[it]) << " ";
                return out;
        }
};
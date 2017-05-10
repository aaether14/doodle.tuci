#pragma once
#include <uuid/uuid.h>
#include <ostream>
class UUID
{
        uuid_t m_uuid;
        friend struct std::hash<UUID>;
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
                out << std::hex;
                for (auto it = 0u; it < 16u; ++it) out << uint32_t(other.m_uuid[it]);
                out << std::dec;
                return out;
        }
};
namespace std
{
        template<> struct hash<UUID>
            {
                using argument_type = UUID;
                using result_type = std::size_t;
                template <typename SIZE_T>
                inline typename std::enable_if<sizeof(SIZE_T) == 8, SIZE_T>::type fnv_hash(const argument_type& uuid) const
                {
                        const SIZE_T fnv_prime = 1099511628211;
                        SIZE_T fnv_offset = 0xcbf29ce484222325;
                        for (auto it = 0u; it < 16u; ++it)
                        {
                                fnv_offset = fnv_offset ^ uuid.m_uuid[it];
                                fnv_offset *= fnv_prime;
                        }
                        return fnv_offset;
                }
                template <typename SIZE_T>
                inline typename std::enable_if<sizeof(SIZE_T) == 4, SIZE_T>::type fnv_hash(const argument_type& uuid) const
                {
                        const SIZE_T fnv_prime = 16777619;
                        SIZE_T fnv_offset = 0x811c9dc5;
                        for (auto it = 0u; it < 16u; ++it)
                        {
                                fnv_offset = fnv_offset ^ uuid.m_uuid[it];
                                fnv_offset *= fnv_prime;
                        }
                        return fnv_offset;
                }
                result_type operator()(const argument_type& uuid) const
                {
                    return fnv_hash<result_type>(uuid);
                }
            };
}
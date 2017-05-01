#pragma once
#include <pthread.h>
class RWLock
{
        pthread_mutex_t m_shared_mutex;
        pthread_cond_t m_read_condition;
        pthread_cond_t m_write_condition;
        int m_active_readers;
        int m_active_writers;
        int m_waiting_writers;
        RWLock(const RWLock& other) = delete;
        RWLock& operator=(const RWLock& other) = delete;
        RWLock(RWLock&& other) = delete;
        RWLock& operator=(RWLock&& other) = delete;
public:
        RWLock() : m_active_readers(0),
                   m_active_writers(0),
                   m_waiting_writers(0)
        {
                pthread_mutex_init(&m_shared_mutex, nullptr);
                pthread_cond_init(&m_read_condition, nullptr);
                pthread_cond_init(&m_write_condition, nullptr);                
        }
        ~RWLock()
        {
                pthread_mutex_destroy(&m_shared_mutex);
                pthread_cond_destroy(&m_read_condition);
                pthread_cond_destroy(&m_write_condition);                
        }
        void ReadLock()
        {
                pthread_mutex_lock(&m_shared_mutex);
                while (m_waiting_writers > 0)
                        pthread_cond_wait(&m_read_condition, &m_shared_mutex);
                ++m_active_readers;
                pthread_mutex_unlock(&m_shared_mutex);
        }
        void ReadUnlock()
        {
                pthread_mutex_lock(&m_shared_mutex);
                --m_active_readers;
                pthread_cond_signal(&m_write_condition);
                pthread_mutex_unlock(&m_shared_mutex);
        }
        void WriteLock()
        {
                pthread_mutex_lock(&m_shared_mutex);
                ++m_waiting_writers;
                while (m_active_readers > 0 || m_active_writers > 0)
                        pthread_cond_wait(&m_write_condition, &m_shared_mutex);
                ++m_active_writers;
                pthread_mutex_unlock(&m_shared_mutex);
        }
        void WriteUnlock()
        {
                pthread_mutex_lock(&m_shared_mutex);
                --m_waiting_writers;
                --m_active_writers;
                if (m_waiting_writers > 0)
                        pthread_cond_signal(&m_write_condition);
                else
                        pthread_cond_broadcast(&m_read_condition);
                pthread_mutex_unlock(&m_shared_mutex);
        }
};
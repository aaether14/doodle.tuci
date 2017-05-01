#pragma once
#include <pthread.h>
#include <queue>
template <typename T>
class SPMCQueue
{
        std::queue<T>   m_queue;
        pthread_mutex_t m_mutex;
        pthread_cond_t  m_condition_variable;
        SPMCQueue(const SPMCQueue& other) = delete;
        SPMCQueue& operator=(const SPMCQueue& other) = delete;
        SPMCQueue(SPMCQueue&& other) = delete;
        SPMCQueue& operator=(SPMCQueue&& other) = delete;
public:
        SPMCQueue()
        {
                pthread_mutex_init(&m_mutex, nullptr);
                pthread_cond_init(&m_condition_variable, nullptr);
        }
        ~SPMCQueue()
        {
                pthread_mutex_destroy(&m_mutex);
                pthread_cond_destroy(&m_condition_variable);
        }
        template <typename Item>
        void Push(Item&& item)
        {
                pthread_mutex_lock(&m_mutex);
                m_queue.push(std::forward<Item>(item));
                pthread_cond_signal(&m_condition_variable);
                pthread_mutex_unlock(&m_mutex);
        }
        T Pop()
        {
                pthread_mutex_lock(&m_mutex);
                while (m_queue.empty())
                        pthread_cond_wait(&m_condition_variable, &m_mutex);
                auto item = std::move(m_queue.front());
                m_queue.pop();
                pthread_mutex_unlock(&m_mutex);
                return item;
        }
        int Size()
        {
                pthread_mutex_lock(&m_mutex);
                int size = m_queue.size();
                pthread_mutex_unlock(&m_mutex);
                return size;
        }
};
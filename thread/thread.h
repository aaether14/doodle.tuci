#pragma once
#include <pthread.h>
class Thread
{
        inline static void* run_thread(void* arg)
        {
                return ((Thread*)arg)->Run();
        } 
        virtual void* Run() = 0;
        void Clean();
        pthread_t m_thread_id;
        bool m_is_running;
        bool m_is_detached;
        Thread(const Thread& other) = default;
public:
        Thread();
        virtual ~Thread();
        Thread& operator=(const Thread& other) = delete;
        Thread(Thread&& other);
        Thread& operator=(Thread&& other);
        void Start();
        void Join();
        void Detach();
        inline pthread_t Self() const { return m_thread_id; }
        inline bool IsRunning() const { return m_is_running; }
        inline bool IsDetached() const { return m_is_detached; }
};
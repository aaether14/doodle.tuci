#include "thread.h"
#include <stdexcept>
#include <string>


void Thread::Clean()
{
        if (m_is_running && !m_is_detached)
                pthread_detach(m_thread_id);
        if (m_is_running)
                pthread_cancel(m_thread_id);
}

Thread::Thread() : m_thread_id(0),
                   m_is_running(false),
                   m_is_detached(false) {}
Thread::~Thread()
{
        Clean();
}

Thread::Thread(Thread&& other) : Thread(other)
{
        other.m_is_running = false;
}

Thread& Thread::operator=(Thread&& other)
{
        Clean();
        m_thread_id = other.m_thread_id;
        m_is_running = other.m_is_running;
        m_is_detached = other.m_is_detached;
        other.m_is_running = false;
        return *this;
}

void Thread::Start()
{
        int error_code = pthread_create(&m_thread_id, nullptr, Thread::run_thread, this);
        if (error_code) 
                throw std::runtime_error("Thread creation resulted in error: " + std::to_string(error_code) + "!\n");
        m_is_running = true;
}

void Thread::Join()
{
        if (!m_is_running) return;
        int error_code = pthread_join(m_thread_id, nullptr);
        if (error_code) 
                throw std::runtime_error("Joining thread with id: " + std::to_string(m_thread_id) + " resulted in error " + std::to_string(error_code) + "!\n");
        m_is_detached = true;
}

void Thread::Detach()
{
        if (!m_is_running || m_is_detached) return;
        int error_code = pthread_detach(m_thread_id);
        if (error_code) 
                throw std::runtime_error("Detaching thread with id: " + std::to_string(m_thread_id) + " resulted in error " + std::to_string(error_code) + "!\n");
        m_is_detached = true;
}

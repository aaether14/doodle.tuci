#include "log_policies.h"
#include <fstream>

LogToFilePolicy::LogToFilePolicy() : m_stream(new std::ofstream())
{       
}

LogToFilePolicy::~LogToFilePolicy()
{
        if (m_stream)
                close();
}

void LogToFilePolicy::open(const std::string& name)
{
        m_stream->open(name, std::ios_base::binary | std::ios_base::out);
        if (!m_stream->is_open())
                throw std::runtime_error("LOGGER: Could not open an outout stream: " + name + "!\n");
}

void LogToFilePolicy::close()
{
        if (m_stream)
                m_stream->close();
}

void LogToFilePolicy::write(const std::string& msg)
{
        (*m_stream) << msg << std::endl;
}
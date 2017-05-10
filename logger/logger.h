#pragma once
#include <pthread.h>
#include <sstream>
#include <iomanip>
#include <ctime>
enum class SeverityType
{
        E_DEBUG,
        E_WARNING,
        E_ERROR
};
template <typename LogPolicy>
class Logger
{
        LogPolicy m_log_policy;
        pthread_mutex_t m_mutex;
        std::size_t m_log_line;
        std::stringstream m_stream;
        std::string GetTime();
        std::string GetHeader();
        template <typename Head, typename... Rest>
        void print_implementation(Head param, Rest... rest_of_params);
        void print_implementation();
        Logger(const Logger& other) = delete;
        Logger& operator=(const Logger& other) = delete;
        Logger(Logger&& other) = delete;
        Logger& operator=(Logger&& other) = delete;
public:
        Logger(const std::string& name);
        ~Logger();
        template <SeverityType severity, typename... Args>
        void print(Args... args);
};
template <typename LogPolicy>
std::string Logger<LogPolicy>::GetTime()
{
        time_t raw_time;
        time(&raw_time);
        std::string time_as_string = ctime(&raw_time);
        return time_as_string.substr(0, time_as_string.size() - 1);
}
template <typename LogPolicy>
std::string Logger<LogPolicy>::GetHeader()
{
        std::stringstream header;
        header.fill('0');
        header << std::setw(7) << m_log_line++ << " <" << GetTime() << "> ";
        return header.str();
}
template <typename LogPolicy>
Logger<LogPolicy>::Logger(const std::string& name) : m_log_line(0u)
{
        if (pthread_mutex_init(&m_mutex, nullptr) != 0)
                throw std::runtime_error("LOGGER: Could not create logger instance: " + name + "!\n");
        m_log_policy.open(name);
}
template <typename LogPolicy>
Logger<LogPolicy>::~Logger()
{
        pthread_mutex_destroy(&m_mutex);
}
template <typename LogPolicy>
template <SeverityType severity, typename... Args>
void Logger<LogPolicy>::print(Args... args)
{
        pthread_mutex_lock(&m_mutex);
        switch (severity)
        {
                case SeverityType::E_DEBUG:
                        m_stream << "<DEBUG>: ";
                        break;
                case SeverityType::E_WARNING:
                        m_stream << "<WARNING>: ";
                        break;
                case SeverityType::E_ERROR:
                        m_stream << "<ERROR>: ";
                        break;
        };
        print_implementation(args...);
        pthread_mutex_unlock(&m_mutex);
}
template <typename LogPolicy>
void Logger<LogPolicy>::print_implementation()
{
        m_log_policy.write(GetHeader() + m_stream.str());
        m_stream.str("");
}
template <typename LogPolicy>
template <typename Head, typename... Rest>
void Logger<LogPolicy>::print_implementation(Head param, Rest... rest_of_params)
{
        m_stream << param;
        print_implementation(rest_of_params...);
}
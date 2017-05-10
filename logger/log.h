#pragma once
#include "logger.h"
#include "log_policies.h"
#ifdef PROVIDE_LOGGER_DEFINITION
Logger<LogToFilePolicy>& GetFileLogger()
{
        static Logger<LogToFilePolicy> m_instance("server.log");
        return m_instance;
}
Logger<LogToCerrPolicy>& GetCerrLogger()
{
        static Logger<LogToCerrPolicy> m_instance("");
        return m_instance;
}
#else
Logger<LogToFilePolicy>& GetFileLogger();
Logger<LogToCerrPolicy>& GetCerrLogger();
#endif
#define DEBUG(...) { GetFileLogger().print<SeverityType::E_DEBUG>(__VA_ARGS__); GetCerrLogger().print<SeverityType::E_DEBUG>(__VA_ARGS__); }
#define WARNING(...) { GetFileLogger().print<SeverityType::E_WARNING>(__VA_ARGS__); GetCerrLogger().print<SeverityType::E_WARNING>(__VA_ARGS__); }
#define ERROR(...) { GetFileLogger().print<SeverityType::E_ERROR>(__VA_ARGS__); GetCerrLogger().print<SeverityType::E_ERROR>(__VA_ARGS__); }
#pragma once
#include "logger.h"
#include "log_policies.h"
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
#define DEBUG(...) { GetFileLogger().print<SeverityType::E_DEBUG>(__VA_ARGS__); GetCerrLogger().print<SeverityType::E_DEBUG>(__VA_ARGS__); }
#define WARNING(...) { GetFileLogger().print<SeverityType::E_WARNING>(__VA_ARGS__); GetCerrLogger().print<SeverityType::E_WARNING>(__VA_ARGS__); }
#define ERROR(...) { GetFileLogger().print<SeverityType::E_ERROR>(__VA_ARGS__); GetCerrLogger().print<SeverityType::E_ERROR>(__VA_ARGS__); }
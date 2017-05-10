#pragma once
#include <memory>
#include <fstream>
#include <iostream>
#include "log_policy_interface.h"
class LogToFilePolicy : public LogPolicyInterface
{
        std::unique_ptr<std::ofstream> m_stream;
public:
        LogToFilePolicy();
        ~LogToFilePolicy();
        void close();
        void open(const std::string& name);
        void write(const std::string& msg);
};

class LogToCerrPolicy : public LogPolicyInterface
{
public:
        LogToCerrPolicy() = default;
        ~LogToCerrPolicy() = default;
        void close() {}
        void open(const std::string& name) {}
        void write(const std::string& msg) { std::cerr << msg << std::endl; }
};
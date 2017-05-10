#pragma once
#include <string>
class LogPolicyInterface
{
public:
        virtual void close() = 0;
        virtual void open(const std::string& name) = 0;
        virtual void write(const std::string& msg) = 0;
};
#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include "../containers/rwlock.h"
class FileManager
{
        RWLock m_rw_lock;
        std::ifstream m_in_stream;
        std::ofstream m_out_stream;
public:
        FileManager() = default;
        ~FileManager() = default;
        static std::unique_ptr<std::vector<char>> GetFilesInDirectory(const std::string& directory_name);
        void AppendData(const std::string& filename, const char* data, std::size_t length, bool reopen = false);
        void BeginReading(const std::string& filename);
        void EndReading();
        void ReadData(char* data, std::size_t length);
};
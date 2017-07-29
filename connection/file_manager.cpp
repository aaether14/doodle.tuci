#include "file_manager.h"
#include <cstring>
#include <dirent.h>

std::unique_ptr<std::vector<char>> FileManager::GetFilesInDirectory(const std::string& directory_name)
{
        std::vector<char> result; result.resize(4);
        DIR *directory;
        struct dirent *directory_entry;
        
        directory = opendir(directory_name.c_str());
        if (directory)
        {
                while ((directory_entry = readdir(directory)) != nullptr)
                        if (directory_entry->d_type == DT_REG)
                                result.insert(result.end(), directory_entry->d_name, directory_entry->d_name + std::strlen(directory_entry->d_name) + 1);
                closedir(directory);
        }
        else
                throw std::runtime_error("Could not open directory!\n");
        return std::make_unique<std::vector<char>>(std::move(result));

}

void FileManager::AppendData(const std::string& filename, const char* data, std::size_t length, bool reopen)
{
        m_rw_lock.WriteLock();
        if (reopen && m_out_stream.is_open())
                m_out_stream.close();
        if (!m_out_stream.is_open())
                m_out_stream.open(filename, std::ios_base::binary | std::ios_base::out | std::ios_base::app);
        m_out_stream.write(data, length);
        m_rw_lock.WriteUnlock();
}

void FileManager::BeginReading(const std::string& filename)
{
        m_rw_lock.ReadLock();
        if (m_in_stream.is_open())
                m_in_stream.close();
        m_in_stream.open(filename, std::ios_base::binary | std::ios_base::in);
}

void FileManager::EndReading()
{
        if (m_in_stream.is_open())
                m_in_stream.close();
        m_rw_lock.ReadUnlock();
}

void FileManager::ReadData(char* data, std::size_t length)
{
        m_in_stream.read(data, length);
}
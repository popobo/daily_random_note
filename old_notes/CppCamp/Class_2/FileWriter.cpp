#include "FileWriter.hpp"
#include <iostream>
#include "NormalStrategy.hpp"
#include <cstdio>
#include <cstdlib>
#include <cassert>

class FileWriter::FileWriterImpl {
public:
    FileWriterImpl(const std::string& filename) {
        std::cout << "FileWriterImpl" << std::endl;
        m_fp = fopen(filename.c_str(), "w+");
        fseek(m_fp, 0, SEEK_SET);
        std::cout << this << std::endl;
        //assert(m_fp != nullptr);
    }
    
    ~FileWriterImpl() {
        fclose(m_fp);
    }

    FileWriterImpl(const FileWriterImpl& other) = delete;
    FileWriterImpl& operator=(const FileWriterImpl& other) = delete;
    FileWriterImpl(const FileWriterImpl&& other) = delete;
    FileWriterImpl& operator=(const FileWriterImpl&& other) = delete;

    int writeAtBegin(void *data, int length)
    {
        fseek(m_fp, 0, SEEK_SET);
        fwrite(data, sizeof(char), length, m_fp);
        return length;
    }

    int writeAt(int pos, void *data, int length)
    {
        fseek(m_fp, pos, SEEK_SET);
        fwrite(data, sizeof(char), length, m_fp);
        return length;
    }

    int writeAtEnd(void *data, int length)
    {
        fseek(m_fp, 0, SEEK_END);
        fwrite(data, sizeof(char), length, m_fp);
        fwrite("\n", sizeof(char), 1, m_fp);
        return length;
    }
    
private:
    FILE *m_fp = nullptr;
};

int FileWriter::writeAtBegin(void *data, int length)
{
    // std::cout << "write " << length << " at the begin." << std::endl;
    // m_strategy->onWrite(0, data, length);
    m_pImpl->writeAtBegin(data, length);
    return length;
}

int FileWriter::writeAt(int pos, void *data, int length)
{
    std::cout << "write" << length << " at pos " << pos << std::endl;
    m_strategy->onWrite(pos, data, length);
    m_pImpl->writeAt(pos, data, length);
    OnWrite(pos, length);
    return length;
}

int FileWriter::writeAtEnd(void *data, int length)
{
    std::cout << "write " << length << " at the end." << std::endl; 
    m_pImpl->writeAtEnd(data, length);
    return length;
}

FileWriter::FileWriter(const std::string &filename, StrategyType strategy): m_pImpl{new FileWriterImpl{filename}}
{
    switch (strategy)
    {
    case StrategyType::NORMAL:
        m_strategy = new NormalStrategy{};
        break;
    
    default:
        break;
    }
}

FileWriter::~FileWriter() noexcept
{
    delete m_strategy;
    delete m_pImpl;
}

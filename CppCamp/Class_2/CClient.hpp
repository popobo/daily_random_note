#pragma once

#include "CFileWriter.hpp"
#include "IWriter.hpp"

class CClient final {
public:
    CClient(IWriter *writer);
    CClient() = delete;
    ~CClient() noexcept;
    CClient(const CClient& other) = delete;
    CClient& operator=(const CClient& other) = delete;
    CClient(const CClient&& other) = delete;
    CClient& operator=(const CClient&& other) = delete;
    
    void process();

private:
    IWriter *m_writer;
};

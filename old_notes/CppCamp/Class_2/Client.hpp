#pragma once

#include "FileWriter.hpp"
#include "IWriter.hpp"

class Client final {
public:
    Client(IWriter *writer);
    Client() = delete;
    ~Client() noexcept;
    Client(const Client& other) = delete;
    Client& operator=(const Client& other) = delete;
    Client(const Client&& other) = delete;
    Client& operator=(const Client&& other) = delete;
    
    void process();

private:
    IWriter *m_writer;
};

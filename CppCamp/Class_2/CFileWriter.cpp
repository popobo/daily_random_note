#include "CFileWriter.hpp"
#include <iostream>
#include "NormalStrategy.hpp"

CFileWriter::CFileWriter(const std::string &filename)
{
}

int CFileWriter::writeAtBegin(void *data, int length)
{
    std::cout << "write " << length << " at the begin." << std::endl; 
    m_strategy->onWrite(0, data, length);
    return 0;
}

int CFileWriter::writeAt(int pos, void *data, int length)
{
    std::cout << "write" << length << " at pos " << pos << std::endl;
    return 0;
}

int CFileWriter::writeAtEnd(void *data, int length)
{
    std::cout << "write " << length << " at the end." << std::endl; 
    return 0;
}

CFileWriter::CFileWriter(const std::string &filename, StrategyType strategy)
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

CFileWriter::~CFileWriter() noexcept
{
    delete m_strategy;
}

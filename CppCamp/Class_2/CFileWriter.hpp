#pragma once

#include <string>
#include "IWriter.hpp"
#include "IWriteStrategy.hpp"

class CFileWriter: public IWriter {
public:
    CFileWriter(const std::string& filename);
    CFileWriter(const std::string& filename, StrategyType strategy);
    ~CFileWriter() noexcept;
    
    int writeAtBegin(void *data, int length) override;
    int writeAt(int pos, void *data, int length) override;
    int writeAtEnd(void *data, int length) override;

private:
    IWriteStrategy *m_strategy;
};  
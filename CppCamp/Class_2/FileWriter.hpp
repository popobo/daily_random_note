#pragma once

#include <string>
#include "IWriter.hpp"
#include "IWriteStrategy.hpp"
#include "Observable.hpp"

class FileWriter: public IWriter, public Observable {
public:
    FileWriter(const std::string& filename);
    FileWriter(const std::string& filename, StrategyType strategy);
    ~FileWriter() noexcept;
    
    int writeAtBegin(void *data, int length) override;
    int writeAt(int pos, void *data, int length) override;
    int writeAtEnd(void *data, int length) override;

private:
    struct FileWriterImpl;
    FileWriterImpl * m_pImpl;
    IWriteStrategy *m_strategy;
};
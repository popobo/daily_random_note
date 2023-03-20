#pragma once

class IWriter {
public:
    IWriter() = default;
    virtual ~IWriter() noexcept = default;
    IWriter(const IWriter& other) = delete;
    IWriter& operator=(const IWriter& other) = delete;
    IWriter(const IWriter&& other) = delete;
    IWriter& operator=(const IWriter&& other) = delete;

    virtual int writeAtBegin(void *data, int length) = 0;
    virtual int writeAt(int pos, void *data, int length) = 0;
    virtual int writeAtEnd(void *data, int length) = 0;
};
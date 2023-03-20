#include "CFileWriter.hpp"

class CClient final {
public:
    CClient(CFileWriter *writer);
    CClient() = delete;
    ~CClient() noexcept;
    
    void process();

private:
    CFileWriter *m_writer;
};

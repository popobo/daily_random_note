#include "CClient.hpp"
#include <cstring>

CClient::CClient(CFileWriter *writer): m_writer{writer} {}

CClient::~CClient() noexcept {}

void CClient::process()
{
    char buf[64];

    memset(buf, 'a', 64);
    m_writer->writeAtBegin(buf, 64);

    memset(buf, 'b', 32);
    m_writer->writeAt(16, buf, 32);

    memset(buf, 'c', 64);
    m_writer->writeAtEnd(buf, 64);
}

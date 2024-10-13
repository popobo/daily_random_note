#include "Client.hpp"
#include <cstring>
#include <array>

static const int BUF_SIZE = 64;

Client::Client(IWriter *writer): m_writer{writer} {}

Client::~Client() noexcept {}

void Client::process()
{
    std::array<char, BUF_SIZE> buf{};

    buf.fill('A');
    m_writer->writeAtBegin(buf.data(), BUF_SIZE);

    buf.fill('B');
    m_writer->writeAt(BUF_SIZE / 4, buf.data(), BUF_SIZE / 2);

    buf.fill('C');
    m_writer->writeAtEnd(buf.data(), BUF_SIZE);
}

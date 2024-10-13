#include <cinttypes>
#include <string>
#include <utility>

#define PRINT_FUNC_ADDR std::cout << __func__ << " at " << this << std::endl

class A {
    int m_a{1};
    int m_b{2};
    int m_c{3};
    uint8_t m_d{4};

public:
    A() = default;
    ~A() = default;
    A(const A& other) = default;
    A& operator=(const A& other) = default;

};

class CDataBuffer {
    CDataBuffer(const std::string& name, A *data, uint32_t length);
    CDataBuffer(const CDataBuffer& other);
    CDataBuffer& operator=(const CDataBuffer& other);
    ~CDataBuffer() noexcept;

private:
    std::string m_dataName;
    uint32_t m_dataLen;
    uint32_t m_bufSize;
    A *m_pfoo;
};

CDataBuffer::CDataBuffer(const std::string& name, A *data, uint32_t length):
m_dataName(name), m_dataLen(length), m_pfoo(data !=nullptr ? new A{*data} : new A{})  {}

CDataBuffer::CDataBuffer(const CDataBuffer& other) {
    m_dataName = other.m_dataName;
    m_dataLen = other.m_dataLen;
    m_pfoo = other.m_pfoo !=nullptr ? new A{*other.m_pfoo} : nullptr;
}

CDataBuffer& CDataBuffer::operator=(const CDataBuffer& other) {
    if (this == &other) {
        return *this;
    }
    
    // if (other.m_pfoo) {
    //     if (m_pfoo) {
    //         *m_pfoo = *other.m_pfoo;
    //     } else {
    //         m_pfoo = new A{*other.m_pfoo};
    //     }
    // } else {
    //     delete m_pfoo;
    //     m_pfoo = nullptr;
    // }
    CDataBuffer temp{other};
    std::swap(m_pfoo, temp.m_pfoo); // copy swap的好处

    m_dataName = other.m_dataName;
    m_bufSize = other.m_bufSize;
    m_dataLen = other.m_dataLen;

    return *this;
}

CDataBuffer::~CDataBuffer() noexcept {
    delete m_pfoo;
}

int main() {
    return 0;
}
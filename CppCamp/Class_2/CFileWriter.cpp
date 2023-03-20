#include "CFileWriter.hpp"
#include <iostream>

CFileWriter::CFileWriter(const std::string &filename)
{
}

CFileWriter::~CFileWriter()
{
}

int CFileWriter::writeAtBegin(void *data, int length)
{
    std::cout << "write " << length << " at the begin." << std::endl; 
    return 0;
}

int CFileWriter::writeAt(int pos, void *data, int length)
{
    std::cout << "write" << length << "at pos " << pos << std::endl;
    return 0;
}

int CFileWriter::writeAtEnd(void *data, int length)
{
    std::cout << "write " << length << " at the end." << std::endl; 
    return 0;
}

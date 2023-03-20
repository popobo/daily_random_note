#include <string>

class CFileWriter {
public:
    CFileWriter(const std::string& filename);
    CFileWriter() = delete;
    ~CFileWriter();
    
    int writeAtBegin(void *data, int length);
    int writeAt(int pos, void *data, int length);
    int writeAtEnd(void *data, int length);

};
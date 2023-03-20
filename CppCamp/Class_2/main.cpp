#include "CClient.hpp"

int main() {
    IWriter *writer = new CFileWriter{"file writer", StrategyType::NORMAL};

    CClient client{writer};
    client.process();

    delete writer;
    
    return 0;
}
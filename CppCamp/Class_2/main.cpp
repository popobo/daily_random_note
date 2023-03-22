#include "Client.hpp"

int main() {
    FileWriter writer{"filetest", StrategyType::NORMAL};

    auto handler{
        writer.GetWriteEvent()+=[](int pos, int length) {
            std::cout << "ssssssssssssssssss" << std::endl;
        }
    };

    Client client{&writer};
    client.process();

    writer.GetWriteEvent()-=handler;
    
    return 0;
}
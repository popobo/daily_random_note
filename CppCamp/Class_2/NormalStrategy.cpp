#include "NormalStrategy.hpp"
#include <iostream>

void NormalStrategy::onWrite(int pos, void *data, int length) {
    std::cout << "NormalStrategy:" << std::endl;
}
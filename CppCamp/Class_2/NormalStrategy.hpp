#pragma once

#include "IWriteStrategy.hpp"

class NormalStrategy: public IWriteStrategy {
public:
    void onWrite(int pos, void *data, int length) override;
};
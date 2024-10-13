#pragma once

enum class StrategyType: int {
    NORMAL,
    LAZY
};

class IWriteStrategy {
public:
    IWriteStrategy() = default;
    virtual ~IWriteStrategy() noexcept = default;
    IWriteStrategy(const IWriteStrategy& other) = delete;
    IWriteStrategy& operator=(const IWriteStrategy& other) = delete;
    IWriteStrategy(const IWriteStrategy&& other) = delete;
    IWriteStrategy& operator=(const IWriteStrategy&& other) = delete;

    virtual void onWrite(int pos, void *data, int length) = 0;
};
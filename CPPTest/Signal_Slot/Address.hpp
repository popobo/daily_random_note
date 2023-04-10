#pragma once

#include <cstddef>
#include <functional>

class Object;

struct Address
{
    void* object;
    void* function;

    Address() = default;

    Address(void* object, void* function): object(object), function(function) {
    }

    template<typename Observer, typename... Args>
    Address(Object* observer, void (Observer::* handler)(Args...)) {
        object = observer;
        memcpy(&function, &handler, sizeof(void*));
    }

    template<typename Observer, typename... Args>
    Address(Object* observer, void (Observer::* handler)(Args...) const)  {
        object = observer;
        memcpy(&function, &handler, sizeof(void*));
    }

    template<typename Notifier, typename Event>
    Address(Object* notifier, Event Notifier::* event) {
        object = notifier;
        memcpy(&function, &event, sizeof(void *));
    }

    template<typename... Args>
    explicit Address(void(*handler)(Args...)) {
        object = nullptr;
        memcpy(&function, &handler, sizeof(void*));
    }

    bool operator<(const Address& other) const {
        return other.object < object;
    }

    bool operator==(const Address& other) const {
        return other.object == object && other.function == function;
    }
};

struct AddressHasher {
    std::size_t operator()(const Address& addr) const {
        auto hash1 = std::hash<void*>{}(addr.object);
        auto hash2 = std::hash<void*>{}(addr.function);
        return hash1 ^ (hash2 << 1);
    }
};
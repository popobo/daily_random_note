#pragma once

#include <functional>
#include <map>

using EventToken = size_t;

template<typename... Args>
class Event {
public:
    EventToken operator+=(std::function<void(Args...)> observer) {
        auto n{++m_count};
        // TODO: error handling
        m_observers[n] = observer;
        return n;
    }

    Event& operator-=(EventToken handler) {
        // TODO: error handling
        m_observers.erase(handler);
        return *this;
    }

    void raise(Args... args) {
        for (const auto& observer: m_observers) {
            (observer.second)(args...);
        }
    }

private:
    std::map<EventToken, std::function<void(Args...)>> m_observers;
    size_t m_count = 0;
};
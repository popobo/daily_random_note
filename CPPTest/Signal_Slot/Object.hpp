#pragma once

#include "Event.hpp"

class Object {
    private:
        std::map<Address, std::function<void()>> m_events;
        std::shared_mutex m_shared_mutex;
    public:
        template<typename Notifier, typename Observer, typename... Args, typename... TrueArgs> 
        void Subscribe(Notifier* notifier, Event<Args...> Notifier::*event, void (Observer::*func)(TrueArgs...)) {
            void* buffer = nullptr;
            memcpy(&buffer, &func, sizeof(void*));
            Address address{notifier, buffer};

            std::lock_guard lg{m_shared_mutex};

            if (!m_events.count(address)) {
                (notifier->*event).Attach(static_cast<Observer*>(this), func);
                
                m_events.insert(std::make_pair(address, [=]() {
                    (notifier->*event).Detach(static_cast<Observer*>(this), func);
                }));
            }
        }


        template<typename Notifier, typename Observer, typename... Args, typename... TrueArgs> 
        void Unsubscribe(Notifier* notifier, Event<Args...> Notifier::*event, void (Observer::*func)(TrueArgs...)) {
            void* buffer = nullptr;
            memcpy(&buffer, &func, sizeof(void*));
            Address address{notifier, buffer};

            std::lock_guard lg{m_shared_mutex};

            if (!m_events.count(address)) {
                (notifier->*event).Detach(static_cast<Observer*>(this), func);
                m_events.erase(address);
            }
        }

        virtual ~Object() {
            std::lock_guard lg{m_shared_mutex};
            for (const auto& event: m_events) {
                event.second();
            }
        }
};
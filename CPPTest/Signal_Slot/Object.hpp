#pragma once

#include "Event.hpp"

class Object {
    private:
        std::map<Address, std::function<void()>> m_events;
        std::map<Address, std::function<void()>> m_handlers;
        std::shared_mutex m_shared_mutex;
    public:
        template<typename Notifier, typename Observer, typename... Args, typename... TrueArgs> 
        void Subscribe(Notifier* notifier, Event<Args...> Notifier::*event, void (Observer::*func)(TrueArgs...)) {
            void* buffer = nullptr;
            memcpy(&buffer, &func, sizeof(void*));
            Address observer_address{this, buffer};

            memcpy(&buffer, &event, sizeof(void *));
            Address notifier_address{notifier, buffer};

            std::lock_guard lg{m_shared_mutex};

            if (!m_events.count(notifier_address)) {
                (notifier->*event).Attach(static_cast<Observer*>(this), func);
                
                m_events.insert(std::make_pair(notifier_address, [=] {
                    (notifier->*event).Detach(static_cast<Observer*>(this), func);
                    notifier->m_handlers.erase(observer_address);
                }));
            }

            if (!notifier->m_handlers.count(observer_address)) {
                notifier->m_handlers.insert(std::make_pair(observer_address, [=] {
                    // as event notifier, when I am destroyed, I should tell observer(handler) that do not try to detach me if it is going to be destroyed.
                    m_events.erase(observer_address);
                }));
            }
        }


        template<typename Notifier, typename Observer, typename... Args, typename... TrueArgs> 
        void Unsubscribe(Notifier* notifier, Event<Args...> Notifier::*event, void (Observer::*func)(TrueArgs...)) {
            void* buffer = nullptr;
            memcpy(&buffer, &func, sizeof(void*));
            Address observer_address{this, buffer};

            memccpy(&buffer, &event, sizeof(void *));
            Address notifier_address{notifier, buffer};

            std::lock_guard lg{m_shared_mutex};

            if (m_events.count(notifier_address)) {
                (notifier->*event).Detach(static_cast<Observer*>(this), func);
                m_events.erase(notifier_address);
            }

            if (notifier->m_handlers.count(observer_address)) {
                notifier->m_handlers.erase(observer_address);
            }
        }

        virtual ~Object() {
            std::lock_guard lg{m_shared_mutex};
            for (const auto& event: m_events) {
                event.second();
            }

            for (const auto& handler: m_handlers) {
                handler.second();
            }
        }
};
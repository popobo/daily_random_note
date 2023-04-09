#pragma once

#include "EventHandler.hpp"
#include <map>
#include <mutex>
#include <shared_mutex>

struct Address
{
    void* object;
    void* function;

    bool operator<(const Address& other) const {
        return other.object < object;
    }

    bool operator==(const Address& other) const {
        return other.object == object && other.function == function;
    }
};

template<typename... Args>
class Event {
    using Handler = EventHandlerInterface<Args...>*;

    private:
        std::map<Address, Handler> m_handlers;
        mutable std::shared_mutex m_mutex;

    public:
        template<typename... TrueArgs>
        void Attach(void(*func)(TrueArgs...)) {
            Address address{nullptr, reinterpret_cast<void*>(func)};
            
            std::lock_guard lg{m_mutex};

            if (!m_handlers.count(address)) {
                Handler handler = new EventHandler<void, std::tuple<TrueArgs...>, Args...>{func};
                m_handlers.insert(std::make_pair(address, handler));
            }
        }

        template<typename T, typename... TrueArgs>
        void Attach(T* object, void (T::*func)(TrueArgs...)) {
            void* buffer = nullptr;
            memcpy(&buffer, &func, sizeof(void *));
            Address address{object, buffer};

            std::lock_guard lg{m_mutex};

            if (!m_handlers.count(address)) {
                Handler handler = new EventHandler<T, std::tuple<TrueArgs...>, Args...>{object, func};
                m_handlers.insert(std::make_pair(address, handler));
            }
        }

        template<typename T, typename... TrueArgs>
        void Attach(T* object, void (T::*func)(TrueArgs...) const) {
            void* buffer = nullptr;
            memcpy(&buffer, &func, sizeof(void *));
            Address address{object, buffer};

            std::lock_guard lg{m_mutex};
            
            if (!m_handlers.count(address)) {
                Handler handler = new EventHandler<T, std::tuple<TrueArgs...>, Args...>{object, func};
                m_handlers.insert(std::make_pair(address, handler));
            }
        }

    private:
        template<typename T, typename U, typename... TrueArgs>
        void ImitationFunctionHelper(T* object, U&& t, void(std::decay_t<U>::*func)(TrueArgs...)) {
            void* buffer = nullptr;
            memcpy(&buffer, &func, sizeof(void *));
            Address address{object, buffer};
            std::function<void(TrueArgs...)> f = std::forward<U>(t);
            Handler handler = new EventHandler<void, std::tuple<TrueArgs...>, Args...>{std::move(t)};

            std::lock_guard lg{m_mutex};

            m_handlers.insert(std::make_pair(address, handler));
        }

        template<typename T, typename U, typename... TrueArgs>
        void ImitationFunctionHelper(T* object, U&& t, void(std::decay_t<U>::*func)(TrueArgs...) const) {
            void* buffer = nullptr;
            memcpy(&buffer, &func, sizeof(void *));
            Address address{object, buffer};
            std::function<void(TrueArgs...)> f = std::forward<U>(t);
            Handler handler = new EventHandler<void, std::tuple<TrueArgs...>, Args...>{std::move(t)};

            std::lock_guard lg{m_mutex};

            m_handlers.insert(std::make_pair(address, handler));
        }

    public:
        template<typename T, typename Lambda>
        void Attach(T* object, Lambda&& lambda) {
            ImitationFunctionHelper(object, std::forward<Lambda>(lambda), &Lambda::operator());
        }

    public:
        template<typename... TrueArgs>
        void Detach(void(*func)(TrueArgs...)) {
            Address address{nullptr, reinterpret_cast<void*>(func)};
            
            std::lock_guard lg{m_mutex};

            if (m_handlers.count(address)) {
                delete m_handlers[address];
                m_handlers.erase(address);
            }
        }

        template<typename T, typename... TrueArgs>
        void Detach(T* object, void (T::*func)(TrueArgs...)) {
            void* buffer = nullptr;
            memcpy(&buffer, &func, sizeof(void *));
            Address address{object, buffer};

            std::lock_guard lg{m_mutex};

            if (m_handlers.count(address)) {
                delete m_handlers[address];
                m_handlers.erase(address);
            }
        }

        template<typename T, typename... TrueArgs>
        void Detach(T* object, void (T::*func)(TrueArgs...) const) {
            void* buffer = nullptr;
            memcpy(&buffer, &func, sizeof(void *));
            Address address{object, buffer};

            std::lock_guard lg{m_mutex};
            
            if (m_handlers.count(address)) {
                delete m_handlers[address];
                m_handlers.erase(address);
            }
        }

        void Clear() {
            std::lock_guard lg{m_mutex};
            for (auto& handler: m_handlers) {
                delete handler.second;
            }
            m_handlers.clear();
        }

        void operator()(const Args& ...args) {
            std::shared_lock lg{m_mutex};
            for (const auto& handler: m_handlers) {
                (*handler.second)(args...);
            }
        }

        void Fired(const Args& ...args) {
            std::shared_lock lg{m_mutex};
            for (const auto& handler: m_handlers) {
                (*handler.second)(args...);
            }
        }

        ~Event() {
            Clear();
        }
};
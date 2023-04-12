#pragma once

#include "EventHandler.hpp"
#include "Address.hpp"
#include <unordered_map>
#include <mutex>
#include <shared_mutex>

template<typename... Args>
class Event {
    using Handler = EventHandlerInterface<Args...>*;
    private:

        std::unordered_map<Address, Handler, AddressHasher> m_handlers_map;
        mutable std::shared_mutex m_shared_mutex;
    
    private:
        void AddHandler(const Address& address, Handler handler) {
            std::lock_guard lg{m_shared_mutex};
            
            if (!m_handlers_map.count(address)) {
                m_handlers_map[address] = handler;
            }
        }

        void RemoveHandler(const Address& address) {
            std::lock_guard lg{m_shared_mutex};
            
            if (m_handlers_map.count(address)) {
                delete m_handlers_map[address];
                m_handlers_map.erase(address);
            }
        }

    private:
        template<typename Observer, typename Lambda, typename... FuncArgs>
        Address ImitationFunctionHelper(Observer* object, Lambda&& t, void(std::decay_t<Lambda>::*func)(FuncArgs...)) 
        {
            std::function<void(FuncArgs...)> f = std::forward<Lambda>(t);
            Address address{object, func};
            AddHandler(
                address,
                new EventHandler<void, std::tuple<FuncArgs...>, Args...>{std::move(t)}
            );
            return address;
        }

        template<typename Observer, typename Lambda, typename... FuncArgs>
        Address ImitationFunctionHelper(Observer* object, Lambda&& t, void(std::decay_t<Lambda>::*func)(FuncArgs...) const) 
        {
            std::function<void(FuncArgs...)> f = std::forward<Lambda>(t);
            Address address{object, func};
            AddHandler(
                address,
                new EventHandler<void, std::tuple<FuncArgs...>, Args...>{std::move(t)}
            );
            return address;
        }

    public:
        void emit(const Args&... args) {
            std::shared_lock sl{m_shared_mutex};
            for (auto& handler: m_handlers_map) {
                (*handler.second)(args...);
            }
        }


        ~Event() {
            std::lock_guard lg{m_shared_mutex};
            for (auto& handler: m_handlers_map) {
                delete handler.second;
            }
        }

    public:
        template<typename Notifier, typename Observer, typename... EmitArgs, typename... HandlerArgs>
        friend void connect(
            Notifier* notifier,
            Event<EmitArgs...> Notifier::* event,
            Observer* observer,
            void (Observer::*handler)(HandlerArgs...)
        );

        template<typename Notifier, typename Observer, typename... EmitArgs, typename... HandlerArgs>
        friend void disconnect(
            Notifier* notifier,
            Event<EmitArgs...> Notifier::* event,
            Observer* observer,
            void (Observer::*handler)(HandlerArgs...)
        );

        template<typename Notifier, typename... EmitArgs, typename... HandlerArgs>
        friend void connect(
            Notifier* notifier,
            Event<EmitArgs...> Notifier::* event,
            void (*handler)(HandlerArgs...)
        );

        template<typename Notifier, typename... EmitArgs, typename... HandlerArgs>
        friend void disconnect(
            Notifier* notifier, 
            Event<EmitArgs...> Notifier::* event, 
            void (*handler)(HandlerArgs...)
        );

        template<typename Notifier, typename Observer, typename Lambda, typename... EmitArgs, typename... HandlerArgs>
        friend void connect(
            Notifier* notifier,
            Event<EmitArgs...> Notifier::* event,
            Observer* observer,
            Lambda&& lambda
        );
};
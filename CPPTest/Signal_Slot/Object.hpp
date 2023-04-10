#pragma once

#include "Event.hpp"
#include <map>
#include <unordered_map>

class Object {
    
    private:
        using Connection = std::map<void*, std::function<void()>>;
        std::unordered_map<Address, Connection, AddressHasher> m_notifiers;
        std::shared_mutex m_notifiers_shared_mutex;
        
        std::unordered_map<Address, std::function<void()>, AddressHasher> m_observers;
        std::shared_mutex m_observers_shared_mutex;

    private:
        void AddNotifier(
            const Address& address,
            void* handler_func_ptr,
            const std::function<void()>& func_at_destroyed) 
        {
            std::lock_guard lg{m_notifiers_shared_mutex};
            // 自动添加新元素的行为是std::unordered_map的标准行为
            m_notifiers[address].insert(std::make_pair(handler_func_ptr, func_at_destroyed));
        }

        void RemoveNotifier(const Address& address, void* handler_func_ptr) {
            std::lock_guard lg{m_notifiers_shared_mutex};
            // erase 函数删除一个不存在的元素时，程序不会抛出异常或崩溃，但也不会删除任何元素
            // 如果address不存在，会使m_notifiers增加一个空的std::map<>对象
            // m_notifiers[address].erase(handler_func_ptr);
            if (m_notifiers.count(address)) {
                m_notifiers[address].erase(handler_func_ptr);
            }
        }

        void AddObserver(const Address& address, const std::function<void()>& func_at_destroyed) 
        {
            std::lock_guard lg{m_observers_shared_mutex};
            m_observers[address] = func_at_destroyed;
        }

        void RemoveObserver(const Address& address)
        {
            std::lock_guard lg{m_observers_shared_mutex};
            if (m_observers.count(address)) {
                m_observers.erase(address);
            }
        }
        
        bool ContainNotifier(
            const Address& address,
            void* handler_func_ptr) 
        {
            std::shared_lock sl{m_notifiers_shared_mutex};
            if (m_notifiers.count(address))
                return m_notifiers[address].count(handler_func_ptr);
            return false;
        }

        bool ContainObserver(const Address& address) 
        {
            std::shared_lock sl{m_observers_shared_mutex};
            return m_observers.count(address);
        }

    public:
        virtual ~Object() 
        {
            {
                std::shared_lock sl{m_notifiers_shared_mutex};
                for (const auto& notifier: m_notifiers) {
                    for (const auto& connection: notifier.second) {
                        connection.second();
                    }
                }
            }

            std::shared_lock sl{m_observers_shared_mutex};
            for (const auto& observer: m_observers) {
                observer.second();
            }
        }

        template<typename Notifier, typename Observer, typename... EmitArgs, typename... FuncArgs>
        friend void connect(
            Notifier* notifier,
            Event<EmitArgs...> Notifier::* event,
            Observer* observer,
            void (Observer::*func)(FuncArgs...)
        );

        template<typename Notifier, typename Observer, typename... EmitArgs, typename... FuncArgs>
        friend void disconnect(
            Notifier* notifier,
            Event<EmitArgs...> Notifier::* event,
            Observer* observer,
            void (Observer::*func)(FuncArgs...)
        );

        template<typename Notifier, typename Observer, typename Lambda, typename... EmitArgs, typename... FuncArgs>
        friend void connect(
            Notifier* notifier,
            Event<EmitArgs...> Notifier::* event,
            Observer* observer,
            Lambda&& lambda
        );
};

template<typename Notifier, typename Observer, typename... EmitArgs, typename... FuncArgs>
inline void connect(
    Notifier* notifier,
    Event<EmitArgs...> Notifier::* event,
    Observer* observer,
    void (Observer::*func)(FuncArgs...)) 
{
    Address observer_address{notifier, event};
    Address notifier_address{observer, func};

    if (!(notifier->ContainObserver(observer_address) && 
          observer->ContainNotifier(notifier_address, observer_address.function)))
    {
        auto event_handler = new EventHandler<Observer, std::tuple<FuncArgs...>, EmitArgs...>{
            observer, func
        };

        (notifier->*event).AddHandler(observer_address, event_handler);

        notifier->AddObserver(observer_address, [=] {
            observer->RemoveNotifier(notifier_address, observer_address.function);
        });

        observer->AddNotifier(notifier_address, observer_address.function, [=] {
            (notifier->*event).RemoveHandler(observer_address);
            notifier->RemoveObserver(observer_address);
        });
    }
}

template<typename Notifier, typename Observer, typename... EmitArgs, typename... FuncArgs>
inline void disconnect(
    Notifier* notifier,
    Event<EmitArgs...> Notifier::* event,
    Observer* observer,
    void (Observer::*func)(FuncArgs...)
) 
{
    Address observer_address{notifier, event};
    Address notifier_address{observer, func};

    if (notifier->ContainObserver(observer_address) && 
        observer->ContainNotifier(notifier_address, observer_address.function))
    {
        (notifier->*event).RemoveHandler(observer_address);
        notifier->RemoveObserver(observer_address);
        observer->RemoveNotifier(notifier_address, observer_address.function);
    }
}

template<typename Notifier, typename Observer, typename Lambda, typename... EmitArgs, typename... FuncArgs>
inline void connect(
    Notifier* notifier,
    Event<EmitArgs...> Notifier::* event,
    Observer* observer,
    Lambda&& lambda
)
{
    Address notifier_address{notifier, event};
    Address observer_address = (notifier->*event).ImitationFunctionHelper(observer, lambda, &Lambda::operator());

    notifier->AddObserver(observer_address, [=] {
        observer->RemoveNotifier(notifier_address, observer_address.function);
    });

    observer->AddNotifier(notifier_address, observer_address.function, [=] {
        (notifier->*event).RemoveHandler(observer_address);
        notifier->RemoveObserver(observer_address);
    });
}

template<typename Notifier, typename... EmitArgs, typename... FuncArgs>
inline void connect(
    Notifier* notifier,
    Event<EmitArgs...> Notifier::* event,
    void (*func)(FuncArgs...)
)
{
    auto event_handler = new EventHandler<void, std::tuple<FuncArgs...>, EmitArgs...>{func};
    (notifier->*event).AddHandler(Address{nullptr, func}, func);
}

template<typename Notifier, typename... EmitArgs, typename... FuncArgs>
inline void disconnect(
    Notifier* notifier, 
    Event<EmitArgs...> Notifier::* event, 
    void (*func)(FuncArgs...)
)
{
    (notifier->*event).RemoveHandler(Address{nullptr, func});
}
#pragma once

#include <tuple>
#include <functional>
#include "PrarameterCrop.hpp"
    
template<typename... Args>
class EventHandlerInterface {
public:
    EventHandlerInterface() = default;
    virtual ~EventHandlerInterface() = default;
    EventHandlerInterface(const EventHandlerInterface&) = delete;
    EventHandlerInterface& operator=(const EventHandlerInterface&) = delete;
    
    virtual void operator()(const Args& ...args) = 0;
};


template<typename T, typename Tuple, typename... Args>
class EventHandler : public EventHandlerInterface<Args...> {};

template<typename... TrueArgs, typename... Args>
class EventHandler<void, std::tuple<TrueArgs...>, Args...> : public EventHandlerInterface<Args...> {
    private:
        std::function<void(TrueArgs...)> m_handler;
    public:
        template<typename Callable>
        EventHandler(Callable&& func) {
            m_handler = std::forward<Callable>(func);
        }

        void operator()(const Args& ...args) final {
            TupleTake(m_handler, std::make_tuple(args...));      
        }
};

template<typename T, typename... TrueArgs, typename... Args>
class EventHandler<T, std::tuple<TrueArgs...>, Args...> : public EventHandlerInterface<Args...> {
    private:
        using FunctionPointer = void (T::*)(TrueArgs...);
        using ConstFunctionPointer = void (T::*)(TrueArgs...) const;
        
        T *m_receiver;
        std::function<void(T*, TrueArgs...)> m_handler;
    public:
        //非const函数
        EventHandler(T* receiver, FunctionPointer handler) {
            m_receiver = receiver;
            m_handler = handler;
        }
        
        EventHandler(T* receiver, ConstFunctionPointer handler) {
            m_receiver = receiver;
            m_handler = handler;
        }

        void operator()(const Args& ...args) final {
            TupleTake(m_handler, std::make_tuple(m_receiver, args...));      
        }
};

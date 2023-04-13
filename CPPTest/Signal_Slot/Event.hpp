#pragma once

#include "EventHandler.hpp"
#include "Address.hpp"
#include "EventLoop.hpp"
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <optional>
#include <cassert>

enum class ConnectionType  
{  
    AutoConnection,  
    DirectConnection,  
    QueuedConnection,  
    BlockingQueuedConnection,  
};



template<typename... Args>
class Event {
    using Handler = EventHandlerInterface<Args...>*;

    struct Task
    {
        std::optional<std::thread::id> thread_id;
        Handler event_handler;
        ConnectionType connection_type;
    };

    private:

        std::unordered_map<Address, Task, AddressHasher> m_tasks_map;
        mutable std::shared_mutex m_shared_mutex;
    
    private:
        void AddTask(const Address& address, Task task) {
            std::lock_guard lg{m_shared_mutex};
            
            if (!m_tasks_map.count(address)) {
                m_tasks_map[address] = task;
            }
        }

        void RemoveTask(const Address& address) {
            std::lock_guard lg{m_shared_mutex};
            
            if (m_tasks_map.count(address)) {
                delete m_tasks_map[address].event_handler;
                m_tasks_map.erase(address);
            }
        }

    private:
        template<typename Observer, typename Lambda, typename... FuncArgs>
        Address ImitationFunctionHelper(Observer* observer, Lambda&& t, void(std::decay_t<Lambda>::*func)(FuncArgs...), ConnectionType connection_type) 
        {
            Task task;
            SetTaskThreadId(task, connection_type);
            task.event_handler = new EventHandler<void, std::tuple<FuncArgs...>, Args...>{std::move(t)};
            task.thread_id = observer->ThreadId();
            task.connection_type = connection_type;

            Address address{observer, func};

            AddTask(address,task);
            return address;
        }

        template<typename Observer, typename Lambda, typename... FuncArgs>
        Address ImitationFunctionHelper(Observer* observer, Lambda&& t, void(std::decay_t<Lambda>::*func)(FuncArgs...) const, ConnectionType connection_type) 
        {
            Task task;
            task.event_handler = new EventHandler<void, std::tuple<FuncArgs...>, Args...>{std::move(t)};
            task.thread_id = observer->ThreadId();
            task.connection_type = connection_type;

            Address address{observer, func};

            AddTask(address,task);
            return address;
        }

    public:
        void emit(const Args&... args) {
            std::shared_lock sl{m_shared_mutex};
            for (auto& task: m_tasks_map) {
                if (task.second.connection_type == ConnectionType::AutoConnection)
                {
                    if (task.second.thread_id == std::this_thread::get_id())
                    {
                        (*task.second.event_handler)(args...);
                    }
                    else
                    {
                        EventLoop *loop = EventLoopManger::GetInstance()->GetEventLoop(task.second.thread_id.value_or(std::thread::id()));
                        if (loop)
                        {
                            loop->PostEvent(
                            [handler = task.second.event_handler, args...]
                            {
                                (*handler)(args...);
                            });
                        }
                    }
                }
                else if (task.second.connection_type == ConnectionType::DirectConnection)
                {
                    (*task.second.event_handler)(args...);
                }
                else if (task.second.connection_type == ConnectionType::QueuedConnection)
                {
                    EventLoop *loop = EventLoopManger::GetInstance()->GetEventLoop(task.second.thread_id.value_or(std::thread::id()));
                    if (loop)
                    {
                        loop->PostEvent(
                        [handler = task.second.event_handler, args...]
                        {
                            (*handler)(args...);
                        });
                    }
                }
                else if (task.second.connection_type == ConnectionType::BlockingQueuedConnection)
                {
                    EventLoop *loop = EventLoopManger::GetInstance()->GetEventLoop(task.second.thread_id.value_or(std::thread::id()));
                    if (loop)
                    {
                        loop->SendEvent(
                        [handler = task.second.event_handler, args...]
                        {
                            (*handler)(args...);
                        });
                    }
                }
            }
        }


        ~Event() {
            std::lock_guard lg{m_shared_mutex};
            for (auto& handler: m_tasks_map) {
                delete handler.second.event_handler;
            }
        }

    public:
        template<typename Notifier, typename Observer, typename... EmitArgs, typename... HandlerArgs>
        friend void connect(
            Notifier* notifier,
            Event<EmitArgs...> Notifier::* event,
            Observer* observer,
            void (Observer::*handler)(HandlerArgs...),
            ConnectionType connection_type
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
            Lambda&& lambda,
            ConnectionType connection_type
        );
};
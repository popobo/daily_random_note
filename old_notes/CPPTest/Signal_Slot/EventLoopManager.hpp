#pragma once

#include <unordered_map>
#include <thread>
#include <shared_mutex>

class EventLoop;

class EventLoopManger 
{
private:
    std::unordered_map<std::thread::id, EventLoop*> m_event_loop;
    std::shared_mutex m_shared_mutex;
    EventLoopManger() = default;
    ~EventLoopManger() = default;

public:
    static EventLoopManger* GetInstance() {
        static EventLoopManger instance;
        return &instance;
    }

    EventLoopManger(const EventLoopManger&) = delete;
    EventLoopManger& operator=(const EventLoopManger&) = delete;

    void AddEventLoop(EventLoop* event_loop)
    {
        std::lock_guard lg{m_shared_mutex};
        m_event_loop.insert(std::make_pair(std::this_thread::get_id(), event_loop));
    }

    void RemoveEventLoop()
    {
        std::lock_guard lg{m_shared_mutex};
        m_event_loop.erase(std::this_thread::get_id());
    }

    EventLoop* GetEventLoop(std::thread::id id)
    {
        std::shared_lock sl{m_shared_mutex};
        if(m_event_loop.count(id))
        {
            return m_event_loop[id];
        }
        return nullptr;
    }
};
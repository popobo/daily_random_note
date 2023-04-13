#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>
#include <functional>
#include "EventLoopManager.hpp"
#include <atomic>

class EventLoop {
private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::deque<std::function<void()>> m_message;
    std::atomic<bool> m_quit;
    
public:
    EventLoop()
    {
        EventLoopManger::GetInstance()->AddEventLoop(this);
    }

    ~EventLoop()
    {
        EventLoopManger::GetInstance()->RemoveEventLoop();
    }

    template<typename Callable>
    void PostEvent(Callable&& func)
    {
        std::lock_guard lg{m_mutex};
        m_message.push_back(std::forward<Callable>(func));
        m_cv.notify_one();
    }

    template<typename Callable>
    void SendEvent(Callable&& func)
    {
        std::unique_lock ul{m_mutex};
        std::atomic<bool> finished = false;
        m_message.push_back(
        [&]
        {
            std::function<void()> f = std::forward<Callable>(func);    
            f();
            finished = true;
        });
        m_cv.notify_one();
        m_cv.wait(ul, 
        [&finished]
        {
            return finished.load();
        });
    }

    void Run()
    {
        m_quit = false;
        while(!m_quit)
        {
            std::unique_lock ul{m_mutex};
            m_cv.wait(ul, 
            [this]
            {
                return !m_message.empty() || m_quit;
            });

            if (m_quit)
                break;
            
            std::function<void()> func = std::move(m_message.front());
            m_message.pop_front();
            ul.unlock();
            func();
            m_cv.notify_all();
        }
    }

    void Quit()
    {
        m_quit = true;
        m_cv.notify_all();
    }
};
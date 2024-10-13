#pragma once

#include "Event.hpp"
#include <iostream>

class Observable {
public:
    auto& GetWriteEvent() {
        return m_writeEvent;
    }

    auto& GetWriteEndEvent() {
        return m_writeEndEvent;
    }
    
    void OnWrite(int pos, int length) {
        GetWriteEvent().raise(pos, length);
    }

    void OnWriteEnd() {
        GetWriteEndEvent().raise();
    }

private:
    Event<int, int> m_writeEvent;
    Event<> m_writeEndEvent;
};
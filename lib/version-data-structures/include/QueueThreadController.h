#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "VersionedQueue.h"

namespace {
    constexpr int MAIN_THREAD_INDEX = 0;
}

namespace VersionedStructures {
    template <class Type> class QueueThreadController {
    public:
        QueueThreadController(const size_t threadLimit = 12) : m_threadLimit(threadLimit) {
            m_states.resize(threadLimit);
            m_states[MAIN_THREAD_INDEX] = std::make_shared<StackState<Type>>();
        }

        Queue<Type> GetQueue(size_t threadIndex = MAIN_THREAD_INDEX) {
            if (threadIndex >= m_threadLimit) {
                throw std::logic_error("Thread number more than thread limit!");
            }
            if (!m_states[threadIndex]) {
                m_states[threadIndex] = std::make_shared<StackState<Type>>(*m_states[MAIN_THREAD_INDEX]);
                m_states[threadIndex]->SetStartState(m_states[threadIndex]->GetEndState());
            }
            return Queue<Type>(m_states[threadIndex]);
        }

        StackState<Type> GetQueueState(size_t threadIndex = MAIN_THREAD_INDEX) const {
            if (threadIndex >= m_threadLimit) {
                throw std::logic_error("Thread number more than thread limit!");
            }
            const auto state = m_states[threadIndex];
            if (!state) {
                throw std::logic_error("Not exist Queue for this thread index!");
            }
            return *state;
        }

    private:
        size_t m_threadLimit = 12;
        std::vector<std::shared_ptr<StackState<Type>>> m_states;
    };
}
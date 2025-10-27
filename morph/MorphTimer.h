#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <map>
#include <mutex>
#include <thread>

#include "../aspectable/Aspectable.h"

namespace morph
{
    struct MorphTimer : virtual aspectable::Aspectable<aspectable::Aspect>
    {
        MorphTimer() : aspectable::Aspectable<aspectable::Aspect>(this) {}

        int setTimeout(const std::function<void()>& callback, const int delayMS)
        {
            const int id = m_nextId++;
            m_callbacks[id] = callback;

            // TODO: v8 isolate can only be used in on thread. 
            // Need use task runner to post task in worker thread.
            // Currently make setTimout synchronous.
            // std::thread([this, id, delayMS, callback]() {
            //     std::this_thread::sleep_for(std::chrono::milliseconds(delayMS));
            //     std::lock_guard<std::mutex> lock(m_mutex);
            //     if (m_callbacks.count(id))
            //     {
            //         callback();
            //         m_callbacks.erase(id);
            //     }
            // }).detach();

            callback();
            m_callbacks.erase(id);

            return id;
        }

        void clearTimeout(const int id)
        {
            m_callbacks.erase(id);
        }

        private:
        std::atomic<int> m_nextId{1};
        std::map<int, std::function<void()>> m_callbacks;
    };
}
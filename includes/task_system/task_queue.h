#pragma once

#include <functional>
#include <mutex>
#include <vector>
#include <optional>

struct TaskQueue {
    struct TryLock {
        uintptr_t address;
        [[nodiscard]] TryLock(std::mutex & mutex) {
            uintptr_t success = mutex.try_lock();
            address = reinterpret_cast<uintptr_t>(&mutex) | success;
        }
        operator bool() { return address & 1; }
        ~TryLock() { if (address & 1) {
            auto mutex = reinterpret_cast<std::mutex *>(address & ~1LU);
            mutex->unlock();
        }}
        TryLock(TryLock &) = delete;
    };
    static_assert(sizeof(TryLock) == 8);
    using Task = std::function<void()>;
    std::mutex mutex;
    std::vector<Task> tasks;
    bool try_push(Task && task) {
        auto locked = TryLock(mutex);
        if (locked) tasks.push_back(std::move(task));
        return locked;
    }
    std::optional<Task> try_pop() {
        auto locked = TryLock(mutex);
        if (!locked || tasks.empty()) return {};
        auto ret = std::optional<Task>{ std::move(tasks.back()) };
        tasks.pop_back();
        return ret;
    }
};

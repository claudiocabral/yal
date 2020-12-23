#pragma once

#include <thread>
#include <task_system/task_queue.h>
#include <algorithm>
#include <atomic>

struct TaskSystem {
    unsigned count{std::thread::hardware_concurrency()};
    std::vector<TaskQueue> queues{count};
    std::vector<std::thread> threads;
    int current = 0;
    std::atomic<int> tasks = 0;
    bool done = false;

    void spawn(TaskQueue::Task && task) {
        tasks.fetch_add(1, std::memory_order_release);
        auto i = current++;
        auto size = queues.size();
        while (true) {
            if (done || queues[i++ % size].try_push(std::move(task))) return;
        }
    }
    void runTask(int threadId) {
        auto size = queues.size();
        int i = threadId;
        if (tasks == 0) return;
        while (true) {
            if (done) return;
            auto task = queues[i++ % size].try_pop();
            if (task) {
                (*task)();
                auto remaningTasks = tasks.fetch_sub(1, std::memory_order_release) - 1;
                if (remaningTasks <= 0) done = true;
            }
        }
    }
    void run() {
        for (uint8_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
            threads.emplace_back([&, i]() {
                    this->runTask(i);
                    });
        }
        for (auto & t: threads) t.join();
    }
};

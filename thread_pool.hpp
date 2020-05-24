#pragma once

#include <atomic>
#include <vector>
#include <iostream>
#include "thread_safe_queue.hpp"
#include "common.hpp"
class thread_pool
{
    std::atomic_bool done;
    thread_safe_queue<std::function<void()>> work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread()
    {
        while (!done)
        {
            std::function<void()> task;
            if (work_queue.pop(task))
            {
                task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

public:
    thread_pool() : done(false), joiner(threads)
    {
        int const thread_cnt = std::thread::hardware_concurrency();

        std::cout << "total number of thread is " << thread_cnt << std::endl;

        for (int i = 0; i < thread_cnt; i++)
        {
            threads.push_back(std::thread(&thread_pool::worker_thread, this));
        }
    }
    ~thread_pool()
    {
        done = false;
    }
    template <typename fp>
    void submit(fp f)
    {
        work_queue.push(std::function<void()>(f));
    }
};

void run_thread_pool()
{
    thread_pool tp;
    std::cout << "Testing TP " << endl;

    for (int i = 0; i < 100; i++)
    {
        tp.submit([=] {
            std::cout << "Thread_id is & i " << std::this_thread::get_id() << "........" << i << endl;
        });
    }
}
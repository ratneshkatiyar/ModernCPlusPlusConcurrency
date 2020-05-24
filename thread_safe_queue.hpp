#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>

using namespace std;

template <typename T>
class thread_safe_queue
{
    std::mutex m;
    condition_variable cv;
    std::queue<std::shared_ptr<T>> q;

public:
    thread_safe_queue() = default;
    thread_safe_queue(thread_safe_queue const &obj)
    {
        std::lock_guard<std::mutex> lg(obj.m);
        q = obj.q;
    }
    void push(T val)
    {
        std::lock_guard<std::mutex> lg(m);
        q.push(std::make_shared<T>(val));
        cv.notify_one();
    }
    std::shared_ptr<T> pop()
    {
    std:;
        std::lock_guard<std::mutex> lg(m);
        if (q.empty())
            return std::shared_ptr<T>();
        else
        {
            std::shared_ptr<T> ref(q.front());
            q.pop();
            return ref;
        }
    }

    std::shared_ptr<T> wait_pop()
    {
        std::unique_lock<std::mutex> lg(m);
        cv.wait(lg, [this]() { return !q.empty(); });
        std::shared_ptr<T> ref(q.front());
        q.pop();
        return ref;
    }
    bool wait_pop(T &ref)
    {
        std::unique_lock<std::mutex> lg(m);
        cv.wait(lg, [this] {
            return !q.empty();
        });
        ref = q.front();
        q.pop();
    }

    size_t size()
    {
        std::lock_guard<std::mutex> lg(m);
        return q.size();
    }

    bool pop(T &ref)
    {
        std::lock_guard<std::mutex> lg(m);
        if (q.empty())
        {
            return false;
        }
        else
        {
            ref = *(q.front().get());
            q.pop();
            return true;
        }
    }
};
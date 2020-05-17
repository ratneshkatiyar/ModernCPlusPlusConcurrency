#pragma once

#include <iostream>
#include <memory>
#include <stack>
#include <thread>
#include <mutex>

template <typename T>
class threadsafe_stack
{
    std::stack<std::shared_ptr<T>> s;
    std::mutex m;

public:
    void push(T val)
    {
        std::lock_guard<std::mutex> lg(m);
        s.push(std::make_shared<T>(val));
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lg(m);
        if (s.empty())
        {
            //throw std::runtime_error("stack is empty");
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> res(s.top());
        s.pop();
        return res;
    }

    void pop(T &val)
    {
        std::lock_guard<std::mutex> lg(m);
        if (s.empty())
        {
            throw std::runtime_error("stack is empty");
        }
        val = *(s.top().get());
        s.pop();
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lg(m);
        return s.empty();
    }
    size_t size()
    {
        std::lock_guard<std::mutex> lg(m);
        return s.size();
    }
};
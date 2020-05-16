#pragma once

#include <vector>
#include <thread>

using namespace std;

class thread_guard
{
    thread &t;
public:
     explicit thread_guard(std::thread & _t):t(_t)
    {

    }
    ~thread_guard()
    {
        if(t.joinable())
        {
            t.join();
        }
    }

    thread_guard(const thread_guard&) = delete;
    thread_guard(thread_guard &&obj) = delete;
    thread_guard &operator=(const thread_guard &) = delete;
    thread_guard &operator= (thread_guard&&) = delete;
};
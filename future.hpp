#pragma once
#include <future>

#include <iostream>

void print()
{
    std::cout << "Inside print fun " << std::endl;
}

float add(float &x, float &y)
{
    std::cout << "Inside add fun " << std::endl;
    return x + y;
}

int sub(int x, int y)
{
    std::cout << "Inside sub fun" << std::endl;
    return x - y;
}

int fact(std::future<int> &f)
{
    int num = f.get(); // will wait until we set the value in promise
    return num;
}

void run()
{
    std::cout << "main thread id " << std::this_thread::get_id() << std::endl;
    float a = 50.5;
    float b = 60.3;
    // future will be according to the return value from a function
    // this is the ways we return some value from a async thread
    //1) print returns void
    std::future<void> fp = std::async(std::launch::async, print);
    //2 ) add returns float
    std::future<float> fa = std::async(std::launch::async, add, std::ref(a), std::ref(b));
    //3) sub returns int

    fp.get();
    auto res_sum = fa.get();
    std::cout << "value of sume is " << res_sum << std::endl;

    // reverse can also we done , when main thread send some value to other thread,
    // when the value will be ready

    std::promise<int> p;
    std::future<int> pf = p.get_future();
    std::future<int> fu = std::async(std::launch::async, fact, std::ref(pf));
    /*................do some other work and set promise***********/
    p.set_value(4);
    std::cout << "value return from fact function is " << fu.get() << std::endl;

    // for broadcasting value to multiple threads check std::shared_future<int>sf = pf.share();
}

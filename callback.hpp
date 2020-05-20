#pragma once
#include <iostream>
#include <vector>
#include <functional>

//1) function pointer
//2) pointer to member function
//3) functor
//4) Lambda function

//function ptr
float functionpointercb(float a, float b)
{
    std::cout << "Inside functionpointer cb" << std::endl;
}
//pointer to member func
class someotherclass
{
public:
    float call(float a, float b)
    {
        std::cout << "Inside call function " << std::endl;
    }
};

//functor
class functorcb
{
public:
    float operator()(float a, float b)
    {
        std::cout << "Inside functor class " << std::endl;
    }
};

//lambda class
auto cb = [](float a, float b) -> float {
    std::cout << "Inside lambda  func " << std::endl;
};

class SomeWork
{

public:
    using Tcallback = std::function<float(float, float)>;
    using Tcallbackvector = std::vector<Tcallback>;

    void addcallbacks(Tcallback cb)
    {
        m_callbacks.push_back(cb);
    }
    float callbackstocall()
    {
        auto res = 0;
        for (const auto &cb : m_callbacks)
        {
            res = cb(a, b);
        }
        return res;
    }

private:
    Tcallbackvector m_callbacks;
    float a = 3.3;
    float b = 6.5;
};

int main()
{
    SomeWork sw;
    float (*fp)(float, float) = functionpointercb;
    someotherclass soc;
    sw.addcallbacks(fp);
    auto cb_a = std::bind(&someotherclass::call,
                          &soc, //(*this)
                          std::placeholders::_1,
                          std::placeholders::_2);
    sw.addcallbacks(cb_a);
    functorcb cb_f;
    sw.addcallbacks(cb_f);
    auto l_cb = [](float a, float b) -> float {
        std::cout << "Inside lambda  func " << std::endl;
    };
    sw.addcallbacks(l_cb);

    sw.callbackstocall();
    return 0;
}
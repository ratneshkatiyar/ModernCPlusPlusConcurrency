#pragma once

#include <thread>
#include <vector>
#include <numeric>
#include <iostream>
#include <algorithm>
#define MIN_BLOCK_SIZE 1000

using namespace std;

template <typename iterator, typename T>
void accumulate(iterator start, iterator end, T &ref)
{
    ref = std::accumulate(start, end, 0);
}

template <typename iterator, typename T>
T parallel_accumlate(iterator start, iterator end, T &ref)
{
    auto input_size = std::distance(start, end);
    int allowed_thread_by_elements = input_size / MIN_BLOCK_SIZE;
    int allowed_thread_by_hardware = std::thread::hardware_concurrency();
    auto num_threads = std::min(allowed_thread_by_elements, allowed_thread_by_hardware);
    auto block_size = (input_size + 1) / num_threads;

    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);

    iterator last;

    for (int i = 0; i < num_threads - 1; i++)
    {
        last = start;
        std::advance(last, block_size);
        threads[i] = std::thread(accumulate<iterator, T>, start, last, std::ref(results[i]));
        start = last;
    }
    results[num_threads - 1] = std::accumulate(start, end, 0);

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return std::accumulate(results.begin(), results.end(), ref);
}

void start()
{
    const int size = 8000;
    vector<int> my_arr(size);
    int ref = 0;

    srand(0);
    for (int i = 0; i < size; i++)
    {
        my_arr[i] = rand() % 10;
    }

    auto ret_val = parallel_accumlate<vector<int>::iterator, int>(my_arr.begin(), my_arr.end(), ref);
    std::cout <<"Return value received is "<<ret_val<<std::endl;
}
# YA-THREADPOOL
Yet another C++11 threadpool implementation. Header-only threadpool implementation 

## Usage Notes
When constructing the threadpool, you can choose not to provide a number of threads,
and it will default to your system's own maximum allowed threads (according to `std::thread::hardware_concurrency()`).

If you choose to provide a pool-size (`your_size`), there will be allocated `std::min(std::thread::hardware_concurrency(), your_size)` threads in the pool.

## Example Usage
```c++
#include <threadpool.h> // include the library
// example functions to pool
void printA() { std::cout << "A"; }
void printB() { std::cout << "B"; }

{ // Place the threadpool in a scope if you dont want your tasks to overlap
    ya::threadpool my_pool{5};
    // You can push one task at a time:
    my_pool.push_task(printA);
    my_pool.push_task(printB);
    // RESULT: (may vary)
    // AB
}
std::cout << std::endl;
{
    ya::threadpool my_pool{5};
    // Or you can push many tasks at once:
    std::vector<ya::threadpool::task_t> my_tasks{};
    my_tasks.emplace_back(printA);
    my_tasks.emplace_back(printB);
    my_tasks.emplace_back(printA);
    my_tasks.emplace_back(printB);
    my_tasks.emplace_back(printA);
    my_tasks.emplace_back(printB);
    my_tasks.emplace_back(printA);
    my_tasks.emplace_back(printB);
    my_pool.push_many(my_tasks);
    // RESULT: (may vary)
    // ABABAABB
}
```

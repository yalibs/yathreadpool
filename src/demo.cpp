/* MIT License

Copyright (c) 2022 Asger Gitz-Johansen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <iostream>
#include "implementation/threadpool.h"

int main(int argc, char** argv) {
    auto printA = []() { std::cout << "A"; };
    auto printB = []() { std::cout << "B"; };
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
    return 0;
}

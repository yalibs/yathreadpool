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
#ifndef YATHREADPOOL_THREADPOOL_H
#define YATHREADPOOL_THREADPOOL_H
#include <functional>
#include <thread>
#include <vector>
#include <queue>

class threadpool {
public:
    using task_t = std::function<void()>;

    threadpool() : tasks{}, pool{}, task_lock{}, task_q{}, alive{true} {
        initialize(std::thread::hardware_concurrency());
    }

    explicit threadpool(size_t pool_size) : tasks{}, pool{}, task_lock{}, task_q{}, alive{true} {
        initialize(std::min<size_t>(std::thread::hardware_concurrency(), pool_size));
    }

    void push_task(const task_t& t) {
        std::lock_guard<std::mutex> lock{task_lock};
        tasks.push(t);
        task_q.notify_all();
    }

    void push_many(const std::vector<task_t>& v_tasks) {
        std::lock_guard<std::mutex> lock{task_lock};
        for(auto& t : v_tasks) {
            tasks.push(t);
        }
        task_q.notify_all();
    }

    ~threadpool() {
        {
            std::lock_guard<std::mutex> lock{task_lock};
            alive = false;
            task_q.notify_all(); // We are no longer alive, so close the pool
        }
        for(auto& t : pool)
            t.join();
    }

private:
    std::queue<task_t> tasks;
    std::vector<std::thread> pool;
    std::mutex task_lock;
    std::condition_variable task_q;
    std::atomic_bool alive;
    void initialize(size_t size) {
        for (int i = 0; i < size; i++) {
            pool.emplace_back([&](){ while(true) {
                task_t t;
                {
                    std::unique_lock<std::mutex> a(task_lock);
                    while(tasks.empty() && alive.load())
                        task_q.wait(a);
                    if(tasks.empty() && !alive.load())
                        return;
                    t = tasks.front();
                    tasks.pop();
                }
                t();
            } });
        }
    }
};

#endif

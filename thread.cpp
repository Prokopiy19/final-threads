#include <iostream>
#include <mutex>
#include <thread>
#include <queue>

void thread1_main(std::queue<int>& q, std::mutex& m)
{
    for (int i = 0; i < 1000; ++i) {
        const std::lock_guard<std::mutex> lock{m};
        q.emplace(i);
        std::cout << "thread 1 added " << i << " to queue" << std::endl;
    }
}

void thread2_main(std::queue<int>& q, std::mutex& m)
{
    int received = 0;
    while (received < 1000) {
        const std::lock_guard<std::mutex> lock{m};
        if (!q.empty()) {
            std::cout << "thread 2 received " << q.front() << std::endl;
            q.pop();
            ++received;
        }
    }
}

int main()
{
    std::queue<int> q;
    std::mutex m;
    std::thread t1(thread1_main, std::ref(q), std::ref(m));
    std::thread t2(thread2_main, std::ref(q), std::ref(m));
    t1.join();
    t2.join();
    return 0;
}
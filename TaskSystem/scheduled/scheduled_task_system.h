#ifndef scheduled_task_system_h
#define scheduled_task_system_h

#include <vector>
#include <thread>
#include <atomic>
#include "scheduled_notification_queue.h"

class scheduled_task_system {
    const unsigned                              _count{10 * std::thread::hardware_concurrency()};
    std::vector<std::thread>                    _threads;
    std::vector<scheduled_notification_queue>   _q{_count};
    std::atomic<unsigned>                       _index{0};

    void run(unsigned i) {
        while (true) {
            std::function<void()> f;
            if (!_q[i].pop(f)) break;
            f();
        }
    }

public:
    scheduled_task_system() {
        for(unsigned n=0; n != _count; ++n)
            _threads.emplace_back([&, n]{ run(n); });
    }

    ~scheduled_task_system() {
        for(auto& e: _q) e.done();
        for(auto& e : _threads) e.join();
    }

    template <typename F>
    void async_(F&& f) {
        auto i = _index++;
        _q[i % _count].push(std::forward<F>(f));
    }
};

#endif /* scheduled_task_system_h */

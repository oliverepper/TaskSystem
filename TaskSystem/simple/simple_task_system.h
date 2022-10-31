#ifndef simple_task_system_h
#define simple_task_system_h

#include <vector>
#include <thread>
#include "simple_notification_queue.h"

class simple_task_system {
    const unsigned              _count{10 * std::thread::hardware_concurrency()};
    std::vector<std::thread>    _threads;
    simple_notification_queue   _q;

    void run() {
        while (true) {
            std::function<void()> f;
            if (!_q.pop(f)) break;
            f();
        }
    }

public:
    simple_task_system() {
        for(unsigned n=0; n != _count; ++n)
            _threads.emplace_back([&]{ run(); });
    }

    ~simple_task_system() {
        _q.done();
        for(auto& e : _threads) e.join();
    }

    template <typename F>
    void async_(F&& f) {
        _q.push(std::forward<F>(f));
    }
};

#endif /* simple_task_system_h */

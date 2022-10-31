#ifndef stealing_task_system_h
#define stealing_task_system_h

#include <vector>
#include <thread>
#include <atomic>
#include "stealing_notification_queue.h"

class stealing_task_system {
    const unsigned                              _count{10 * std::thread::hardware_concurrency()};
    std::vector<std::thread>                    _threads;
    std::vector<stealing_notification_queue>    _q{_count};
    std::atomic<unsigned>                       _index{0};
    const unsigned                              _spins{1};

    void run(unsigned i) {
        while (true) {
            std::function<void()> f;

            for(unsigned n=0; n != _spins * _count; ++n)
                if(_q[(i + n) % _count].try_pop(f)) break;

            if(!f && !_q[i].pop(f)) break;
            f();
        }
    }

public:
    stealing_task_system(const unsigned spins = 1) : _spins{spins} {
        for(unsigned n=0; n != _count; ++n)
            _threads.emplace_back([&, n]{ run(n); });
    }

    ~stealing_task_system() {
        for(auto& e: _q) e.done();
        for(auto& e : _threads) e.join();
    }

    template <typename F>
    void async_(F&& f) {
        auto i = _index++;

        for (unsigned n = 0; n != _count; ++n)
            if (_q[(i + n) % _count].try_push(std::forward<F>(f))) return;

        _q[i % _count].push(std::forward<F>(f));
    }
};

#endif /* stealing_task_system_h */

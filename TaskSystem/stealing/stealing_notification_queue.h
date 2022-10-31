#ifndef stealing_notification_queue_h
#define stealing_notification_queue_h

#include <deque>
#include <functional>
#include <mutex>
#include <condition_variable>

class stealing_notification_queue {
    std::deque<std::function<void()>>   _q;
    bool                                _done{false};
    std::mutex                          _mutex;
    std::condition_variable             _ready_cond;

public:
    void done() {
        {
            std::unique_lock<std::mutex> lock{_mutex};
            _done = true;
        }
        _ready_cond.notify_all();
    }

    bool pop(std::function<void()>& x) {
        std::unique_lock<std::mutex> lock{_mutex};
        while (_q.empty() && !_done) _ready_cond.wait(lock);
        if (_q.empty()) return false;
        x = std::move(_q.front());
        _q.pop_front();
        return true;
    }

    template <typename  F>
    void push(F&& f) {
        {
            std::unique_lock<std::mutex> lock{_mutex};
            _q.emplace_back(std::forward<F>(f));
        }
        _ready_cond.notify_one();
    }

    bool try_pop(std::function<void()>& x) {
        std::unique_lock<std::mutex> lock{_mutex, std::try_to_lock};
        if(!lock || _q.empty()) return false;
        x = std::move(_q.front());
        _q.pop_front();
        return true;
    }

    template <typename  F>
    bool try_push(F&& f) {
        {
            std::unique_lock<std::mutex> lock{_mutex, std::try_to_lock};
            if(!lock) return false;
            _q.emplace_back(std::forward<F>(f));
        }
        _ready_cond.notify_one();
        return true;
    }
};

#endif /* stealing_notification_queue_h */

#ifndef no_task_system_h
#define no_task_system_h

struct no_task_system {
    template <typename F>
    void async_(F&& f) {
        f();
    }
};

#endif /* no_task_system_h */

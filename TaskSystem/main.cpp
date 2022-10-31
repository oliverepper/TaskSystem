#include <iostream>
#include <numeric>
#include <iomanip>
#include "no_task_system.h"
#include "simple_task_system.h"
#include "scheduled_task_system.h"
#include "stealing_task_system.h"

namespace TS {
struct system {
    enum struct kind { no, simple, scheduled, stealing, stealing_8 };
    constexpr static const kind all[] = { kind::no, kind::simple, kind::scheduled, kind::stealing, kind::stealing_8 };
    system(const kind& x) : m_kind(x) {}
    friend
    std::ostream& operator<<(std::ostream& os, const system& x) {
        switch (x.m_kind) {
            case kind::no: os << "No:"; break;
            case kind::simple: os << "Simple:"; break;
            case kind::scheduled: os << "Scheduled:"; break;
            case kind::stealing: os << "Stealing:"; break;
            case kind::stealing_8: os << "Stealing(8):"; break;
        }
        return os;
    }
private:
    kind m_kind;
};
}

__attribute__((optnone)) int upto(int n) {
    if(n == 1) return n;
    return 1 + upto(n - 1);
}

__attribute__((optnone)) void work() {
    if (upto(500) != 500) throw std::runtime_error("error");
}

auto run_test(TS::system::kind system) {
    constexpr unsigned int numberOfTasks = 250000;
    auto use = [](auto&& system) {
        for(unsigned i=0; i<numberOfTasks; ++i)
            system.async_(work);
    };
    auto start = std::chrono::steady_clock::now();

        switch(system) {
            case TS::system::kind::no: use(no_task_system()); break;
            case TS::system::kind::simple: use(simple_task_system()); break;
            case TS::system::kind::scheduled: use(scheduled_task_system()); break;
            case TS::system::kind::stealing: use(stealing_task_system()); break;
            case TS::system::kind::stealing_8: use(stealing_task_system(8)); break;
        }

    auto stop = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
}

int main() {
    constexpr unsigned int runs = 10;
    std::vector<unsigned int> durations{runs};

    for(const auto& system : TS::system::all) {
        std::cout << std::setw(sizeof("stealing(8): ")) << std::left << system;
        for(unsigned i=0; i < runs; ++i) {
            durations.emplace_back(run_test(system).count());
            std::cout << std::setw(4) << std::right << durations.back() << ", ";
            std::cout.flush();
        }
        std::cout << "Mean: "
            << std::setw(4) << std::accumulate(durations.begin(), durations.end(), 0) / runs
            << std::endl;
        durations.clear();
    }

    return 0;
}

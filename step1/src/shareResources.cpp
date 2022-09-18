#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <vector>
#include <string>
#include <random>

using namespace std::string_literals;

static const size_t kNumThreads = 5;

//--------------------------------------
long long
getRandom() {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_int_distribution<long long> random(200, 600);

    return random(mt);
}

//--------------------------------------
int
main() {
    std::vector<std::thread>    threadPool;
    std::vector<std::string>    strings;        // shared resource (complex, like user defined)
    std::mutex                  mutex;
    std::atomic_bool            finish = false; // shared resource (basic type)
    std::atomic_size_t          counter{};      // shared resource (basic type)

    // worker
    auto worker = [&finish, &strings, &mutex, &counter](size_t id) {
        size_t localCounter{};
        char name[16];

        snprintf(name, sizeof(name), "Thread %zu", id);

        while (!finish) {   // atomic
            std::this_thread::sleep_for(std::chrono::milliseconds(getRandom()));
            {
                // Protect the complex (user defined) data type with a mutex
                std::lock_guard<std::mutex> lock(mutex);
                strings.emplace_back(name);
            }
            ++localCounter;
        }

        printf("'%s' added '%zu' strings\n", name, localCounter);
        counter += localCounter;    // atomic
    };

    // Populate thread pool with workers
    threadPool.reserve(kNumThreads);
    for (size_t i = 0; i < kNumThreads; ++i) {
        threadPool.emplace_back(worker, i);
        
    }

    // Wait some time
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Finish all threads
    finish = true;

    // Join them
    for (auto &thr : threadPool) {
        thr.join();
    }

    printf("Total strings added: %zu\n", counter.load());
    for (size_t i = 0; i < strings.size(); ++i) {
        printf(" - %2zu: %s\n", i+1, strings[i].c_str());
    }

    return 0;
}
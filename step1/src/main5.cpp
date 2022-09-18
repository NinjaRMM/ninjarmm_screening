#include <chrono>
#include <thread>
#include <cstdio>
#include <cstdint>
#include <cinttypes>

using namespace std::chrono;

static inline high_resolution_clock::time_point
Now() {
    return high_resolution_clock::now();
}

uint64_t
GetTimePoint() {
    return time_point_cast<milliseconds>(Now()).time_since_epoch().count();
}

uint64_t
GetA() {
    // This variable is initialized the first time it is accesed
    static auto timestamp = GetTimePoint();
    return timestamp;
}

uint64_t
GetB() {
    // This variable is initialized the first time it is accesed
    static auto timestamp = GetTimePoint();
    return timestamp;
}

int
main() {
    auto start = GetTimePoint();
    printf("%" PRIu64 "\n", GetA() - start);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("%" PRIu64 "\n", GetB() - start);

    return 0;
}

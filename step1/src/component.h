#pragma once

#include <cstdint>

struct Counter {
    inline static uint64_t currentId = 0;
};

template <typename T>
struct Component {
    inline static const uint64_t id = Counter::currentId++;
};

template <typename C>
inline uint64_t
RegisterComponent() {
    return Component<C>::id;
}

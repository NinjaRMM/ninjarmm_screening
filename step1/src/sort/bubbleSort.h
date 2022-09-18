#pragma once

#include <cstddef>
#include <cstdint>
#include <algorithm>

//--------------------------------------
template <typename T>
void
bubbleSort(T *buffer, size_t size) {
    for (size_t step = 0; step < size; ++step) {
        for (size_t i = 0; i < size - step; ++i) {
            if (buffer[i] > buffer[i + 1]) {
                std::swap(buffer[i], buffer[i + 1]);
            }
        }
    }
}

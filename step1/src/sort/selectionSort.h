#pragma once

#pragma once

#include <cstddef>
#include <cstdint>
#include <algorithm>

//--------------------------------------
template <typename T>
void
selectionSort(T *buffer[], size_t size) {
    for (size_t step = 0; step < size - 1; ++step) {
        size_t minIndex = step;
        for (size_t i = step + 1; i < size; ++i) {
            if (buffer[i] < buffer[minIndex]) {
                minIndex = i;
            }
        }
        std::swap(buffer[minIndex], buffer[step]);
    }
}

#pragma once

#include <cstddef>
#include <cstdint>

//--------------------------------------
template <typename T>
void
shellSort(T *buffer, size_t size) {
    size_t j;

    for (size_t interval = size >> 1; interval > 0; interval >>= 1) {
        for (size_t i = interval; i < size; ++i) {
            T temp = buffer[i];
            for (j = i; (j >= interval) && (buffer[j - interval] > temp); j -= interval) {
                buffer[j] = buffer[j - interval];
            }
            buffer[j] = temp;
        }
    }
}

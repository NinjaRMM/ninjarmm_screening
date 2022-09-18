#pragma once

template <typename T>
void
InsertionSort(T *buffer, size_t length) {
    size_t  i, j;

    for (i = 1; i < length; i++) {
        j = i;
        while ((j > 0) && (buffer[j - 1] > buffer[j])) {
            std::swap(buffer[j], buffer[j - 1]);
            --j;
        }
    }
}

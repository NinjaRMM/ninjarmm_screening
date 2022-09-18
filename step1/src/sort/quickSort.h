#pragma once

#include <cstddef>
#include <cstdint>
#include <algorithm>

//--------------------------------------
template <typename T>
static inline int32_t
partition(T *buffer, int32_t start, int32_t end) {
    int32_t i = start;
    int32_t j = start;

    const T &pivot = buffer[end];
    while (i <= end) {
        if(buffer[i] > pivot){
            ++i;
        }
        else {
            std::swap(buffer[i], buffer[j]);
            ++i;
            ++j;
        }
    }

    return j - 1;
}

//--------------------------------------
template <typename T>
static inline int32_t
partitionB(T *buffer, int32_t start, int32_t end) {
    const T &pivot = buffer[start];

    int32_t count = 0;
    for (int32_t i = start + 1; i <= end; i++) {
        if (buffer[i] <= pivot)
            ++count;
    }

    // Giving pivot element its correct position
    int32_t pivotIndex = start + count;
    swap(buffer[pivotIndex], buffer[start]);

    // Sorting left and right parts of the pivot element
    int32_t i = start;
    int32_t j = end;

    while (i < pivotIndex && j > pivotIndex) {
        while (buffer[i] <= pivot) {
            i++;
        }

        while (buffer[j] > pivot) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(buffer[i++], buffer[j--]);
        }
    }

    return pivotIndex;
}

//--------------------------------------
template <typename T>
void
QuickSort(T *buffer, int32_t start, int32_t end) {
    if (start >= end)
        return;

    int32_t pivot = partition(buffer, start, end);

    QuickSort(buffer, start,     pivot - 1);
    QuickSort(buffer, pivot + 1, end);
}

//--------------------------------------
template <typename T>
void
QuickSort(T *buffer, size_t size) {
    QuickSort(buffer, 0, int32_t(size - 1));
}

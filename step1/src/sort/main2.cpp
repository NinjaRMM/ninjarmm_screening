#include <vector>
#include <chrono>
#include <random>
#include <iostream>
//--
#include "quickSort.h"
#include "insertionSort.h"
#include "radixSort.h"
#include "shellSort.h"
#include "bubbleSort.h"
#include "selectionSort.h"

using namespace std::chrono;

//--------------------------------------
template <typename T>
static inline uint32_t
GetTime(T time) {
    return uint32_t(duration_cast<nanoseconds>(time).count());
}

//--------------------------------------
static inline high_resolution_clock::time_point
Now() {
    return high_resolution_clock::now();
}

//--------------------------------------
static const int size = 32;

//--------------------------------------
int
main() {
    std::vector<float>    values;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> random(-12345678, 12345678);

    values.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        auto value = random(mt);
        values.push_back(float(value) * 0.123f);
    }

    auto copyQ = values;
    auto timeStampQ = Now();
    QuickSort(copyQ.data(), copyQ.size());
    auto timeQ = Now() - timeStampQ;

    auto copyI = values;
    auto timeStampI = Now();
    InsertionSort(copyI.data(), copyI.size());
    auto timeI = Now() - timeStampI;

    auto copyR = values;
    auto timeStampR = Now();
    radixSort(copyR.data(), copyR.size());
    auto timeR = Now() - timeStampR;

    auto copyS = values;
    auto timeStampS = Now();
    InsertionSort(copyS.data(), copyS.size());
    auto timeS = Now() - timeStampS;

    auto copyB = values;
    auto timeStampB = Now();
    InsertionSort(copyB.data(), copyB.size());
    auto timeB = Now() - timeStampB;

    auto copyS2 = values;
    auto timeStampS2 = Now();
    InsertionSort(copyS2.data(), copyS2.size());
    auto timeS2 = Now() - timeStampS2;

    std::cout << std::boolalpha;
    std::cout << "Are Equal: " << (copyQ == copyI) << ", " << (copyQ == copyR) << ", " << (copyQ == copyS) << ", " << (copyQ == copyB) << ", " << (copyQ == copyS2) << "\n";
    std::cout << "QuickSort:     " << GetTime(timeQ)  << "\n";
    std::cout << "InsertionSort: " << GetTime(timeI)  << "\n";
    std::cout << "RadixSort:     " << GetTime(timeR)  << "\n";
    std::cout << "ShellSort:     " << GetTime(timeS)  << "\n";
    std::cout << "BubbleSort:    " << GetTime(timeB)  << "\n";
    std::cout << "SelectionSort: " << GetTime(timeS2) << "\n";

    return 0;
}

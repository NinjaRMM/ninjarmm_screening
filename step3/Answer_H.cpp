#include <iostream>
#include <algorithm>
using namespace std;

template <typename T>
bool IsInBounds(T  value,  T  from , T   to)
{
    return (value >= from && value <= to);
 }
 int main() {
   
    uint32_t httpResonseCode = 501;
    cout << std::boolalpha << IsInBounds<uint32_t>(httpResonseCode, 500, 599); 
     
};
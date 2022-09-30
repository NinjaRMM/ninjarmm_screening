 
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
  
template <typename F>
int ContainsTheString(const F* func, vector<std::string> &v)
{
   return  (std::count_if(v.cbegin(), v.cend(), (*func)));;
 }


int main() {
     auto theStrings = vector<std::string>{ "One", "test" ,"two",  "test" , "more", "test" };
     auto lcall = [](const std::string& tested) { return tested == "test"; };
     int cnt = ContainsTheString(&lcall, theStrings);
     cout << "Found :" << cnt << " times in -theStrings-  vector.... \n ";
     getchar();



};


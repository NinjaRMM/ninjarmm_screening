/*

NINJARMM Code Challenge

Please review the below code.
We do expect you to execute this code.

This file contains the 'main' function. Program execution begins and ends there.
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>

//using namespace std;

typedef struct SampleObject1
{
	int getResult(const int& param1, const int& param2, const int& param3) const
	{
		return (param1 + param2 + param3 > 0) ? 1 : 0;
	}
}OBJ_INT;

OBJ_INT objInt;

/*template <typename... Args, typename T = typename std::common_type<Args...>::type>
	T sum(Args&&... args) {
	std::initializer_list<T> l{ args... };
	int val = std::accumulate(l.begin(), l.end(), T{});
	//auto value = y(std::forward<Args>(args)...);


	using unused = int[];

	(void)unused { 0, (std::cout << args << ", ", 0)... };

	std::cout << std::endl;





	for (const auto& p : { args... })
	{
		std::cout << p << "\n";
	}



	auto&& range = { args... };
	auto iter = range.begin();     // const int*
	auto end = range.end();       // const int*

	//for (; iter != end; ++iter) {
	//	auto& arg = *iter;         // const int&
	//}
	return 1;
}
*/

template<bool b, typename T1, typename T2>
decltype(auto) replace_if(T1&& t1, T2&& t2)
{
	if constexpr (b)
		return std::forward<T1>(t1);
	else
		return std::forward<T2>(t2);
}

template<typename... Args>
void f1(Args&&... args)
{
	(std::cout << ... << args) << std::endl;
}

template<typename T, typename... Args, size_t... I>
decltype(auto) replace_last_impl(std::index_sequence<I...>, T&& t, Args&&... args)
{
	return f1(replace_if<sizeof...(Args) - 1 == I>(std::forward<T>(t), std::forward<Args>(args))...);
}

template<typename T, typename... Args>
decltype(auto) replace_last(T&& t, Args&&... args)
{
	return replace_last_impl(std::index_sequence_for<Args...>{}, std::forward<T>(t), std::forward<Args>(args)...);
}


int main()
{
	//auto val = sum(1, 2, 3);    // Ok, all ints

	f1(1, 2, 3);  // 123
	replace_last("three", 1, 2, 3);  // 12three

}

/*
template<typename Less, typename... T>
constexpr auto& min(Less&& less, const T& ...values)
{

	std::initializer_list<T> l{ args... };

	auto const compare_wrapped =
		[&less](auto const&a, auto const& b) {
		return std::forward<Less>(less)(a.get(), b.get());
	};

	auto const list = { std::ref(values)..., };
	auto const smallest =
		std::min_element(list.begin(), list.end(), compare_wrapped);

	return smallest->get();
}

int main()
{
	auto const a = 10;
	auto const b = 20;
	auto const c = 30;

	auto const& lowest = min(std::less<int>(), a, b, c);

	return &lowest != &b;
}*/

//int main()
//{
//	cout << sum(objInt, 1, 2, 3, 4, 5, 6, 7) << "\n";
//}

/*
typedef struct SampleObject1
{
	int getResult(const int& param1, const int& param2, const int& param3) const
	{
		return (param1 + param2 + param3 > 0) ? 1 : 0;
	}
}OBJ_INT;

typedef struct SampleObject2
{
	std::string getResult(const int& param1, const int& param2, const int& param3) const
	{
		return (param1 + param2 + param3 > 0) ? "true" : "false";
	}

}OBJ_STR;

OBJ_INT objInt;
OBJ_STR objStr;

*/
//<<<<<<<< START OF EDITABLE SECTION OF CODE <<<<<<<<<<<

//Here's a template function that can accept a sample object and 3 integer parameters. It checks if the sum of the integer params is positive or not.
//The third integer param is optional so it's declared as a default param
//Inside the function is some existing code that needs to exist, but not relevant to this exercise. 
//template <typename T, typename U>
//T checkIfPositive(const U& u, const int& param1, const int& param2, int param3 = 0)
//{

	// Assume there's already some existing code in here, some business logic that we are not interested in this exercise. 
	/*

	SOME EXISTING CODE

	*/

	//return u.getResult(param1, param2, param3);
//}

//<<<<<<<< END OF EDITABLE SECTION OF CODE <<<<<<<<<<<
/*
int main()
{
	//<<<<<<<< START OF NON-EDITABLE SECTION OF CODE <<<<<<<<<<<

	//The following calls are already coded in a lot of places throughout the project so you can't change these
	int iResult = checkIfPositive<bool, OBJ_INT>(objInt, 1, 2);
	std::cout << iResult << std::endl;
	iResult = checkIfPositive<bool, OBJ_INT>(objInt, 1, -2);
	std::cout << iResult << std::endl;
	iResult = checkIfPositive<bool, OBJ_INT>(objInt, 1, -2, 3);
	std::cout << iResult << std::endl;

	std::string sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2);
	std::cout << sResult << std::endl;
	sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, -2);
	std::cout << sResult << std::endl;
	sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, -2, 3);
	std::cout << sResult << std::endl;

	//<<<<<<<< END OF NON-EDITABLE SECTION OF CODE <<<<<<<<<<<

	//Now here's a new requirement where you need to get the total as an output param of checkIfPositive() template function
	//You can't touch the previous calls to checkIfPositive because they are used in a lot of different places throughout the code base.
	//How are you going to solve this problem?
	//You can only change the code inside the EDITABLE SECTION OF CODE
	//Hint, the following calls should now be valid moving forward:

	int total = 0;

	iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2);
	std::cout << iResult <<  std::endl;

	iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, -3);
	std::cout << iResult << std::endl;

	iResult = checkIfPositive<int, OBJ_INT>(objInt, 1, 2, 3, total); //<-- Total is output param
	std::cout << iResult << " " << total << std::endl;

	sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2);
	std::cout << sResult <<  std::endl;

	sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, -3);
	std::cout << sResult <<  std::endl;

	sResult = checkIfPositive<std::string, OBJ_STR>(objStr, 1, 2, 3, total); //<-- Total is output param
	std::cout << sResult << " " << total << std::endl;


	return 0;

}
*/

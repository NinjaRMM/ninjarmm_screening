// question1.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#pragma region  Static variable in functions

double addCounter()
{
	static double n = 0;
	return ++n;
}

double getSomeCalculation(double somevar1, double somevar2)
{
	// to avoid stack data we can use static const data.
	static const  double PI = 3.1428; //explicitly static
	return somevar1 * PI * somevar2;
}

#pragma endregion  Static variable in functions

#pragma region  Static Methods in class

class WithStaticMethods
{
public:
	static void CallMeDirectly()
	{
		std::cout << " i am in a Static Methods" << std::endl;
	}
};
#pragma endregion Static Methods in class


#pragma region  Static member Variable in class

class WithStaticMemberVariable
{
public:
	static int globalvar;
	WithStaticMemberVariable() = delete;

};
int WithStaticMemberVariable::globalvar = 245;

#pragma endregion Static member Variable in class

#pragma region  Static member Variable in class

static int privateMethod1()
{
	return 1;
}
static int privatevar = 1;
#pragma endregion Static member Variable in class

int main()
{
	double v;
	v = addCounter();
	_ASSERT(v == 1);
	v = addCounter();
	_ASSERT(v == 2);
	v = getSomeCalculation(3, 6);

	WithStaticMethods::CallMeDirectly();

	std::cout << "static variable value = " << WithStaticMemberVariable::globalvar;

}



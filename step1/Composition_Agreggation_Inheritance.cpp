#include <string>
#include <sstream>
#include <iostream> 

/////////////////////////////////////
//
// Inheritance Example
//
//

class A 
	{
	public:

		int x;
		int y;

		A(int a)
			{
			x = a;
			y = 1;
			}

		int GetX(void)
			{
			x = x + GetY();		
			return x;
			}

		virtual int GetY(void)
			{
			return y;
			}

	};

// Complex class
class B : public A
	{

	public:

		B(int a) : A(a)
			{
			}

		int GetY(void)
			{
			return 25;	
			}

		void display()
			{
			std::cout << "Data in member object of "
				<< "class A in class B = " << A::GetX();
			}
	};


/////////////////////////////////////
//
// Composition Example
//
//

class C
	{
	public:

		int x;
		int y;
		int a;

		C(void)
			{
			y = 1;
			}

		void SetValue(int a)
			{
			this->a = a;
			}

		int GetX(void)
			{
			x = x + GetY();		
			return x;
			}

		int GetY(void)
			{
			return y;
			}

	};

// Complex class
class D 
	{

	public:

		C ObjC;

		D(int a) 
			{
			ObjC.SetValue(a);
			}

		void display()
			{
			std::cout << "Data in member object of "
				<< "class A in class B = " << ObjC.GetX();
			}
	};



/////////////////////////////////////
//
// Aggregation Example
//
//
class E 
	{
	public:

		int x;
		int y;

		E(int a)
			{
			x = a;
			y = 1;
			}

		int GetX(void)
			{
			x = x + GetY();		
			return x;
			}

		int GetY(void)
			{
			return y;
			}

	};

// Complex class
class F 
	{
	private:

		E *APtr;

	public:

		F(int a, E *APtr) 
			{
			this->APtr = APtr;
			}

		void display()
			{
			std::cout << "Data in member object of "
				<< "class A in class B = " << APtr->GetX();
			}
	};


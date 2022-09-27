// forward example
#include <utility>      // std::forward
#include <iostream>     // std::cout

class String {
private:
	char* name;

public:
	String() {
		this->name = nullptr;
		std::cout << "string initialization" << std::endl;
	}

	String(const char* name) {
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
		std::cout << "string initialization :" << name << std::endl;

	}

	// copy constructor
	String(const String& other) {
		this->name = new char[strlen(other.name) + 1];
		strcpy(this->name, other.name);
		std::cout << "string copy constructor :" << name << std::endl;

	}

	// copy assignment
	String& operator=(const String& rhs) {

		// self-asignment check
		if (this != &rhs) {


			// delete old data
			if (this->name) {
				delete[] this->name;
			}

			this->name = new char[strlen(rhs.name) + 1];
			strcpy(this->name, rhs.name);
			std::cout << "string copy assignment :" << name << std::endl;

		}

		return *this;
	}
	// move constructor
	String(String&& other) {
		this->name = other.name;
		other.name = nullptr;
		std::cout << "string move constructor :" << name << std::endl;

	}

	// move assignment
	String& operator=(String&& rhs) {
		// self-asignment check
		if (this != &rhs) {


			// delete old data
			if (this->name) {
				delete[] this->name;
			}

			this->name = rhs.name;
			rhs.name = nullptr;
			std::cout << "string move assignment :" << name << std::endl;

		}

		return *this;
	}
	const char* GetName() const
	{
		return this->name;
	}
	~String() {
		delete[] this->name;
	}
};



String foo()
{

	String something = "avalue";
	return something;
}


int main() {

	String rvo = foo();

	return 1;
}
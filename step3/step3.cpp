// Step3.cpp :  Compiled and tested on VC++
//

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <tuple>
#include <utility>

using namespace std;


class Job
{
public:

	Job(string name, string descr, int hours) : jobName(name), description(descr), numHours(hours) {}

	// Get the name of the job
	string getName() 
	{
		return jobName;
	}

	// Get the description of the job.
	string getDescription()
	{
		return description;
	}

	// Get the number of hours required by the job
	int getNumHours()
	{
		return numHours;
	}

	// A DoWork function that prints out “My work involves ” + the description.
	string DoWork()
	{
		return string("My work involves " + description);
	}

private:

	string jobName;
	string description;
	int numHours;
};

class Programmer : public Job
{
public:
	Programmer(int hours) : Job(string("Programmer"), string("programming software"), hours)
	{}
};

class Pilot : public Job
{
public:
	Pilot(int hours) : Job(string("Pilot"), string("flying Boeings"), hours)
	{}
};

// Variadic Template and Meta Programming Example
template<typename P>
class MyTupleParser
{
public:

    MyTupleParser(const P& t) : float_sum(0.0), other_num(0)
    {
        processTuple(t);
        std::cout << ">  " << float_sum << ", "
            << concat_string << ", "
            << other_num << std::endl;
    }

private:

    template <std::size_t I = 0, typename... T>
    typename std::enable_if<I == sizeof...(T), void>::type
    processTuple(const std::tuple<T...>& tup)
    {
        // Last one called
        return;
    }

    template <std::size_t I = 0, typename... T>
    typename std::enable_if<(I < sizeof...(T)), void>::type
    processTuple(const std::tuple<T...>& tup)
    {
        processComp(std::get<I>(tup));

        // Go to next element
        processTuple<I + 1>(tup);
    }

    template <typename T>
    typename std::enable_if<std::is_convertible<T, const std::string>::value>::type
    processComp(const T& s)
    {
        if (std::is_same<char*, T>::value) // To handle the passing of a nullptr to a char*
        {
            if (*(reinterpret_cast<const char*>(&s)) == NULL)
            {
                cout << "NULL passed as a char*" << endl;
                other_num++;
            }
            return;
        }

        cout << "string " << s << endl;
        if (concat_string.empty())
            concat_string = s;
        else
        {
            concat_string += ' ';
            concat_string += s;
        }
    }

    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value
                            and !std::is_same<bool, T>::value>::type
    processComp(const T& t)
    {
        cout << "number " << t << endl;
        float_sum += t;
    }

    template <typename T>
    typename std::enable_if<!std::is_arithmetic<T>::value 
                            and !std::is_convertible<T, const std::string>::value 
                            or std::is_same<bool, T>::value
                            or std::is_null_pointer<T>::value>::type
    processComp(const T& t)
    {
        cout << typeid(T).name() << endl;
        other_num++;
    }

    std::string concat_string;
    float float_sum;
    int other_num;
};

template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
IsInBounds(T httpResponseCode, T low, T high)
{
	if (httpResponseCode < low || httpResponseCode > high)
		return false;

	return true;
}

int ContainsTheString(function<int(const std::string&)> func, const vector<std::string> & theStrings)
{
	int count = 0;

	for (int i = 0; i < theStrings.size(); i++)
		count += func(theStrings[i]);

	return count;
}


int main()
{
	vector < unique_ptr<Job> > workers;

	workers.push_back(move(unique_ptr<Job>(new Programmer(20))));
	workers.push_back(move(unique_ptr<Job>(new Programmer(40))));
	workers.push_back(move(unique_ptr<Job>(new Programmer(50))));

	workers.push_back(move(unique_ptr<Job>(new Pilot(30))));
	workers.push_back(move(unique_ptr<Job>(new Pilot(40))));
	workers.push_back(move(unique_ptr<Job>(new Pilot(50))));

	for (int i = 0; i < workers.size(); i++)
	{
		cout << "Job Title: " << workers[i]->getName()
			<< ", Description: " << workers[i]->getDescription()
			<< ", Hours: " << workers[i]->getNumHours()
			<< ", Extended Description: " << workers[i]->DoWork() 
			<< endl;
	}

	uint32_t httpResponseCode(550), httpResponseCode1(400), httpResponseCode2(650);

	cout << std::boolalpha;

	cout << endl << "httpResponseCode: "
		<< httpResponseCode << ". Is it within range 500 to 599 ? " 
		<< IsInBounds<uint32_t>(httpResponseCode, 500, 599) << endl;

	cout << "httpResponseCode: "
		<< httpResponseCode1 << ". Is it within range 500 to 599 ? "
		<< IsInBounds<uint32_t>(httpResponseCode1, 500, 599) << endl;

	cout << "httpResponseCode: "
		<< httpResponseCode2 << ". Is it within range 500 to 599 ? "
		<< IsInBounds<uint32_t>(httpResponseCode2, 500, 599) << endl;

	auto theStrings = vector<std::string>{ "one", "two", "test" };
	auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
	cout << endl << "ContainsTheString result: " << count << endl;

	theStrings = vector<std::string>{ "one", "two", "test", "test" };
	count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
	cout << "ContainsTheString result: " << count << endl;

	theStrings = vector<std::string>{ "one", "two", "three", "four" };
	count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
	cout << "ContainsTheString result: " << count << endl;

  struct MyStruct {};
  using Tuple = std::tuple<int32_t, bool, std::string, float, const char*>;
  using Tuple1 = std::tuple<char, bool, std::string, float, const char*, MyStruct>;
  using Tuple2 = std::tuple<std::string, std::vector<int>>;
  using Tuple3 = std::tuple<long, bool, std::string, float, const char*, MyStruct, char*>;

  cout << endl;

  Tuple t{ 10, true, "Modern", 2.3f, "C++" };
  MyTupleParser<Tuple&> mtp(t);

  Tuple1 t1{ 100, false, "Modern", 2.34, "C++", MyStruct() };
  MyTupleParser<Tuple1&> mtp1(t1);

  Tuple2 t2{ "Testing", std::vector<int>(10, 1) };
  MyTupleParser<Tuple2&> mtp2(t2);

  Tuple3 t3{ 100, false, "Modern", 2.34, "C++", MyStruct(), nullptr};
  MyTupleParser<Tuple3&> mtp3(t3);
}

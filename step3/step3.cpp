
#include <iostream>
#include <vector>
#include <memory>


class Job
{
public:
	virtual ~Job()
	{
		std::cout << "free parent job " << std::endl;
	}
	virtual  std::string GetName() const = 0;
	virtual  std::string GetDescription() const = 0;
	virtual  int GetNumberHoursRequired() const = 0;
	void DoWork() const
	{
		std::cout << "My work involves " + GetDescription() << std::endl;
	}



protected:

	//std::string name;
};

class Programmer : public Job
{
public:
	Programmer()
	{
	}
	~Programmer()
	{
		std::cout << "free " << GetName() << std::endl;

	}
	virtual  std::string GetName() const override {
		return "Programmer";
	}
	virtual  std::string GetDescription() const override {
		return "produce computer programs";
	}
	virtual  int GetNumberHoursRequired() const override {
		return 8;
	}
};

class Pilot : public Job
{
public:
	Pilot()
	{

	}
	~Pilot()
	{
		std::cout << "free " << GetName() << std::endl;

	}
	virtual  std::string GetName() const override {
		return "Pilot";
	}
	virtual  std::string GetDescription() const override {
		return "formulating a flight plan taking into accordance aircraft performance, altitude, fuel, and weather and flying the plane";
	}
	virtual  int GetNumberHoursRequired() const override {
		return 10;
	}
};
template<typename Functor>
uint32_t ContainsTheString(Functor functor,const std::vector<std::string>&list)
{
	uint32_t count = 0;
	for (const auto& item : list)
		if (functor(item))
			count++;

	return count;
}
template< class T >
constexpr bool IsInBounds(T value, T min, T max) noexcept
{
	return value >= min && value <= 599;
}

int main()
{
	
	std::vector<std::shared_ptr<Job>> jobs;

	jobs.push_back(std::make_shared<Programmer>());
	jobs.push_back(std::make_shared<Pilot>());

	for (const auto& job : jobs)
	{
		job->DoWork();
	}

	uint32_t httpResonseCode = 501;
	if (IsInBounds<uint32_t>(httpResonseCode, 500, 599))
	{

	}


	auto theStrings =std::vector<std::string>{ "one", "two",  "test"  };

	auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);


}


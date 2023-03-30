#include "Job.h"
//
#include <iostream>
//
#include <cassert>
using std::unique_ptr;
using std::cout; using std::endl;

namespace ninjaone_screening {

std::unique_ptr<Job::Printer> Job::s_printer = std::unique_ptr<Job::Printer>(new Job::Printer());

const std::string Programmer::sk_programmer_job_name = "Programmer";
const std::string Programmer::sk_programmer_job_description = "creating and maintaining code";

const std::string Pilot::sk_pilot_job_name = "Pilot";
const std::string Pilot::sk_pilot_job_description = "flying aircraft";

Job::Job()
{
}

void Job::DoWork() const
{
	*s_printer << "My work involves " << description();
}

void Job::set_printer(Printer* printer)
{
	assert(printer);
	s_printer = unique_ptr<Printer>(printer);
}

Job::Printer::Printer() :m_ostream(&cout)
{
}

Job::Printer& Job::Printer::operator<<(const std::string& value)
{
	*m_ostream << value;
	return *this;
}

//Programmer Functions
const std::string& Programmer::name() const
{
	return sk_programmer_job_name;
}

const std::string& Programmer::description() const
{
	return sk_programmer_job_description;
}

int Programmer::hours() const
{
	return 50;
}

//Pilot Functions
const std::string& Pilot::name() const
{
	return sk_pilot_job_name;
}

const std::string& Pilot::description() const
{
	return sk_pilot_job_description;
}

int Pilot::hours() const
{
	return 40;
}

void exercise_job_classes(const std::vector<std::unique_ptr<Job>>& jobs)
{
	for (auto& job : jobs) {
		cout << "Job Name: " << job->name() << endl;
		cout << "Job Description: " << job->description() << endl;
		cout << "Job Hours: " << job->hours() << endl;
		job->DoWork();
		cout << endl;
	}
}

} //ninjaone_screening

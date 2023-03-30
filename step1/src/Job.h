#ifndef NINJAONESCREENING_JOB_H
#define NINJAONESCREENING_JOB_H

#include <string>
//
#include <memory>
//
#include <iosfwd>
//
#include <vector>

namespace ninjaone_screening{
class Job;
void exercise_job_classes(const std::vector<std::unique_ptr<Job>>& jobs);
/*
 * Represents a profession.
 * Subclasses inherit from this to provide details about 
 * a specific profession.
 */
class Job {
public:
	class Printer;

	Job();
	//Returns the name of the job.
	virtual const std::string& name()const = 0;

	//Returns the description of the job.
	virtual const std::string& description()const = 0;

	//Returns the number of hours required by the job.
	virtual int hours()const = 0;

	//uses the Job::Printer to output “My work involves ” + the description
	virtual void DoWork()const;

	//Printer object is in charge of outputting data
	//Default printer prints to cout
	//Takes ownership of the printer ptr
	void set_printer(Printer* printer);

	//This class is created to facilitate unit tests
	class Printer {
	public:
		//Default constructor outputs to cout
		//This can be enhanced to take other ostreams
		//We do not default cout here because that would require
		//iostream be declared in this header and we dont need it
		Printer();

		//This is not templated because you cannot have virtual template functions
		virtual Job::Printer& operator<<(const std::string& value);
		
		//So supplied derived classes are properly deleted
		virtual ~Printer() = default;
	private:
		//Where output is printed
		//We cannot take ownership of this reference as it may be cout
		std::ostream* m_ostream;
	};

	virtual ~Job() = default;
protected:
	//For now lets make the printer static. so we don't have to allocate one for each instance.
	//The printer class solely exists for unit testing not for advanced dynamic output capabilities
	//such an enhancement could be done later.
	static std::unique_ptr<Printer> s_printer;
};


class Programmer : public Job{
public:
	const std::string& name()const override;

	const std::string& description()const override;

	int hours()const override;
private:
	//s: static. k: const. prefixes can be adapted to whatever coding style ninjaone uses
	static const std::string sk_programmer_job_name;
	static const std::string sk_programmer_job_description;
};



class Pilot : public Job {
public:
	const std::string& name()const override;

	const std::string& description()const override;

	int hours()const override;
private:
	//s: static. k: const. prefixes can be adapted to whatever coding style ninjaone uses
	static const std::string sk_pilot_job_name;
	static const std::string sk_pilot_job_description;
};

} //namespace ninja_one_screening

#endif //NINJAONESCREENING_JOB_H

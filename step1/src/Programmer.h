#ifndef PROGRAMMER_H_
#define PROGRAMMER_H_

#include "Job.h"
namespace ninjans
{
	/**
	 *  Class Programmer
	 */
	class Programmer: public Job
	{
	public:
		Programmer();
		virtual ~Programmer();
		virtual std::string Work() const override; // Returns a string about what it does for work.
		virtual bool WriteCode() const; //Specialization in child class.
	};
}
#endif /* PROGRAMMER_H_ */

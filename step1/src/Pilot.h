#ifndef PILOT_H_
#define PILOT_H_

#include "Job.h"
namespace ninjans
{
	class Pilot: public Job
	{
	public:
		Pilot();
		virtual ~Pilot();
		virtual std::string Work() const override; // Returns a string about what it does for work.
		virtual bool Fly() const; //Specialization in child class.
	};

} /* namespace ninjans */

#endif /* PILOT_H_ */

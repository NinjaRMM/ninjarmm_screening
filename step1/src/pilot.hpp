#include <iostream>
#include <string>
using namespace std;

#include "job.hpp"

/* Header file for the Pilot class.
 *
 * A subclass of the Job class, the Pilot class has its name, 
 * description, and job hours set, ready to echo back.
 */


class Pilot : public Job {
	public :
		Pilot();
		~Pilot() {};
		virtual string jobname();
		virtual string jobdesc();
		virtual int jobhours();

	private :
		string jobtitle;
		string jobdescription;
		int hoursforjob;
};


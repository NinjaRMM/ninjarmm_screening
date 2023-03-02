#include <iostream>
#include <string>
using namespace std;

#include "job.hpp"

/* Header file for the Job class.
 *
 * The Job class takes in a job name, description, and hours.
 * On demand, it does the work (i.e., repeats back the job description).
 */


class Programmer : public Job {
	public :
		Programmer();
		~Programmer() {};
		virtual string jobname();
		virtual string jobdesc();
		virtual int jobhours();

	private :
		string jobtitle;
		string jobdescription;
		int hoursforjob;
};

#include <iostream>
#include <string>
using namespace std;

#ifndef JOB_HPP
#define JOB_HPP

/* Header file for the Job class.
 *
 * The Job class takes in a job name, description, and hours.
 * On demand, it does the work (i.e., repeats back the job description).
 *
 * In addition, it takes an HTTP response value and determines whether it is in the range expected.
 */


class Job {
	public : 
		Job() {};
		virtual ~Job() {};

		void doWork();
		virtual string jobname() = 0;
		virtual string jobdesc() = 0;
		virtual int jobhours() = 0;

};

#endif // JOB_HPP

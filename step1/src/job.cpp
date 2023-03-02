#include "job.hpp"



void Job::doWork() {
	string 	name_of_job 	= jobname();
	string 	jobdescription 	= jobdesc();
	int 	hours			= jobhours();

	if (name_of_job == "" || jobdescription == "" || hours == -1 ) {
		return;
	}
	cout << "My job involves " << jobdescription << '\n';
};


	


#include "programmer.hpp"


Programmer::Programmer() {
	jobtitle 		= "programmer";
	jobdescription 	= "solving puzzles";
	hoursforjob		= 40;
}

string Programmer::jobname() {
	return jobtitle;
}

string Programmer::jobdesc() {
	return jobdescription;
}

int Programmer::jobhours() {
	return hoursforjob;
}
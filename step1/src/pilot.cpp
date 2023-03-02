
#include "pilot.hpp"


Pilot::Pilot() {
	jobtitle 		= "pilot";
	jobdescription 	= "flying an airplane";
	hoursforjob		= 56;
}

string Pilot::jobname() {
	return jobtitle;
}

string Pilot::jobdesc() {
	return jobdescription;
}

int Pilot::jobhours() {
	return hoursforjob;
}
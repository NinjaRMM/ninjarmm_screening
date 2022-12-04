#ifndef __JOB_H
#define __JOB_H

#include <string>

class Job {
    protected:
        std::string name;
        std::string description;
        int hours;

    public:
        std::string Name();
        std::string Description();
        int Hours();
        void DoWork();
};

#endif

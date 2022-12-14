#include "job.h"
#include "ui_job.h"

Job::Job(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Job)
{
    ui->setupUi(this);
}

class Programmer : public Job {
    public:
    Programmer(){};
    ~Programmer(){};

    QString getJobName(){
        return ("");
    }
    QString getJobDescription(){
        return ("");
    }
    QString getJobHours(){
        return ("");
    }
};

class Pilot : public Job {
    public:
    Pilot(){};
    ~Pilot(){};
};

Job::~Job()
{
    delete ui;
}


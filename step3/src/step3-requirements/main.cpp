#include "job.h"

#include <QApplication>

/*
 * Class job
 * - Get the name of the job
 * - Get the description of the job
 * - Get the number of hours required by the job
 * - A doWork function that prints out "My work involves" + the description
*/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Job w;
    w.show();
    return a.exec();
}

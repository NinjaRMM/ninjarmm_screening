#ifndef JOB_H
#define JOB_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Job; }
QT_END_NAMESPACE

class Job : public QMainWindow
{
    Q_OBJECT

public:
    Job(QWidget *parent = nullptr);
    ~Job();

private:
    Ui::Job *ui;
};
#endif // JOB_H

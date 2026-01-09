#ifndef ADD_TO_DO_TASK_H
#define ADD_TO_DO_TASK_H

#include <QDialog>


namespace Ui {
class add_to_do_task;
}

class add_to_do_task : public QDialog
{
    Q_OBJECT

public:
    explicit add_to_do_task(QWidget *parent = nullptr);
    ~add_to_do_task();

private slots:
    void on_cancel_clicked();

    void on_add_clicked();

private:
    Ui::add_to_do_task *ui;

signals:
    void ok();
};

#endif // ADD_TO_DO_TASK_H

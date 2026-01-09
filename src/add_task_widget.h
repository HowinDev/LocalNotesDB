#ifndef ADD_TASK_WIDGET_H
#define ADD_TASK_WIDGET_H

#include <QDialog>

#include<QSqlDatabase>

class MainWindow;
namespace Ui {


class add_task_widget;
}

class add_task_widget : public QDialog
{
    Q_OBJECT

public:
    explicit add_task_widget(MainWindow *mw,QWidget *parent = nullptr);
    ~add_task_widget();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::add_task_widget *ui;
    QSqlDatabase db;
    MainWindow *mainWindow;

signals:
    void ok();

};

#endif // ADD_TASK_WIDGET_H

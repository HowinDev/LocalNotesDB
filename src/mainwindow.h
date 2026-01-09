#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSqlDatabase>
#include<QDebug>
#include<QFrame>
#include<QLayout>
#include"add_task_widget.h"
#include"add_to_do_task.h"
#include<QSqlQuery>
#include<QListWidget>
#include<QStackedWidget>
#include<QWidget>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_add_task_clicked();

    void on_pushButton_save_task_clicked();

    void on_pushButton_delete_task_clicked();





    void on_task_list_itemClicked(QListWidgetItem *item);




    void on_pushButton_to_do_list_clicked();



    void on_pushButton_notes_clicked();

    void on_pushButton_to_do_list_delete_clicked();

    void on_pushButton_to_do_list_add_clicked();

    void on_pushButton_refresh_clicked();

    void on_pushButtonRefresh_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    void loadNotes();
    void to_do();
    int selectedNoteId=-1;
    void loadTodoTasks();




};
#endif // MAINWINDOW_H

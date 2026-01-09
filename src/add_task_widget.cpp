#include "add_task_widget.h"
#include "ui_add_task_widget.h"
#include<QSqlQuery>
#include<QSqlError>
#include "mainwindow.h"
#include<QTextEdit>
add_task_widget::add_task_widget(MainWindow *mw,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::add_task_widget),mainWindow(mw)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/notes_icon.png"));
    this->setWindowTitle(" ");


}

add_task_widget::~add_task_widget()
{
    delete ui;
}

void add_task_widget::on_pushButton_2_clicked()
{
    this->close();
}


void add_task_widget::on_pushButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");
    if (ui->new_task->toPlainText().isEmpty())
        return;
    else
    {
    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO notes (title, content) "
        "VALUES (:title, :content)"
        );

    query.bindValue(":title", ui->new_task->toPlainText());
    query.bindValue(":content", "");

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return;
    }
    emit ok();
    this->close();
    }


}




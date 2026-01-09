#include "add_to_do_task.h"
#include "ui_add_to_do_task.h"
#include<QSqlQuery>
#include<QSqlError>

add_to_do_task::add_to_do_task(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::add_to_do_task)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/notes_icon.png"));
    this->setWindowTitle(" ");


}

add_to_do_task::~add_to_do_task()
{
    delete ui;
}

void add_to_do_task::on_cancel_clicked()
{
    this->close();
}


void add_to_do_task::on_add_clicked()
{
    QString text = ui->new_task->toPlainText().trimmed();
    if (text.isEmpty())
        return;
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "База данных не открыта!";
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO todo_tasks (task) VALUES (:task)");
    query.bindValue(":task", text);

    if (!query.exec()) {
        qDebug() << "Ошибка INSERT:" << query.lastError().text();
        return;
    }

    emit ok();
    close();
}


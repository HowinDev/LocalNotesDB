#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include<QFrame>
#include<QLayout>
#include<QVBoxLayout>
#include<QLabel>
#include<QTextEdit>
#include"add_task_widget.h"
#include<QStackedWidget>
#include<QWidget>
#include<QIcon>
#include<QStatusBar>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QPSQL","main_connection");
    db.setHostName("192.168.31.81");
    db.setPort(5432);
    db.setDatabaseName("task_manager");
    db.setUserName("postgres");
    db.setPassword("2833");

    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
    } else {
        qDebug() << "Подключение к PostgreSQL успешно!";
    }

    this->setWindowIcon(QIcon(":/icon/notes_icon.png"));
    this->setWindowTitle(" ");






    QSqlQuery query(db);

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS notes ("
            "id SERIAL PRIMARY KEY,"
            "title TEXT NOT NULL,"
            "content TEXT,"
            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
            ")"
            )) {
        qDebug() << "Ошибка создания таблицы:" << query.lastError().text();
    }
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS todo_tasks ("
            "id SERIAL PRIMARY KEY,"
            "task TEXT NOT NULL,"
            "is_done BOOLEAN DEFAULT FALSE,"
            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
            ")"
            )) {
        qDebug() << "Ошибка создания таблицы:" << query.lastError().text();
    }
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS todo_tasks ("
            "id SERIAL PRIMARY KEY,"
            "task TEXT NOT NULL,"
            "is_done BOOLEAN DEFAULT FALSE,"
            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
            ")"
            )) {
        qDebug() << "Ошибка создания таблицы:" << query.lastError().text();
    }
    loadNotes();
    loadTodoTasks();
    ui->stackedWidget->setCurrentIndex(0);
    ui->pushButton_notes->setStyleSheet(
    "QPushButton {"
        "background-color: rgba(200, 200, 200, 0.4);"
        "border: 1px solid #999;"
        "border-radius: 5px;"
        "color: #000;"
        "padding: 5px 10px;"
    "}"
    "QPushButton:pressed {"
        "background-color: rgba(160, 160, 160, 0.8);"
        "}"
    );
    ui->pushButton_to_do_list->setStyleSheet(
        "QPushButton{"
            "background-color: transparent;"
            "border: 1px solid black;"
            "border-radius:5px;"
            "color:black;"
            "}"
    );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_add_task_clicked()
{
    add_task_widget *add_task=new add_task_widget(this);
    add_task->show();
    connect(add_task,&add_task_widget::ok,this,&MainWindow::loadNotes);

}


void MainWindow::on_pushButton_save_task_clicked()
{
    QSqlDatabase db=QSqlDatabase::database("main_connection");
    QString title = ui->title->toPlainText();
    QString content = ui->content->toPlainText();
    QSqlQuery query(db);

    if (selectedNoteId == -1) {

        if(!(ui->title->toPlainText().isEmpty()))
        {
        query.prepare(
            "INSERT INTO notes (title, content) "
            "VALUES (:title, :content)"
            );
        }
    } else {

        query.prepare(
            "UPDATE notes "
            "SET title = :title, content = :content "
            "WHERE id = :id"
            );
        query.bindValue(":id", selectedNoteId);
    }

    query.bindValue(":title", title);
    query.bindValue(":content", content);

    if (!query.exec()) {
        qDebug() << "Ошибка сохранения:" << query.lastError().text();
        return;
    }


    selectedNoteId = -1;
    ui->title->clear();
    ui->content->clear();


    loadNotes();
}

void MainWindow::loadNotes()
{

    ui->task_list->clear();


    QSqlDatabase db = QSqlDatabase::database("main_connection");
    if (!db.isOpen()) {
        qDebug() << "DB не открыта!";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id, title, content FROM notes ORDER BY created_at DESC")) {
        qDebug() << "Ошибка запроса:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int id = query.value("id").toInt();
        QString title = query.value("title").toString();
        QString content = query.value("content").toString();


        QListWidgetItem *item = new QListWidgetItem(title, ui->task_list);


        item->setData(Qt::UserRole, id);
        item->setData(Qt::UserRole + 1, content);

        ui->task_list->addItem(item);
    }
}

void MainWindow::loadTodoTasks()
{
    ui->to_do_list->clear();

    QSqlDatabase db = QSqlDatabase::database("main_connection");
    if (!db.isOpen()) {
        qDebug() << "База данных не открыта";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec(
            "SELECT id, task, is_done "
            "FROM todo_tasks "
            "ORDER BY created_at DESC"
            )) {
        qDebug() << "Ошибка SELECT:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int id = query.value("id").toInt();
        QString task = query.value("task").toString();
        bool isDone = query.value("is_done").toBool();

        QListWidgetItem *item = new QListWidgetItem(task);


        item->setData(Qt::UserRole, id);
        item->setData(Qt::UserRole + 1, isDone);


        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(isDone ? Qt::Checked : Qt::Unchecked);


        if (isDone) {
            item->setForeground(Qt::gray);
        }

        ui->to_do_list->addItem(item);
    }
}

void MainWindow::to_do()
{

}







void MainWindow::on_pushButton_delete_task_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");
    if (!db.isOpen()) {
        qDebug() << "База данных не открыта";
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM notes WHERE id = :id");
    query.bindValue(":id", selectedNoteId);

    if (!query.exec()) {
        qDebug() << "Ошибка удаления:" << query.lastError().text();
        return;
    }


    selectedNoteId = -1;


    ui->title->clear();
    ui->content->clear();


    loadNotes();

}


void MainWindow::on_pushButton_notes_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->pushButton_notes->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(200, 200, 200, 0.4);"
        "border: 1px solid #999;"
        "border-radius: 5px;"
        "color: #000;"
        "padding: 5px 10px;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(160, 160, 160, 0.8);"
        "}"
        );
    ui->pushButton_to_do_list->setStyleSheet(
        "QPushButton{"
        "background-color: transparent;"
        "border: 1px solid black;"
        "border-radius:5px;"
        "color:black;"
        "}"
        );
}





void MainWindow::on_pushButton_to_do_list_add_clicked()
{
    add_to_do_task *add_to_do=new add_to_do_task(this);
    add_to_do->show();
    connect(add_to_do,&add_to_do_task::ok,this,&MainWindow::loadTodoTasks);
}

void MainWindow::on_task_list_itemClicked(QListWidgetItem *item)
{
    if (!item)
        return;


    selectedNoteId = item->data(Qt::UserRole).toInt();

    QString title = item->text();
    QString content = item->data(Qt::UserRole + 1).toString();

    ui->title->setText(title);
    ui->content->setPlainText(content);
}





void MainWindow::on_pushButton_to_do_list_delete_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("main_connection");
    if (!db.isOpen()) {
        qDebug() << "База данных не открыта";
        return;
    }

    QList<int> idsToDelete;


    for (int i = 0; i < ui->to_do_list->count(); ++i) {
        QListWidgetItem *item = ui->to_do_list->item(i);
        if (item->checkState() == Qt::Checked) {
            idsToDelete.append(item->data(Qt::UserRole).toInt());
        }
    }

    if (idsToDelete.isEmpty()) {
        qDebug() << "Нет отмеченных задач для удаления";
        return;
    }


    db.transaction();

    QSqlQuery query(db);
    query.prepare("DELETE FROM todo_tasks WHERE id = :id");

    for (int id : idsToDelete) {
        query.bindValue(":id", id);
        if (!query.exec()) {
            qDebug() << "Ошибка удаления:" << query.lastError().text();
            db.rollback();
            return;
        }
    }

    db.commit();


    loadTodoTasks();
}





void MainWindow::on_pushButton_to_do_list_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->pushButton_to_do_list->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(200, 200, 200, 0.4);"
        "border: 1px solid #999;"
        "border-radius: 5px;"
        "color: #000;"
        "padding: 5px 10px;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(160, 160, 160, 0.8);"
        "}"
        );
    ui->pushButton_notes->setStyleSheet(
        "QPushButton{"
        "background-color: transparent;"
        "border: 1px solid black;"
        "border-radius:5px;"
        "color:black;"
        "}"
        );
}






void MainWindow::on_pushButton_refresh_clicked()
{
    loadTodoTasks();
}


void MainWindow::on_pushButtonRefresh_clicked()
{
    loadNotes();
}


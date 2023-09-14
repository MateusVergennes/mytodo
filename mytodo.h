#ifndef MYTODO_H
#define MYTODO_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtSql>
#include <QFileInfo>//torna o banco de dados acessivel ao usuario,faz um backup do arquivo de recursos para um diretorio que o usuario acessa
#include <QDebug>
#include "editar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MyToDo; }
QT_END_NAMESPACE

class MyToDo : public QMainWindow
{
    Q_OBJECT

public:
    MyToDo(QWidget *parent = nullptr);
    QString home = getenv("HOME");//home do usuario
    void start();
    QString dir = ":/mytodo.db";
    //QString user = home + "/.config/mytodo.db";
    QString user = QCoreApplication::applicationDirPath() + "/mytodo.db";


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    void show_data();

    ~MyToDo();

private slots:
    void on_actionSair_triggered();

    void on_actionSobre_triggered();

    void on_pushButton_clicked();

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::MyToDo *ui;
};
#endif // MYTODO_H

#ifndef EDITAR_H
#define EDITAR_H

#include <QDialog>
#include <QtSql>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class Editar;
}

class Editar : public QDialog
{
    Q_OBJECT

public:
    explicit Editar(QWidget *parent = nullptr, int id = 0, QString todo = NULL);//para essa janela, passaremos da anterior o id e a task o todo clicada
    ~Editar();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Editar *ui;
};

#endif // EDITAR_H

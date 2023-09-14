#include "mytodo.h"
#include "ui_mytodo.h"

MyToDo::MyToDo(QWidget *parent): QMainWindow(parent), ui(new Ui::MyToDo){
    ui->setupUi(this);

    start();

    db.setDatabaseName(user);

    //se ele estiver aberto (arquivo encontrado), senao (falha)
    !db.open() ? qDebug() << "Falha ao Encontrar o Arquivo do Banco: " + user : qDebug() << "Arquivo do Banco Encontrado com Sucesso!";

    show_data();
}

void MyToDo::start(){
    QFile quser(user);

    if(!quser.exists()){//se o arquivo ainda n existir, vai copiar o db para o user
        QFile::copy(":/mytodo.db", user);//gravamos mas precisamos dar algumas permissoes para gravar a frente
        QFile::setPermissions(user, QFile::WriteOwner | QFile::ReadOwner);
        qDebug() << "O arquivo Inicial foi Copiado e as permissões de escrita foram definidas.";//dessa forma nosso bd esta presente no arquivo executavel também, e so vai copiar na 1ª vez
    }
}

void MyToDo::show_data(){
    QSqlQuery query;
    QString sql = "SELECT * FROM todos ORDER BY id DESC";
    query.prepare(sql);

    if(query.exec()){
        int i = 0;

        ui->tableWidget->setColumnCount(3);//3 colunas(id, tarefa, data)
        while(query.next()){
            ui->tableWidget->insertRow(i);//para mostrar os dados da linha

            //setando as colunas (aba mais de cima)
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(query.value(0).toString() ) );//Linha, coluna, valor como construtor //id // o new ja faz pegar o construtor
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(query.value(1).toString() ) );//task
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(query.value(2).toString() ) );//data

            i++;
        }

        //definindo cabecalho
        QStringList headers = {"ID", "Tarefa", "Data/Hora"};
        ui->tableWidget->setHorizontalHeaderLabels(headers);

        //para definir tamanho das Colunas:
        ui->tableWidget->setColumnWidth(0, 60);//coluna 1 com 60 de largura
        ui->tableWidget->setColumnWidth(1, 540);
        ui->tableWidget->setColumnWidth(2, 150);

        while(i <ui->tableWidget->rowCount()){
            ui->tableWidget->removeRow(i);//para excluir todos os itens quando forem removidos da tabela, sem deixar tracos (vai dar um clear na tabela mostrada)
        }

        ui->tableWidget->verticalHeader()->setVisible(false);//para nao ter a numeracao de linha ao lado


    }else{
        qDebug() << "Falha ao Consultar os Dados no Banco de Dados";
    }
}

MyToDo::~MyToDo()
{
    delete ui;
}


void MyToDo::on_actionSair_triggered(){
    close();
}


void MyToDo::on_actionSobre_triggered(){
    QMessageBox::about(this, "Sobre Esse Projeto", "<h2>MyToDo 1.0.0</h2>"
                                                   "<p>Desenvolvido por Mateus Vergennes <br>"
                                                    "Projeto para o Curso de Qt Moderno com C++<br>"
                                                    "Copyright 2008-2023 Matt Systems Inc. </p>"
                                                    "<p><a href='https://google.com.br'>Google</a></p>"
                                                   "<p><i>Isso ai meu Caro.</i></p>");
}


void MyToDo::on_pushButton_clicked(){
    if(ui->lineEdit->text() == ""){
        QMessageBox::warning(this, "Aviso", "Preencha o Campo da Tarefa.");
        return;
    }

    if(!db.isOpen()){
        QMessageBox::warning(this, "Aviso", "Falha ao Conectar ao Banco de Dados");
        return;
    }

    QSqlQuery query;
    QString sql = "INSERT INTO todos (todos) VALUES ( '" + ui->lineEdit->text() + "' )";
    query.prepare(sql);

    if(query.exec()){
        ui->statusbar->showMessage("Dados Inseridos com Sucesso!");
        QTimer::singleShot(2000, [&](){ui->statusbar->showMessage(""); } );
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
        ui->tableWidget->clear();
        show_data();
    }else{
        qDebug() << "Falha ao Inserir Dados";
        qDebug() << sql;
        qDebug() << query.lastError().text();
    }
}

void MyToDo::on_tableWidget_cellClicked(int row, int column){
    column = 0;
    int id = ui->tableWidget->item(row, column)->text().toInt();//pega o a linha que foi clicada, a coluna 0(que é a do id) e como id é int, converte ele
    QString todo = ui->tableWidget->item(row, 1)->text();//para o conteudo da task

    Editar e(this, id, todo);
    e.exec();//para abrir a janela .ui
    show_data();//para quando terminar a execucao da janela editar ou excluir ja atualiza a lista
}


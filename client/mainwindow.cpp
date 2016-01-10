#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <threads.h>


int newconnect(int sock,sockaddr* serv_addr,int size)
{
    return connect(sock,serv_addr,size);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    portno=12008;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(portno);
    int rval=newconnect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));

    if(rval<0)
    {
        this->ui->browser->setText("connect error\n");
        this->ui->browser->append(QString::number(rval));
    }

    if (rval==0)
    {
        this->ui->browser->setText("connect success\n");
    }

    listener *worker=new listener;
    connect(worker,SIGNAL(message(QString)),SLOT(recivecount(QString)));
    worker->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    //this->ui->browser->append(this->ui->input->toPlainText());
    const char* words=this->ui->input->toPlainText().toStdString().c_str();
    write(sockfd,words,255);

}

void MainWindow::recivecount(QString info)
{
    this->ui->browser->append(info);
}




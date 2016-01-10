#ifndef MAINWINDOW_H
#define MAINWINDOW_H


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

extern int sockfd;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void recivecount(QString info);

private:
    Ui::MainWindow *ui;
    int portno;
    struct sockaddr_in serv_addr;


};

#endif // MAINWINDOW_H

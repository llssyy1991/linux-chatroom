#include <QThread>
 #include <unistd.h>
extern int sockfd;
/*
class transmiter:public QObject
{
signals:
    void message(QString);
};
*/

class listener: public QThread
{
 Q_OBJECT
 public:
 void run()
 {
     char buff[256];
     while(1)
     {
         bzero(buff,256);
         read(sockfd,buff,256);
         QString mess(buff);
         emit message(mess);
         sleep(1);
     }
 }

signals:
    void message(QString);

};



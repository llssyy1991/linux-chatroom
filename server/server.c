#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "server.h"
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

//#define optimized

int shareindex=0;
char sharememory[512];
pthread_cond_t  writing=PTHREAD_COND_INITIALIZER;
pthread_cond_t  reading=PTHREAD_COND_INITIALIZER;
bool finishwriting;
int thread_ready=0;                                //how many thread get ready to read
int thread_finished=0;
int threadnum=0;                     // how many child thread has been created
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t threadlock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t readlock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t readfinishlock=PTHREAD_MUTEX_INITIALIZER;
int condition=0;                         //1.writting  0.not writting

int fdvec[20];
int usernum=0;


void main(int argc,char *argv[])
{
     if(argc<2)
    {
    printf("not enough parament");
    return;
    }


    int portnum=atoi(argv[1]);
    struct sockaddr_in client_addr;
    char buffer[256];
    int connectnum;// file descriptor for client file
    bool working=true;
    pthread_t controll;


    int socketnum=setserver(portnum);// serserver function in server.h


    socklen_t client_len=sizeof(client_addr);
#ifdef optimized
    pthread_create(&controll,NULL,(void *)(&controllthread),NULL);
#else
    pthread_create(&controll,NULL,(void *)(&controllthread1),NULL);
#endif
    //connectnum=accept(socketnum,(struct sockaddr *) &client_addr,& client_len);
    printf("portnum%d\n",portnum);

    while(working)
    {
            connectnum=accept(socketnum,(struct sockaddr *) &client_addr,& client_len);
            if(connectnum<0)
            {
                printf("accept error\n");
            }
            if(connectnum>=0)
            {
                printf("accept success: descriptor  %d\n",connectnum);
            }

	    //char buff[256];
	    //read(connectnum,buff,256);
	    //printf("%s",buff);
	    fcntl(connectnum, F_SETFL, O_NONBLOCK);
	    fdvec[usernum]=connectnum;
	    usernum++;
#ifdef optimize
            pthread_t threadwrite;
            pthread_t threadread;
            pthread_mutex_lock(&threadlock);
            pthread_create(&threadwrite,NULL,(void *)(&writeservice),(void *)(&connectnum));//create writting thread
            pthread_create(&threadread,NULL,(void *)(&readservice),(void *)(&connectnum));//create reading thread
            threadnum++;//create reading thread
            pthread_mutex_unlock(&threadlock);
#endif

    }



}





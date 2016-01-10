#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>


extern int shareindex;
extern char sharememory[512];
extern pthread_cond_t writing;
extern pthread_cond_t reading;
extern pthread_mutex_t lock;
extern int thread_ready;
extern int thread_finished;
extern int threadnum;
extern pthread_mutex_t threadlock;
extern pthread_mutex_t readlock;
extern pthread_mutex_t readfinishlock;
extern int condition;
extern int fdvec[20];
extern int usernum;

int setserver(int portnum)
{
    int socketnum,connectnum;// socketnum is file number of owner computer , connectnum is request file number
    struct sockaddr_in server_addr;


    socketnum=socket(AF_INET,SOCK_STREAM,0);
    if(socketnum==-1)
    {
        printf("socket create unsuccessful");
    }
    //socket creation
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(portnum);
    socklen_t serverlen=sizeof(server_addr);
    int check=bind(socketnum,(struct sockaddr *) & server_addr,serverlen);
    //socket binding
    if(check==0)
    {
        printf("bind success\n");
    }

    listen(socketnum,10);

    return socketnum;
}


void* writeservice(void * arg)
{
        int working=1;
        char buff[256];
        int* connectnum=(int*) (arg);
        bzero(buff,256);
        while(working==1)
        {
            // writing process
		int length=read(*connectnum,buff,256);
                pthread_mutex_lock(&lock);
                printf("before writing\n");
            //add writing process here********************************
                int i;
                printf("writing working here\n");
                for(i=0;i<shareindex+length;i++)
                {
                    sharememory[i+shareindex]=buff[i];
                }
                shareindex=shareindex+length;
                bzero(buff,256);
            //**********************************

            //using condition to decide wheather to wake up another writting process
                if(condition==1)
                {
                    pthread_cond_signal(&writing);
                }
                pthread_mutex_unlock(&lock);
                usleep(10000);
        }

}

void* readservice(void *client)
{
        int working=1;

        while(working==1)
        {

            pthread_mutex_lock(&readlock);
            printf("read begin\n");
            thread_ready++;
            pthread_cond_wait(&reading,&readlock);
            pthread_mutex_unlock(&readlock);
            //reading process here
            printf("reading is working here\n");
            printf("%s\n",sharememory);

            //************************************
            thread_finished++;
            pthread_mutex_lock(&readfinishlock);
            pthread_mutex_unlock(&readfinishlock);



        }


}


void controllthread()
{

        while(1)
        {
                pthread_mutex_lock(&threadlock);

                while(thread_ready!=threadnum)
                {
                //block the thread,let the thread get ready for  reading start
                }
                pthread_mutex_lock(&readfinishlock);
                thread_ready=0;
                pthread_cond_broadcast(&reading);//start reading here
                sleep(1);
                while(thread_finished!=threadnum)
                {
                //block the thread,let all the thread finish reading
                }

                pthread_mutex_unlock(&readfinishlock);

                pthread_mutex_unlock(&threadlock);
                thread_finished=0;
                //reading finished here
                condition=1;
                if(threadnum==0)
                {
                        continue;
                }
                pthread_cond_signal(&writing);
                sleep(1);
                condition=0;
                pthread_mutex_lock(&lock);
                pthread_mutex_unlock(&lock);

                //pthread_cond_

        }



}


void controllthread1()
{
        char buff[256];
        while(1)
        {
                int i=0;
                 for(;i<usernum;i++)
                {
                        if(read(fdvec[i],buff,256)>0)
                        {
                                int j=0;
                                for(;j<usernum;j++)
                                {
                                        write(fdvec[j],buff,256);
                                }
                                bzero(buff,256);
                        }
                }
	 }



}




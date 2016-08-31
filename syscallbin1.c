// Completely dummy skeleton
// for syscall reversing purpose
// unixfreaxjp

#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[])
{

int sockfd = 0, n = 0;
char recvBuff[1024];
// struct sockaddr_in serv_addr; 

// chdir

chdir("/tmp");

// open file


int fd;
fd = open("/tmp/testtest",O_RDONLY|O_CREAT,S_IRWXU);
close (fd);

// exec ls

execv("/bin/ls", (char *[]){ "/bin/ls", argv[0], NULL });


// nanosleep

struct timespec tim, tim2;
   tim.tv_sec = 1;
   tim.tv_nsec = 500;
   if(nanosleep(&tim , &tim2) < 0 )   
   {
      printf("Nano sleep system call failed \n");
      return -1;
   }
   printf("Nano sleep successfull \n");



// damonized

  int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
    int len = sizeof(struct sockaddr);
    struct sockaddr_in addr, foo;


    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff)); 

        close(connfd);
        sleep(1);
     }

// get socket

getsockname(listenfd, (struct sockaddr *) &foo, &len);


// socket connect


memset(recvBuff, '0',sizeof(recvBuff));
if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 


// end


    return 0;
}

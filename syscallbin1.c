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

// chdir
chdir("/tmp");

// open/close file
int fd;
fd = open("/tmp/testtest",O_RDONLY|O_CREAT,S_IRWXU);
close (fd);

// execvc ...lead to execve in libs
execv("/bin/ls", (char *[]){ "/bin/ls", argv[0], NULL });

// nanosleep ...lead to sleep in libs
struct timespec tim, tim2;
   tim.tv_sec = 1;
   tim.tv_nsec = 500;
   if(nanosleep(&tim , &tim2) < 0 )  // lea eax, [ebp+rem] ; push eax; lea eax, [ebp+req] ; push eax; call nanosleep; 
   {  // add esp, 10h; test eax, eax
      printf("Nano sleep system call failed \n"); // push offset $1 ; "Nano sleep system call failed \n"; call puts
      return -1;
   }
   printf("Nano sleep successfull \n");

// damonized

  // controls:
  int sockfd = 0, n = 0;
  int listenfd = 0, connfd = 0;
  struct sockaddr_in serv_addr; 
  int len = sizeof(struct sockaddr);
  struct sockaddr_in addr, foo;
   
   // init
    char sendBuff[1024];
    time_t ticks; 
   // form socket, memset & buffer
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 
   // form protocol
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 
   // bind the protocol and start listening
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    listen(listenfd, 10); 

    while(1)
    { 
       // accepting stream via socket...
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); // connfd will get the data 
        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks)); // lead to syscall puts..
        write(connfd, sendBuff, strlen(sendBuff));  // syscal write 

        close(connfd); // close connfd
        sleep(1); // not nanosleep..direct syscall sleep
     }

// get socket info
getsockname(listenfd, (struct sockaddr *) &foo, &len); // noted the foo 


// socket connect
memset(recvBuff, '0',sizeof(recvBuff));
if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // perror if not opening  socket..
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 
    // protocol as per listened...
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0) // perror for inet_pton
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // perror for fail connect
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

   // end
    return 0;
}

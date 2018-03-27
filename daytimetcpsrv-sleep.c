/*
Name:Philip Butts
Date:9/27/17
Class:CSC3460
Dscription: Server for connection to socket
Sleep Version -
*/
//#include<stdio.h>
//#include<stdlib.h>
#include  "unp.h"
#include  <time.h>
#include <string.h>
//#include <unistd.h>
 
int main(int argc, char **argv)
{
  int    listenfd, connfd, n;
  struct sockaddr_in  servaddr;
  char   buff[MAXLINE];
  time_t ticks;
  char   recvline[MAXLINE + 1];
  char   *token1; //Should be one of two token values
  char   *token2; //Should be one of two token values
  char   *search = " "; //This will  be the indication, for how to split the numbers up
  

  //initiates the socket and includes it 3 paramters
  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family      = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  //servaddr.sin_port = htons(); /* daytime server */
  servaddr.sin_port = htons((unsigned short)strtoul(argv[1], NULL, 0));//cast the port number to unisgned short
                                                                       //Convert argv[1] to an unsigned long int value via base
                                                                       //Null is a reference to the string
                                                                       //Cast to an unsigned short 
  Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

  //listen coverts an unconnected socket into a passive socket, 
  //indicating that the kernel should accept
  //incoming connection requests directed to this socket
  Listen(listenfd, LISTENQ);

  for ( ; ; ) {
    //connected descriptor
    connfd = Accept(listenfd, (SA *) NULL, NULL);//accept returns the value from client to connfd
    while ((n = read(connfd, recvline, MAXLINE)) > 0){ //stores the read value (in bytes)from client in n
        int msgs;//Declare new messages variable to hold amount of time the message need to print.
        int sleepAmount;
        recvline[n] = '\0'; /* null terminate */

        //split the strings up
        token1 = strtok(recvline , search);//This should point to the first number 
        token2 = strtok(NULL, search);//Should point to second value

        //Code to msgs lines and to sleep for certain amount of time
        msgs = atoi(token1);//converts messages back to integer
        sleepAmount = atoi(token2);

        while(msgs > 0){
            sleep(sleepAmount);//Sleep for specified num of secs
            ticks = time(NULL);
            snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
            Write(connfd, buff, strlen(buff)); //result is written back to the client
           
            msgs--;
        }
        printf("Finished.\n");//Print message when done.
      
    }
    Close(connfd);

  }

}






/*
 Name:Philip Butts
 Date:9/27/17
 Class:CSC3460
 Dscription: Server for connection to socket
 Sleep Version - Client sends the server the amount of time to send a message and how much time in between each
                 Server responds with the formated time in the specified intervals


 Question 3- I was unable to get an error while contacting the server from another client.
 The other client actually waited until the connection was closed from the first client, however, if had
 received an error it probably would have been a "connection refused", and Wireshark would have stated
 a destination unreachable description.
 */

#include "unp.h"
#include "time.h"
#include "string.h"

int main(int argc, char **argv) 
{
   int  info, sockfd, n;
   char  recvline[MAXLINE + 1];
   struct sockaddr_in   servaddr;
   short portNumber = 0;
   char msgBuffer[MAXLINE]; 
   char sleepBuffer[MAXLINE];
   int numOfMessages = 0;
   int sleepTime = 0;
   char* delimeter = " ";
   char totalInfo[MAXLINE];

   printf("Please enter the port number: ");
   scanf("%hd", &portNumber);


   if (argc != 2)
      err_quit("usage: a.out <IPaddress>");
   
   //Changed 1st arguement from AF_INET to 9999
   //Specify the three arguements
   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      err_sys("socket error");
       
   //set up server address
   //initializing address structure entirely to 0
   //bzero not std function, but found in networking libraries
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_port   = htons(portNumber);  /* daytime server */

   //convert dotted quad IPv4 address strng to network ordered integer
   //Also handles IPv6
   if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
      err_quit("inet_pton error for %s", argv[1]);

   //establish connection with listening socket on server 3-way
   //exit if unsuccessful
   if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
      err_sys("connect error");
        
   //SA is a server address structure
   printf("Connected to server at %s\n", sock_ntop((SA *) &servaddr, sizeof(servaddr)));

   // Need to convert two seperate values to strings 
   //Focus on sending one value.
   printf("Please enter the number of messages you want + sleep time using a space as a delimeter: ");
   scanf("%d", &numOfMessages);
   scanf("%d", &sleepTime);
  
   snprintf(msgBuffer, MAXLINE, "%d", numOfMessages);//Convert both numbers to strings
   snprintf(sleepBuffer, MAXLINE, "%d", sleepTime);
   snprintf(totalInfo, MAXLINE, "%s%s%s", msgBuffer, delimeter, sleepBuffer);//Concatenate the integers being entered with space


   info = write(sockfd, totalInfo, strlen(totalInfo)); //attempts to write nbytes from buffer to the file associated with handle

   if (info < 0) { //If data not sufficient then message wont send
      fprintf(stderr, "Failed to send message\n");//Anythng written to stderr isnt buffered
      close(sockfd);
      exit(1);//indicates unsucessful termination.
   } else {
      printf("Clients info is being sent...\n");
      while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
         //printf("Hello Im here: %s\n", recvline);
         recvline[n] = '\0';  /* null terminate */

         if (fputs(recvline, stdout) == EOF){//display to std output
            err_sys("fputs error");
         }
         //check if normal or abnormal exit
         //meaning of read: 0 means connection was closed by server; -1 return means error
         //checks the value of global variable errno for specific error
         if (n < 0){
            err_sys("read error");
         }
      }
   }
   Close(sockfd);
   exit(0);
}












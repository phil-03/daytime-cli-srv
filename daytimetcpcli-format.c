/*
 Name:Philip Butts
 Date:9/27/17
 Class:CSC3460
 Dscription: Server for connection to socket
 Format Version for client - Client communicates with the server, the client specifies the format in which the date/time will appear
 							 Server send this data back to the client.
 */
  
#include "unp.h"
#include <time.h>
#include "string.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	int	sockfd, n, info; //added to hold bytes added
	short portNumber = 0;
	char recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	char buff[MAXLINE];
	

	//Prompt user for port number
	printf("Please enter your port number, along with your format for date/time: \n");
	scanf("%hd", &portNumber);
	

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");
	
	//Changed 1st arguement from AF_INET to 9999
	//Specify the three arguements
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	//set up server address
	//initializing address structure entirely to 0
	//bzero not std function, but found in networking libraries
	//could use memset
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(portNumber);	/* daytime server */

	//convert dotted quad IPv4 address strng to netwrok ordered integer
	//Also handles IPv6
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);


	//establish connection with listening socket  on server 3-way
	//exit if unsuccessful
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");
        //use loop to read response
 	//SA is a server address structure
	printf("Connected to server at %s\n", sock_ntop((SA *) &servaddr, sizeof(servaddr)));

	//we can use read and write to communicate with a socket as long as it is connected
	while(1){
	   printf("The format for time will be: ");
	   fgets(buff, sizeof(buff), stdin); //Calls to sprintf cannot check for overflow of the destination buffer.
	   info = write(sockfd, buff, strlen(buff)); //attempts to write nbytes from buffer to the file associated with handle

   	   if (info < 0) { //If data not sufficient then message wont send
	      fprintf(stderr, "Failed to send message\n");//Anythng written to stderr isnt buffered
          close(sockfd);
          exit(1);//indicates unsucessful termination.
	   } else {
		  printf("Clients info is being sent...\n");
		  //same code to error check
	      while ((n = read(sockfd, recvline, MAXLINE)) > 0) { //read is the same as recv 
		     recvline[n] = '\0'; /* null terminate */
		     if (fputs(recvline, stdout) == EOF){
	          	err_sys("fputs error");
				 	
			 }

			 if (n < 0)
			   err_sys("read error");
			   exit(0); //Basically like a return zero
		  }
        }

	}
	Close(sockfd);
	
}

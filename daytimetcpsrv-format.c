/*
Name:Philip Butts
Date:9/27/17
Class:CSC3460
Dscription: Server for connection to socket
Format Version -
*/

#include"unp.h"
#include <time.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int	listenfd, connfd, n;
	struct sockaddr_in	servaddr;
	char buff[MAXLINE];
	time_t	ticks;
	//short portNumber = 0;
	char recvline[MAXLINE + 1];

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	//Prompt user for port number
	//printf("Please enter your port number, along with your format for date/time: \n");
	//scanf("%hd", &portNumber);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons((unsigned short)strtoul(argv[1], NULL, 0));	/* daytime server */

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));//server is bound to the socket by filling in Internet socket address
	Listen(listenfd, LISTENQ); //the socket is converted into a listening socket, on which incoming 
								//connections from clients will be accepted by the kernel

	for ( ; ; ) {
		//connected descriptor
		connfd = Accept(listenfd, (SA *) NULL, NULL);//accept returns the value from client to connfd
		while ((n = read(connfd, recvline, MAXLINE)) > 0){ //stores the read value (in bytes)from client in n

			recvline[n] = 0; /* null terminate */
	        ticks = time(NULL);
	        strftime(buff, sizeof(buff),recvline, localtime(&ticks));//str, maxsize , format received from the client, A pointer to a 
	        												   //structure of val being replaced

	        //snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
	        Write(connfd, buff, strlen(buff)); //result is written back to the client
	        Close(connfd);//close the connection 
    	}
    }
}

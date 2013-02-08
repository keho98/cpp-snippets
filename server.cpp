/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     if (argc < 2) {
         fprintf(stderr,"No port provided, falling to default port 8888\n");
	     portno = 8888;
     }
     else{
	 portno = atoi(argv[1]);
     }	
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     else 
              printf("Connected to port %d \n", portno);
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while(1){
         newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
              error("ERROR on accept");
        pid = fork(); 
	if(pid < 0)
		error("ERROR on fork");
	 if(pid == 0){
	 	close(sockfd);
		bzero(buffer,256);
         	int n_read = read(newsockfd,buffer,255);
         	fprintf(stderr, "Request received.\n");
         	int n_write = write(newsockfd, buffer,strlen(buffer));
         	exit(0);
	 }
	 else
		close(newsockfd);		
     } 
     close(sockfd);
     return 0;
}

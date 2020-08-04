
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clientlen;
     char buffer[512];
     struct sockaddr_in server_address, client_address;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &server_address, sizeof(server_address));
     portno = atoi(argv[1]);
     server_address.sin_family = AF_INET;
     server_address.sin_addr.s_addr = INADDR_ANY;
     server_address.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &server_address,
              sizeof(server_address)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clientlen = sizeof(client_address);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &client_address, 
                 &clientlen);
     if (newsockfd < 0) 
          error("ERROR on accept");
          
          
          
          FILE *filep;
         int ch = 0;
            filep = fopen("msgfile_receive.txt","a");            
            int words;
		read(newsockfd, &words, sizeof(int));
            //printf("Passed integer is : %d\n" , words);      //Ignore , Line for Testing
          while(ch != words)
       	   {
        	 read(newsockfd , buffer , 512); 
	   	 fprintf(filep , " %s" , buffer);   
		 //printf(" %s %d "  , buffer , ch); //Line for Testing , Ignore
		 ch++;
	   }
     	printf("The file was received successfully\n");
	   printf("The new file created is msgfile5.txt");
     close(newsockfd);
     close(sockfd);
     return 0; 
}

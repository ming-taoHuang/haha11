#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

char webpage[] =
"HTTP/1.1 200 0K\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title>406410001 Ming-Tao Huang</title>\r\n"
"<style>body{background-color:#FFFAFA }</style></html>\r\n"
"<body><center><h1>My name is Ming-Tao Huang!</h1><br>\r\n"
"<img src=\"ming.jpg\"></center></body></html>\r\n"
"<form method=\"post\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"file\" /><input type=\"submit\" value=\"Upload\" /></form>\r\n";


int main(int argc,char *argv[]){
	int create_socket,new_socket;
	socklen_t addrlen;
	int bufsize=2048;
	char *buffer =malloc(bufsize);
	char *buffer2 =malloc(bufsize);
	struct sockaddr_in address;
	int fd_img;
	if((create_socket=socket(AF_INET,SOCK_STREAM,0))>0)
	{
		printf("The socket was created\n");
	}
	address.sin_family=AF_INET;
	address.sin_addr.s_addr=INADDR_ANY;
	address.sin_port=htons(8080);

	if(bind(create_socket,(struct sockaddr *)&address,sizeof(address))==-1){
		perror("server: Binding\n");
		exit(1);
	}
      	if (listen(create_socket, 10) < 0) {
        	perror("server: listen");
        	exit(1);
     	}
	while(1){
      		if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0) {
         		perror("server: accept");
         		exit(1);
      		}

      		if (new_socket > 0){
         		printf("The Client is connected...\n");
      		}
		
		if(!fork())
		{
			close(create_socket);
			memset(buffer,0,2048);
			read(new_socket,buffer,2047);
			printf("%s\n",buffer);

			if(strncmp(buffer, "GET /ming.jpg",13)==0){
				fd_img=open("ming.jpg",O_RDONLY);
				sendfile(new_socket,fd_img,NULL,40000000);
				close(fd_img);
			}
			else if(strncmp(buffer, "POST /",6)==0){
				FILE *fp;
				fp=fopen("write.txt","w");
				fprintf(fp,"%s",buffer+680);
				write(new_socket,webpage,sizeof(webpage)-1);
			}
			else{
				write(new_socket,webpage,sizeof(webpage)-1);
			}
			close(new_socket);
			printf("closing.......\n");
			exit(0);
			
		}
		close(new_socket);
   	}
   	return 0;
}

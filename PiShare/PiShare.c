
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#define LISTEN_PORT 31235

int terminate = 0;

void sighandler(int signal)
{
	terminate = 1;
}



int listen_udp()
{
	char buffer[256];
	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if( sockfd < 0 )
		return -1;
	close(sockfd);

	return 0;
}

int listen_tcp()
{
	char buffer[256];
	int sockfd, clientfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server, client;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = LISTEN_PORT;
	if( bind(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0 ) {
		perror("");
		return -11;
	}
	listen(sockfd, 5);
	while( !terminate )
	{
		struct sockaddr_in client;
		ssize_t n;
		socklen_t addrlen = sizeof(client);
		clientfd = accept(sockfd, (struct sockaddr*) &client, &addrlen);
		if( clientfd < 0 )
			return -12;
		n = read(clientfd, buffer, 255);
		if( n < 0 ) {
			close(clientfd);
			continue;
		}
		if( strncmp(buffer, "HELO", 4) )
			puts("HELO!\n");
		else if(strncmp(buffer, "SHOW", 4) )
			puts("Not implemented yet :(\n");
		close(clientfd);
		
	}
	close(sockfd);
	return 0;
}


int main()
{
	int ret_udp, ret_tcp;
	pid_t pid = fork();
	struct sigaction act;
	act.sa_handler = sighandler;
	sigaction(SIGINT, &act, NULL);

	if( pid == 0 )
		ret_udp = listen_udp();
	else
		ret_tcp = listen_tcp();

	if( ret_udp < 0 )
		return ret_udp;
	if( ret_tcp < 0 )
		return ret_tcp;
	return 0;
}



#include<stdio.h> 			/* perror */
#include<stdlib.h>			/* exit	*/
#include<sys/types.h>		/* WNOHANG */
#include<sys/wait.h>		/* waitpid */
#include<string.h>			/* memset */
#include "dbtime.h"
#include "socketwrapper.h" 	/* socket layer wrapper */

#define	true		1
#define false		0

#define PORT 		3490 	/* Server�Ķ˿� */
#define MAXDATASIZE 100 	/*һ�ο��Զ�������ֽ��� */


int main(int argc, char *argv[])
{
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct hostent *he; /* ������Ϣ */
	struct sockaddr_in their_addr; /* �Է���ַ��Ϣ */
	if (argc != 2) 
	{
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
	}
	
	/* get the host info */
	if ((he=Gethostbyname(argv[1])) == NULL) 
	{
		/* ע�⣺��ȡDNS��Ϣʱ����ʾ������Ҫ��herror������perror */
		/* herror ���µİ汾�л���־��棬�Ѿ����鲻Ҫʹ���� */
		perror("gethostbyname");
		exit(1);
	}
	
	if ((sockfd=Socket(AF_INET,SOCK_STREAM,0))==-1) 
	{
		perror("socket");
		exit(1);
	}
	
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = Htons(PORT); /* short, NBO */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr_list[0]);
	memset(&(their_addr.sin_zero),0, 8); /* ���ಿ�����0 */

	dbtime_startTest ("Connect & Recv");
	
	if (Connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) 
	{
		perror("connect");
		exit(1);
	}
	
	if ((numbytes=Recv(sockfd,buf,MAXDATASIZE,0))==-1) 
	{
		perror("recv");
		exit(1);
	}
	
	dbtime_endAndShow ();

	buf[numbytes] = '\0';
	printf("Received: %s",buf);
	Close(sockfd);
	
	dbtime_startTest ("Sleep 5s");
    sleep(5);
	dbtime_endAndShow ();
	dbtime_finalize ();
	return true;

}



#include<stdio.h> 			/* perror */
#include<stdlib.h>			/* exit	*/
#include<sys/types.h>		/* WNOHANG */
#include<sys/wait.h>		/* waitpid */
#include<string.h>			/* memset */

#include "socketwrapper.h" 	/* socket layer wrapper */

#define	true		1
#define false		0

#define MYPORT 		3490                /* �����Ķ˿� */ 
#define BACKLOG 	10                 	/* listen��������ն��г��� */

int main() 
{
	int sockfd, new_fd;            /* �����˿ڣ����ݶ˿� */
	struct sockaddr_in sa;         /* ����ĵ�ַ��Ϣ */ 
	struct sockaddr_in their_addr; /* ���ӶԷ��ĵ�ַ��Ϣ */ 
	int sin_size;
	
	if ((sockfd = Socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		perror("socket"); 
		exit(1); 
	}
	
	sa.sin_family = AF_INET;
	sa.sin_port = Htons(MYPORT);         /* �����ֽ�˳�� */
	sa.sin_addr.s_addr = INADDR_ANY;     /* �Զ����IP */
	memset(&(sa.sin_zero),0, 8);            /* ���ಿ����0 */
	
	if ( Bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1) 
	{
		perror("bind");
		exit(1);
	}
	
	if (Listen(sockfd, BACKLOG) == -1) 
	{
		perror("listen");
		exit(1);
	}
	
	/* ��ѭ�� */
	while(1) 
	{
		sin_size = sizeof(struct sockaddr_in);
		new_fd = Accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) 
		{
			perror("accept");
			continue;
		}
		
		printf("Got connection from %s\n", Inet_ntoa(their_addr.sin_addr));
		if (fork() == 0) 
		{
			/* �ӽ��� */
			if (Send(new_fd, "Hello, world!\n", 14, 0) == -1)
			perror("send");
			Close(new_fd);
			exit(0);
		}
	
		Close(new_fd);
		
		/*��������ӽ��� */
		while(waitpid(-1,NULL,WNOHANG) > 0); 

	}
	return true;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#define PER_BIT_LEN  4096
#define SERV_PORT  5000
#define HEADER 	(sizeof(long long) + sizeof(int))

int main(void)
{
	int listenfd, connfd, fd;
	struct sockaddr_in servaddr, cliaddr;
	char buf[HEADER + PER_BIT_LEN];
	int len, data_len;
	long long pos, total;
	int count, index;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd < 0)
	{
		printf("socket error\n");
		return -1;
	}
	memset(&servaddr, 0, sizeof(struct sockaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	if( bind(listenfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) == -1)
	{
		printf("bind error\n");
		return -1;
	}
	if(listen(listenfd,5) < 0)
	{
		printf("listen error!\n");
		return -1;
	}
	if(( fd = open("disk",O_WRONLY|O_CREAT|O_TRUNC,0660)) < 0)
	{
		printf("open disk error!\n");
		return -1;
	}
	while(1)
	{
		len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, (socklen_t *)&len);
		if(connfd < 0)
		{
			printf("accept error!\n");
			return -1;
		}
		printf("connection from %s, port %d\n",inet_ntop(AF_INET, &cliaddr.sin_addr, buf, sizeof(buf)),
				ntohs(cliaddr.sin_port));
	
		total = 0;
		while(1)
		{
			len = HEADER + PER_BIT_LEN;
			index = 0;
			while(len > 0)
			{
				count = recv(connfd, buf + index, len, 0);
				if(count == 0)
					break;
				if(count < 0)
				{
					printf("recv error\n");
					return -1;
				}
				len -= count;
				index += count;
			}

			/*get the position*/
			pos = *( long long *)buf;

			/*get the data length*/
			data_len = *(int *)(buf + 8);

			/*transfer complete ?*/
			if(pos == -1)
			{
				printf("migration complete!\n");
				printf("total recv %lld\n",total);
				close(fd);
				close(connfd);
				return 0;
			}
			
			if( lseek(fd, pos, SEEK_SET) < 0)
			{
				printf("%lld\n",pos);
				printf("lseek error!\n");
				return -1;
			}
			
			count = write(fd, buf + 12, data_len);
			if(count < 0 )
			{
				printf("write disk error!\n");
				return -1;
			}
			total += count;
		}
	}
	return 0;
}


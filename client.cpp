#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int main() 
{
	int len = 0;
    int sockfd = -1;
	int retbytes = 0;
	struct sockaddr_in address = {0};

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
        printf("socket failed");
		return -1;
	}

	address.sin_family = AF_INET;
    address.sin_port = htons(5555);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
	len = sizeof(address);

	// 向服务器发送连接请求
	if (connect(sockfd, (struct sockaddr*)&address, len) == -1)
	{
        perror("connect failed");
		close(sockfd);
		return -1;
	}

	char msg[256] = { 0 };
	strcpy(msg, "<skinit><app>1</app><tick><tp>1</tp></tick></skinit>");
	len = (int)strlen(msg);
	retbytes = write(sockfd, msg, len);
	if (retbytes == -1)
	{
        perror("socket send data failed");
		close (sockfd);
		return -1;
	}

	char data[1024] = {0}; 
	read(sockfd, data, 1024);
	printf("data:%s", data);
	write(sockfd, msg, len);
	read(sockfd, data, 1024);
	printf("data:%s", data);
	
	close (sockfd);
	return 0;
}

// 编译指令
// g++ client.cpp -o client
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define Max_Open_Fd 1024
#define Socket_Port 5555
#define Max_Len     4096

int main () 
{
    int listen_fd = 0;

    // 创建监听套接字
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)
    {
        perror("listen socket create failed");
        return -1;
    }

    // 初始化结构体
    struct sockaddr_in server_addr = {0};
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(Socket_Port);

    // 绑定套接字
    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("socket bind failed");
        return -1;
    }
    if (listen(listen_fd, 16) == -1)
    {
        perror("socket listen failed");
        return -1;
    }

    // 创建 epoll 套接字
    int epoll_fd = epoll_create(Max_Open_Fd);
    if (epoll_fd == -1)
    {
        perror("create epoll failed");
        return -1;
    }

    // 将监听套接字加入到 epoll 的监控队列中
    struct epoll_event event_listen = {0};
    event_listen.events  = EPOLLIN;
    event_listen.data.fd = listen_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event_listen) == -1)
    {
        perror("epoll_ctl failed");
        return -1;
    }

    char buffer[Max_Len] = {0};
    struct epoll_event event_feed[Max_Open_Fd] = {0};

    printf("start listen");

    // 死循环，一直等待客户的事件
    for (;;)
    {
        // 等待 epoll 事件
        size_t count_ready = epoll_wait(epoll_fd, event_feed, Max_Open_Fd, 100);
        if (count_ready == -1)
        {
            perror("epoll_wait failed");
            return -1;
        }
        
        // 处理 epoll 事件
        for (int i = 0; i < count_ready; ++i)
        {
            // 监控到的套接字是服务器的监听套接字，那么是有新的客户连接请求
            if (event_feed[i].data.fd == listen_fd)
            {
                // accept 客户连接
                struct sockaddr_in client_addr = {0};
                socklen_t sockadd_len = sizeof(client_addr);
                int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &sockadd_len);
                if (client_fd == -1)
                {
                    continue;
                }

                // 将新的套接字加入 epoll 监控队列
                struct epoll_event event_client = {0};
                event_client.events  = EPOLLIN;
                event_client.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event_client) == -1)
                {
                    perror("epoll add client fd failed");
                    continue;
                }
            }
            else
            {
                memset(buffer, 0, Max_Len);

                // 读取客户发送的数据
                int conn_fd = event_feed[i].data.fd;
                int bytes = read(conn_fd, buffer, Max_Len);
                // 数据的长度是0，表示客户端主动关闭了套接字
                if (bytes == 0)
                {
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, conn_fd, NULL);
                    close(conn_fd);
                }
                else
                {
                    char data[] = "hello my dear";
                    // 如果客户发送的数据大于 Max_Len, 根据实际场景处理
                    printf("client[%d] send data:%s", conn_fd, buffer);
                    write(conn_fd, (void*)data, strlen(data));
                }                
            }            
        }
        usleep(100);
    }
}

// 编译指令
// g++ server.cpp -o server -lpthread
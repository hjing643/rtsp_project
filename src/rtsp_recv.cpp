
#include <arpa/inet.h>
#include <pthread.h>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <list>
#include <string>
#include<iostream>

// cache 4Mb, 20seconds
#define PORT 9999           // 服务器监听的端口
#define BUFFER_SIZE 8192    // 接收缓冲区大小
#define BUFFER_NUMBER 500 // 缓存区数量
class ThreadSafeBuffer {
public:
    ThreadSafeBuffer(size_t maxSize) : maxSize(maxSize) {}

    // 写入数据
    void write(const std::string& data) {
        if (write_buffer.size() >= maxSize) 
        {
            printf("****************************************************swap buffer*************************************************");

            std::unique_lock<std::mutex> lock(mutex); // 独占锁（读取时也需要修改列表）
            write_buffer.push_back(data);
            read_buffer.clear();
            read_buffer.swap(write_buffer);
        }
        else{    
            write_buffer.push_back(data);  
            //printf("*****************************************write buffer[%d], buffer size[%d:%d]\n", data.length(), buffer.size() , maxSize);
        }
    }

    // 读取all数据
    std::list<std::string> readAll() {
        std::unique_lock<std::mutex> lock(mutex); // 独占锁（读取时也需要修改列表）
        std::list<std::string> lst_ret;
        lst_ret.swap(read_buffer);
        return lst_ret;
    }

    // 获取当前数据量
    size_t size() const {
        std::unique_lock<std::mutex> lock(mutex); // 共享锁
        return read_buffer.size();
    }

private:
    std::list<std::string> write_buffer; // 存储数据的列表
    std::list<std::string> read_buffer; // 存储数据的列表
    size_t maxSize; // 最大容量
    mutable std::mutex mutex; // 读写锁
};

ThreadSafeBuffer g_thread_buffer(BUFFER_NUMBER);

// TCP服务器逻辑函数
// 处理客户端连接的函数
void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    char buffer[BUFFER_SIZE];

    while (true) {
        // 接收数据
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                std::cout << "*************************Client disconnected.*****************" << std::endl;
            } else {
                std::cout << "*****************recv error: " << std::endl;
            }
            break;
        }
        std::string str_buffer;
        str_buffer.assign(buffer, bytes_received);
        g_thread_buffer.write(str_buffer);
        //usleep(40 * 1000);
    }

    // 关闭客户端套接字
    close(client_socket);
    delete (int*)arg; // 释放动态分配的客户端套接字
    return nullptr;
}


void* tcp_server(void* arg) 
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};

    // 1. 创建Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    // 2. 绑定Socket到本地地址和端口
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // 监听所有网络接口
    server_addr.sin_port = htons(PORT);       // 设置端口

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        pthread_exit(NULL);
    }

    // 3. 监听连接请求
    if (listen(server_fd, 5) < 0) { // 最多允许5个连接排队
        perror("Listen failed");
        close(server_fd);
        pthread_exit(NULL);
    }

    printf("Server is listening on port %d...\n", PORT);
 
    while (1)
    {
        // 4. 接受客户端连接
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
            perror("**************************************Accept failed**************************************************************************");
        }

        printf("**********************************New client connected:*******************: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

     // 动态分配客户端套接字，避免线程间共享栈变量
        int* client_socket_ptr = new int(client_fd);

        // 创建新线程处理客户端连接
        pthread_t thread_id = ~0;
        if (pthread_create(&thread_id, nullptr, handle_client, client_socket_ptr) != 0) {
            std::cerr << "pthread_create error: " << std::endl;
            close(client_fd);
            delete client_socket_ptr;
        } else {
            // 分离线程，使其在退出时自动释放资源
            pthread_detach(thread_id);
        }
    }
     // 关闭服务器套接字
    close(server_fd);
    return nullptr;
}

void start_tcp_server()
{
    static bool start = false;
    if (start)
    {
        return;
    }
    
    // 创建线程运行TCP服务器
    pthread_t server_thread = ~0;

    if (pthread_create(&server_thread, NULL, tcp_server, NULL) != 0) {
        perror("Failed to create server thread");
        return;
    }

    // 主线程可以继续执行其他任务
    printf("Main thread is running...\n");
    start = true;
    
    pthread_detach(server_thread);

    // 等待服务器线程结束
    //if (pthread_join(server_thread, NULL) != 0) {
        //perror("Failed to join server thread");
       // return EXIT_FAILURE;
   // }

   // printf("Main thread exiting.\n");
   // return 0;
}



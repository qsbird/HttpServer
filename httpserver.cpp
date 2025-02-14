//
// Created by qs on 25-2-14.
//
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    // 创建socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "创建socket失败" << std::endl;
        return 1;
    }

    // 设置端口复用
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 绑定地址和端口
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "绑定端口失败" << std::endl;
        return 1;
    }

    // 开始监听
    if (listen(server_fd, 5) < 0) {
        std::cerr << "监听失败" << std::endl;
        return 1;
    }

    std::cout << "服务器运行在 http://localhost:8080" << std::endl;

    while (true) {
        // 接受客户端连接
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);

        if (client_fd < 0) {
            std::cerr << "接受连接失败" << std::endl;
            continue;
        }

        // 读取请求（简单读取不处理）
        char buffer[1024] = {0};
        read(client_fd, buffer, sizeof(buffer));

        // 构造HTTP响应
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 12\r\n\r\n"
            "Hello World!";

        // 发送响应
        send(client_fd, response.c_str(), response.size(), 0);

        // 关闭连接
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
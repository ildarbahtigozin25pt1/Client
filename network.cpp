#include <system_error>
#include "network.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory>
#include <stdexcept>
#include <cstring>

NetworkSession::NetworkSession(const std::string& address, uint16_t port)
    : socket_fd(-1), server_address(address), server_port(port) {}

NetworkSession::~NetworkSession() {
    if (socket_fd != -1) close(socket_fd);
}

void NetworkSession::connect() {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
        throw std::system_error(errno, std::generic_category(), "socket() failed");

    // Установка таймаута 3 минуты (по методичке)
    struct timeval timeout{180, 0};
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    std::unique_ptr<sockaddr_in> serv_addr(new sockaddr_in);
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(server_port);
    serv_addr->sin_addr.s_addr = inet_addr(server_address.c_str());

    int rc = ::connect(socket_fd, reinterpret_cast<sockaddr*>(serv_addr.get()), sizeof(sockaddr_in));
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "connect() failed");
}

void NetworkSession::send_text(const std::string& msg) {
    ssize_t rc = send(socket_fd, msg.c_str(), msg.length(), 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "send() failed");
}

std::string NetworkSession::recv_text() {
    int buflen = 1024;
    std::unique_ptr<char[]> buf(new char[buflen]);
    ssize_t rc = recv(socket_fd, buf.get(), buflen, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "recv() failed");
    return std::string(buf.get(), rc);
}

bool NetworkSession::authenticate(const std::string& auth_msg) {
    send_text(auth_msg);
    std::string response = recv_text();
    return (response == "OK");
}

void NetworkSession::send_binary(const void* data, size_t size) {
    ssize_t rc = send(socket_fd, data, size, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "send() failed");
    if (static_cast<size_t>(rc) != size)
        throw std::runtime_error("send() incomplete transfer");
}

void NetworkSession::recv_binary(void* data, size_t size) {
    ssize_t rc = recv(socket_fd, data, size, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "recv() failed");
    if (static_cast<size_t>(rc) != size)
        throw std::runtime_error("recv() incomplete transfer");
}

void NetworkSession::send_vectors(const std::vector<Vector>& vectors) {
    // Шаг 6: отправка количества векторов
    uint32_t count = vectors.size();
    send_binary(&count, sizeof(count));

    // Шаги 7-10: отправка векторов
    for (const auto& vec : vectors) {
        send_binary(&vec.size, sizeof(vec.size)); // Шаг 7
        if (vec.size > 0) {
            send_binary(vec.data.data(), vec.size * sizeof(int64_t)); // Шаг 8
        }
    }
}

std::vector<int64_t> NetworkSession::receive_results(size_t count) {
    std::vector<int64_t> results(count);
    for (size_t i = 0; i < count; ++i) {
        recv_binary(&results[i], sizeof(int64_t)); // Шаг 9
    }
    return results;
}

void NetworkSession::disconnect() {
    if (socket_fd != -1) {
        close(socket_fd);
        socket_fd = -1;
    }
}

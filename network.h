#pragma once
#include <string>
#include <cstdint>
#include <vector>

struct Vector {
    uint32_t size;
    std::vector<int64_t> data;
};

class NetworkSession {
private:
    int socket_fd;
    std::string server_address;
    uint16_t server_port;

    void send_text(const std::string& msg);
    std::string recv_text();
    void send_binary(const void* data, size_t size);
    void recv_binary(void* data, size_t size);

public:
    NetworkSession(const std::string& address, uint16_t port);
    ~NetworkSession();

    void connect();
    bool authenticate(const std::string& auth_msg);
    void send_vectors(const std::vector<Vector>& vectors);
    std::vector<int64_t> receive_results(size_t count);
    void disconnect();
};

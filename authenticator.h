#pragma once
#include <string>
#include <cstdint>

class Authenticator {
public:
    // Генерация соли
    static std::string generate_salt16();
    
    // Формирование хеша
    static std::string compute_hash(const std::string& salt16, const std::string& password);
    
    // Формирование сообщения аутентификации
    static std::string create_auth_message(const std::string& login, const std::string& salt16, const std::string& hash);
};

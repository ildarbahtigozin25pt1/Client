#pragma once
#include <string>
#include <cstdint>

class Authenticator {
public:
    // Генерация соли (Приложение Б, пример 1)
    static std::string generate_salt16();
    
    // Формирование хеша (Приложение Б, пример 2)
    static std::string compute_hash(const std::string& salt16, const std::string& password);
    
    // Формирование сообщения аутентификации (ТЗ п. 4.2.3)
    static std::string create_auth_message(const std::string& login, const std::string& salt16, const std::string& hash);
};

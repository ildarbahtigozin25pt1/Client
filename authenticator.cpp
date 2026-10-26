#include "authenticator.h"
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>

namespace CPP = CryptoPP;

std::string Authenticator::generate_salt16() {
    // Генерация 8 байт (64 бита) случайной соли
    CPP::byte salt[8];
    CPP::AutoSeededRandomPool prng;
    prng.GenerateBlock(salt, sizeof(salt));
    
    std::string salt_16;
    // Преобразование в 16 hex-символов
    CPP::ArraySource(salt, sizeof(salt), true,
        new CPP::HexEncoder(new CPP::StringSink(salt_16)));
    
    // Дополнение слева нулями до 16 символов (ТЗ п. 4.2.4)
    while (salt_16.length() < 16) {
        salt_16 = "0" + salt_16;
    }
    
    return salt_16;
}

std::string Authenticator::compute_hash(const std::string& salt16, const std::string& password) {
    CPP::SHA256 sha256;
    std::string hash_16;
    
    // HASH = SHA-256(SALT16 || PASSWORD)
    CPP::StringSource(
        std::string(salt16).append(password), true,
        new CPP::HashFilter(
            sha256,
            new CPP::HexEncoder(new CPP::StringSink(hash_16))));
    
    return hash_16;
}

std::string Authenticator::create_auth_message(const std::string& login, const std::string& salt16, const std::string& hash) {
    // MSG = LOGIN || SALT16 || HASH
    return std::string(login).append(salt16).append(hash);
}

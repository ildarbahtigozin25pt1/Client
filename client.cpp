#include "client.h"
#include "authenticator.h"
#include "network.h"
#include "file_io.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Client::run(const Params& params) {
    try {
        // Чтение логина и пароля из конфига
        std::ifstream config_file(params.config_file);
        if (!config_file.is_open())
            throw std::runtime_error("Cannot open config file");
        
        std::string line;
        std::getline(config_file, line);
        size_t pos = line.find(':');
        std::string login = line.substr(0, pos);
        std::string password = line.substr(pos + 1);

        // Чтение векторов
        auto vectors = read_vectors(params.input_file);

        // Сетевое взаимодействие
        NetworkSession session(params.server_address, params.server_port);
        session.connect();

        // Аутентификация
        std::string salt16 = Authenticator::generate_salt16();
        std::string hash = Authenticator::compute_hash(salt16, password);
        std::string auth_msg = Authenticator::create_auth_message(login, salt16, hash);

        if (!session.authenticate(auth_msg)) {
            throw std::runtime_error("Authentication failed (ERR)");
        }

        // Обмен данными
        session.send_vectors(vectors);
        auto results = session.receive_results(vectors.size());

        // Завершение
        session.disconnect();
        write_results(params.output_file, results);

    } catch (const std::exception& e) {
        handle_error("Client::run", params.input_file, e.what());
    }
}

void Client::handle_error(const std::string& func_name, 
                         const std::string& params, 
                         const std::string& error_type) {
    std::cerr << "Ошибка в функции: " << func_name << "\n"
              << "Параметры: " << params << "\n"
              << "Тип ошибки: " << error_type << std::endl;
    std::exit(1);
}

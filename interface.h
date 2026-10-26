#pragma once
#include <boost/program_options.hpp>
#include <string>

namespace po = boost::program_options;

// Структура для хранения параметров (ТЗ п. 4.2.6)
struct Params {
    std::string server_address;
    uint16_t server_port = 33333;
    std::string input_file;
    std::string output_file;
    std::string config_file = "~/.config/vclient.conf";
};

// Интерфейс командной строки
class Interface {
private:
    po::options_description desc;
    po::variables_map vm;
    Params params;

public:
    Interface();
    bool Parser(int argc, const char** argv);
    std::string getDescription();
    Params getParams() { return params; }
};

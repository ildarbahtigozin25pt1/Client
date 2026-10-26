#include "interface.h"
#include <sstream>

Interface::Interface() : desc("Allowed options") {
    desc.add_options()
        ("help,h", "Show help")
        ("address,a", po::value<std::string>(&params.server_address)->required(), "Server address")
        ("port,p", po::value<uint16_t>(&params.server_port)->default_value(33333), "Server port")
        ("input,i", po::value<std::string>(&params.input_file)->required(), "Input file")
        ("output,o", po::value<std::string>(&params.output_file)->required(), "Output file")
        ("config,c", po::value<std::string>(&params.config_file)->default_value("~/.config/vclient.conf"), "Config file");
}

bool Interface::Parser(int argc, const char** argv) {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    
    if (vm.count("help"))
        return false;
        
    po::notify(vm);
    return true;
}

std::string Interface::getDescription() {
    std::ostringstream ss;
    ss << desc;
    return ss.str();
}

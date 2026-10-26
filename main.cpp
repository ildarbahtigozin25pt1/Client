#include <iostream>
#include "interface.h"
#include "client.h"

int main(int argc, const char** argv) {
    try {
        Interface iface;
        if (iface.Parser(argc, argv)) {
            Params params = iface.getParams();
            Client client;
            client.run(params);
        } else {
            std::cout << iface.getDescription();
        }
    } catch (po::error& e) {
        std::cerr << "error: " << e.what() << "\n";
        std::cerr << "Use -h for help\n";
        return 1;
    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 2;
    }
    return 0;
}

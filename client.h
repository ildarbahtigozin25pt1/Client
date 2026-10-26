#pragma once
#include "interface.h"
#include <string>

class Client {
public:
    void run(const Params& params);
    static void handle_error(const std::string& func_name, 
                            const std::string& params, 
                            const std::string& error_type);
};

#pragma once
#include "network.h"
#include <string>
#include <vector>

std::vector<Vector> read_vectors(const std::string& filename);
void write_results(const std::string& filename, const std::vector<int64_t>& results);

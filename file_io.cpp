#include "file_io.h"
#include <fstream>
#include <stdexcept>

std::vector<Vector> read_vectors(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open input file: " + filename);

    uint32_t n;
    file.read(reinterpret_cast<char*>(&n), sizeof(n));
    if (!file)
        throw std::runtime_error("Read error: cannot read vector count");

    std::vector<Vector> vectors(n);
    for (uint32_t i = 0; i < n; ++i) {
        file.read(reinterpret_cast<char*>(&vectors[i].size), sizeof(uint32_t));
        vectors[i].data.resize(vectors[i].size);
        if (vectors[i].size > 0) {
            file.read(reinterpret_cast<char*>(vectors[i].data.data()), 
                     vectors[i].size * sizeof(int64_t));
        }
        if (!file)
            throw std::runtime_error("Read error: cannot read vector data");
    }
    return vectors;
}

void write_results(const std::string& filename, const std::vector<int64_t>& results) {
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open output file: " + filename);

    file << results.size();
    for (const auto& result : results) {
        file << " " << result;
    }
    file << std::endl;
}

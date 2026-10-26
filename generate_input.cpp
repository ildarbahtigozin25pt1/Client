/**
 * @file generate_input.cpp
 * @brief Утилита для создания тестового бинарного файла input.bin
 *        в соответствии с п. 4.2.2 ТЗ
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

// Структура для удобства работы с вектором в коде
struct TestDataVector {
    uint32_t size;
    std::vector<int64_t> data;
};

int main() {
    // 1. Подготавливаем тестовые данные
    // Создадим 2 вектора:
    // Вектор 1: размер 3, значения [10, 20, 30]
    // Вектор 2: размер 2, значения [-100, 200]
    std::vector<TestDataVector> test_vectors = {
        {3, {10, 20, 30}},
        {2, {-100, 200}}
    };

    // 2. Открываем файл в бинарном режиме записи
    std::ofstream file("input.bin", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл input.bin" << std::endl;
        return 1;
    }

    // 3. Записываем количество векторов N (4 байта, uint32_t)
    uint32_t n = test_vectors.size();
    file.write(reinterpret_cast<const char*>(&n), sizeof(n));

    // 4. Записываем каждый вектор
    for (const auto& vec : test_vectors) {
        // 4.1. Записываем размер вектора S (4 байта, uint32_t)
        file.write(reinterpret_cast<const char*>(&vec.size), sizeof(vec.size));
        
        // 4.2. Записываем значения вектора (S значений по 8 байт, int64_t)
        if (vec.size > 0) {
            file.write(reinterpret_cast<const char*>(vec.data.data()), 
                       vec.size * sizeof(int64_t));
        }
    }

    // 5. Проверяем, что запись прошла успешно
    if (file.good()) {
        std::cout << "Файл 'input.bin' успешно создан!" << std::endl;
        std::cout << "Записано векторов: " << n << std::endl;
    } else {
        std::cerr << "Ошибка при записи в файл 'input.bin'" << std::endl;
        return 1;
    }

    file.close();
    return 0;
}

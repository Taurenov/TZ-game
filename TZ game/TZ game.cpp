#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <locale>
#include <codecvt> // Для кодирования/декодирования

class String {
private:
    char* data;   // Указатель на динамическую память
    size_t size;  // Длина строки

public:
    // Конструктор по умолчанию
    String() : data(nullptr), size(0) {}

    // Конструктор от C-строки
    String(const char* str) {
        if (str) {
            size = std::strlen(str);
            data = new char[size + 1];
            std::strcpy(data, str);
        }
        else {
            size = 0;
            data = nullptr;
        }
    }

    // Конструктор копирования
    String(const String& other) : size(other.size) {
        data = new char[size + 1];
        std::strcpy(data, other.data);
    }

    // Конструктор перемещения
    String(String&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    // Оператор присваивания (копирующий)
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new char[size + 1];
            std::strcpy(data, other.data);
        }
        return *this;
    }

    // Оператор присваивания (перемещающий)
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // Оператор += для конкатенации
    String& operator+=(const String& other) {
        if (other.size == 0) return *this;

        char* new_data = new char[size + other.size + 1];
        if (data) {
            std::strcpy(new_data, data);
        }
        std::strcat(new_data, other.data);

        delete[] data;
        data = new_data;
        size += other.size;

        return *this;
    }

    // Внешний оператор сложения
    friend String operator+(const String& lhs, const String& rhs) {
        String result(lhs);
        result += rhs;
        return result;
    }

    // Метод для получения C-строки
    const char* c_str() const {
        return data ? data : "";
    }

    // Деструктор
    ~String() {
        delete[] data;
    }

    // Метод для получения размера строки
    size_t getSize() const {
        return size;
    }
};

// Функция для сравнения строк без учета регистра
bool caseInsensitiveCompare(const String& a, const String& b) {
    size_t minSize = std::min(a.getSize(), b.getSize());

    for (size_t i = 0; i < minSize; ++i) {
        char aLower = std::tolower(a.c_str()[i]);
        char bLower = std::tolower(b.c_str()[i]);
        if (aLower != bLower) {
            return aLower < bLower;
        }
    }

    return a.getSize() < b.getSize(); // если равны, меньшая длина идет первой
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    std::vector<String> strings;
    std::cout << "Введите строки (пустая строка завершает ввод):\n";
    std::string input;

    // Считываем строки от пользователя
    while (std::getline(std::cin, input) && !input.empty()) {
        strings.emplace_back(input.c_str());
    }

    // Проверяем, введены ли строки
    if (strings.empty()) {
        std::cout << "Не введены строки." << std::endl;
        return 0;
    }

    // Сортируем строки в обратном порядке без учета регистра
    std::sort(strings.begin(), strings.end(), caseInsensitiveCompare);
    std::reverse(strings.begin(), strings.end());

    std::cout << "\nОтсортированные строки (в обратном порядке без учета регистра):\n";
    for (const auto& str : strings) {
        std::cout << str.c_str() << '\n';
    }

    return 0;
}

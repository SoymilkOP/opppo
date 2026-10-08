#include "include/Movie.h"
#include "include/MovieContainer.h"
#include "include/CreationMethod.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

// Извлечение строки в кавычках или обычного токена
std::string extractQuoted(std::istringstream& iss) {
    iss >> std::ws;
    if (iss.peek() == '"') {
        iss.get();
        std::string token;
        std::getline(iss, token, '"');
        return token;
    }
    std::string token;
    iss >> token;
    return token;
}

void processFile(const std::string& filename, MovieContainer& container) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Ошибка: не удалось открыть файл \"" << filename << "\"" << std::endl;
        return;
    }

    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        ++lineNum;
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        try {
            if (command == "ADD") {
                std::string type;
                iss >> type;

                if (type == "Feature") {
                    auto title = extractQuoted(iss);
                    auto director = extractQuoted(iss);
                    container.add(std::make_unique<FeatureFilm>(std::move(title), std::move(director)));
                }
                else if (type == "Cartoon") {
                    auto title = extractQuoted(iss);
                    std::string methodStr;
                    iss >> methodStr;
                    auto method = stringToMethod(methodStr);
                    container.add(std::make_unique<Cartoon>(std::move(title), method));
                }
                else if (type == "Series") {
                    auto title = extractQuoted(iss);
                    auto director = extractQuoted(iss);
                    int episodes;
                    if (!(iss >> episodes)) {
                        throw std::runtime_error("Неверное количество серий");
                    }
                    container.add(std::make_unique<Series>(std::move(title), std::move(director), episodes));
                }
                else {
                    std::cerr << "Строка " << lineNum << ": неизвестный тип \"" << type << "\"" << std::endl;
                }
            }
            else if (command == "REM") {
                std::string field;
                iss >> field;
                auto value = extractQuoted(iss);
                size_t removed = container.remove(field, value);
                std::cout << "Удалено объектов: " << removed << std::endl;
            }
            else if (command == "PRINT") {
                container.printAll();
            }
            else {
                std::cerr << "Строка " << lineNum << ": неизвестная команда \"" << command << "\"" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка в строке " << lineNum << ": " << e.what() << std::endl;
        }
    }
}

int main(int argc, const char* argv[]) {
#ifdef _WIN32
    constexpr UINT kUtf8CodePage = 65001;
    SetConsoleOutputCP(kUtf8CodePage);
    SetConsoleCP(kUtf8CodePage);
#endif

    std::string filename = "commands.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    MovieContainer container;
    processFile(filename, container);

    return 0;
}
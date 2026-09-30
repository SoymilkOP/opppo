#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cctype>

// ==================== Перечисление способа создания мультфильма ====================
enum class CreationMethod {
    Drawn,       // рисованный
    Puppet,      // кукольный
    Plasticine,  // пластилиновый
    CGI,         // компьютерная графика
    StopMotion   // покадровая анимация
};

std::string methodToString(CreationMethod m) {
    switch (m) {
        case CreationMethod::Drawn:      return "рисованный";
        case CreationMethod::Puppet:     return "кукольный";
        case CreationMethod::Plasticine: return "пластилиновый";
        case CreationMethod::CGI:        return "CGI";
        case CreationMethod::StopMotion: return "покадровая анимация";
        default:                         return "неизвестно";
    }
}

CreationMethod stringToMethod(const std::string& s) {
    std::string lower = s;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "drawn" || lower == "рисованный")           return CreationMethod::Drawn;
    if (lower == "puppet" || lower == "кукольный")           return CreationMethod::Puppet;
    if (lower == "plasticine" || lower == "пластилиновый")   return CreationMethod::Plasticine;
    if (lower == "cgi")                                      return CreationMethod::CGI;
    if (lower == "stopmotion" || lower == "stop-motion")     return CreationMethod::StopMotion;
    throw std::runtime_error("Неизвестный способ создания мультфильма: " + s);
}

// ==================== Абстрактный базовый класс ====================
class Movie {
protected:
    std::string title;

public:
    explicit Movie(const std::string& title) : title(title) {}
    virtual ~Movie() = default;

    const std::string& getTitle() const { return title; }

    virtual std::string getType() const = 0;
    virtual void print(std::ostream& os) const = 0;

    // Виртуальный метод для проверки условия удаления
    virtual bool matchesCondition(const std::string& field, const std::string& value) const {
        if (field == "title") {
            return title == value;
        }
        if (field == "type") {
            return getType() == value;
        }
        return false;
    }
};

// ==================== Игровой фильм ====================
class FeatureFilm : public Movie {
private:
    std::string director;

public:
    FeatureFilm(const std::string& title, const std::string& director)
        : Movie(title), director(director) {}

    std::string getType() const override { return "Feature"; }

    void print(std::ostream& os) const override {
        os << "[Игровой] \"" << title << "\" | Режиссёр: " << director;
    }

    bool matchesCondition(const std::string& field, const std::string& value) const override {
        if (Movie::matchesCondition(field, value)) return true;
        if (field == "director") return director == value;
        return false;
    }
};

// ==================== Мультфильм ====================
class Cartoon : public Movie {
private:
    CreationMethod method;

public:
    Cartoon(const std::string& title, CreationMethod method)
        : Movie(title), method(method) {}

    std::string getType() const override { return "Cartoon"; }

    void print(std::ostream& os) const override {
        os << "[Мультфильм] \"" << title << "\" | Способ создания: " << methodToString(method);
    }

    bool matchesCondition(const std::string& field, const std::string& value) const override {
        if (Movie::matchesCondition(field, value)) return true;
        if (field == "method") {
            try {
                return method == stringToMethod(value);
            } catch (...) {
                return false;
            }
        }
        return false;
    }
};

// ==================== Сериал ====================
class Series : public Movie {
private:
    std::string director;
    int episodes;

public:
    Series(const std::string& title, const std::string& director, int episodes)
        : Movie(title), director(director), episodes(episodes) {}

    std::string getType() const override { return "Series"; }

    void print(std::ostream& os) const override {
        os << "[Сериал] \"" << title << "\" | Режиссёр: " << director
           << " | Серий: " << episodes;
    }

    bool matchesCondition(const std::string& field, const std::string& value) const override {
        if (Movie::matchesCondition(field, value)) return true;
        if (field == "director") return director == value;
        if (field == "episodes") {
            try {
                return episodes == std::stoi(value);
            } catch (...) {
                return false;
            }
        }
        return false;
    }
};

// ==================== Вспомогательные функции парсинга ====================
std::string extractQuoted(std::istringstream& iss) {
    std::string token;
    iss >> std::ws;
    if (iss.peek() == '"') {
        iss.get(); // пропускаем "
        std::getline(iss, token, '"');
    } else {
        iss >> token;
    }
    return token;
}

// ==================== Основной класс контейнера ====================
class MovieContainer {
private:
    std::vector<std::unique_ptr<Movie>> movies;

public:
    void add(std::unique_ptr<Movie> movie) {
        movies.push_back(std::move(movie));
        std::cout << "Добавлен: ";
        movies.back()->print(std::cout);
        std::cout << std::endl;
    }

    void remove(const std::string& field, const std::string& value) {
        auto it = std::remove_if(movies.begin(), movies.end(),
            [&](const std::unique_ptr<Movie>& m) {
                return m->matchesCondition(field, value);
            });

        size_t removed = std::distance(it, movies.end());
        movies.erase(it, movies.end());

        std::cout << "Удалено объектов: " << removed << std::endl;
    }

    void printAll() const {
        if (movies.empty()) {
            std::cout << "Контейнер пуст." << std::endl;
            return;
        }
        std::cout << "=== Содержимое контейнера (" << movies.size() << ") ===" << std::endl;
        for (size_t i = 0; i < movies.size(); ++i) {
            std::cout << (i + 1) << ". ";
            movies[i]->print(std::cout);
            std::cout << std::endl;
        }
    }
};

// ==================== Обработка команд ====================
void processFile(const std::string& filename, MovieContainer& container) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл \"" << filename << "\"" << std::endl;
        return;
    }

    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        ++lineNum;
        // Пропускаем пустые строки и комментарии
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        try {
            if (command == "ADD") {
                std::string type;
                iss >> type;

                if (type == "Feature") {
                    std::string title = extractQuoted(iss);
                    std::string director = extractQuoted(iss);
                    container.add(std::make_unique<FeatureFilm>(title, director));
                }
                else if (type == "Cartoon") {
                    std::string title = extractQuoted(iss);
                    std::string methodStr;
                    iss >> methodStr;
                    CreationMethod method = stringToMethod(methodStr);
                    container.add(std::make_unique<Cartoon>(title, method));
                }
                else if (type == "Series") {
                    std::string title = extractQuoted(iss);
                    std::string director = extractQuoted(iss);
                    int episodes;
                    iss >> episodes;
                    container.add(std::make_unique<Series>(title, director, episodes));
                }
                else {
                    std::cerr << "Строка " << lineNum << ": неизвестный тип фильма \"" << type << "\"" << std::endl;
                }
            }
            else if (command == "REM") {
                std::string field, value;
                iss >> field;
                value = extractQuoted(iss);
                container.remove(field, value);
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

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, const char* argv[]) {
#ifdef _WIN32
    // Устанавливаем UTF-8 для консоли Windows
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    std::string filename = "commands.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    MovieContainer container;
    processFile(filename, container);

    return 0;
}
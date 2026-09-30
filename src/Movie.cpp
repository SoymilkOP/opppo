#include "include/Movie.h"
#include <stdexcept>

// ==================== Movie ====================
Movie::Movie(std::string title) : title_(std::move(title)) {}

const std::string& Movie::title() const noexcept {
    return title_;
}

bool Movie::matches(std::string_view field, std::string_view value) const {
    if (field == "title") {
        return title_ == value;
    }
    if (field == "type") {
        return type() == value;
    }
    return false;
}

// ==================== FeatureFilm ====================
FeatureFilm::FeatureFilm(std::string title, std::string director)
    : Movie(std::move(title)), director_(std::move(director)) {}

std::string_view FeatureFilm::type() const noexcept {
    return "Feature";
}

void FeatureFilm::print(std::ostream& os) const {
    os << "[Игровой] \"" << title_ << "\" | Режиссёр: " << director_;
}

bool FeatureFilm::matches(std::string_view field, std::string_view value) const {
    if (Movie::matches(field, value)) return true;
    if (field == "director") return director_ == value;
    return false;
}

// ==================== Cartoon ====================
Cartoon::Cartoon(std::string title, CreationMethod method)
    : Movie(std::move(title)), method_(method) {}

std::string_view Cartoon::type() const noexcept {
    return "Cartoon";
}

void Cartoon::print(std::ostream& os) const {
    os << "[Мультфильм] \"" << title_ << "\" | Способ создания: " << methodToString(method_);
}

bool Cartoon::matches(std::string_view field, std::string_view value) const {
    if (Movie::matches(field, value)) return true;
    if (field == "method") {
        try {
            return method_ == stringToMethod(value);
        } catch (...) {
            return false;
        }
    }
    return false;
}

// ==================== Series ====================
Series::Series(std::string title, std::string director, int episodes)
    : Movie(std::move(title)), director_(std::move(director)), episodes_(episodes) {}

std::string_view Series::type() const noexcept {
    return "Series";
}

void Series::print(std::ostream& os) const {
    os << "[Сериал] \"" << title_ << "\" | Режиссёр: " << director_
       << " | Серий: " << episodes_;
}

bool Series::matches(std::string_view field, std::string_view value) const {
    if (Movie::matches(field, value)) return true;
    if (field == "director") return director_ == value;
    if (field == "episodes") {
        try {
            return episodes_ == std::stoi(std::string(value));
        } catch (...) {
            return false;
        }
    }
    return false;
}
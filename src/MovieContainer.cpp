#include "include/MovieContainer.h"
#include <algorithm>
#include <iostream>

void MovieContainer::add(std::unique_ptr<Movie> movie) {
    std::cout << "Добавлен: ";
    movie->print(std::cout);
    std::cout << std::endl;
    movies_.push_back(std::move(movie));
}

size_t MovieContainer::remove(std::string_view field, std::string_view value) {
    const auto oldSize = movies_.size();

    auto it = std::remove_if(movies_.begin(), movies_.end(),
        [&](const std::unique_ptr<Movie>& m) {
            return m->matches(field, value);
        });

    movies_.erase(it, movies_.end());
    return oldSize - movies_.size();
}

void MovieContainer::printAll(std::ostream& os) const {
    if (movies_.empty()) {
        os << "Контейнер пуст." << std::endl;
        return;
    }

    os << "=== Содержимое контейнера (" << movies_.size() << ") ===" << std::endl;
    for (size_t i = 0; i < movies_.size(); ++i) {
        os << (i + 1) << ". ";
        movies_[i]->print(os);
        os << std::endl;
    }
}

size_t MovieContainer::size() const noexcept {
    return movies_.size();
}

bool MovieContainer::empty() const noexcept {
    return movies_.empty();
}
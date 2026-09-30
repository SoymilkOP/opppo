#pragma once
#include "Movie.h"
#include <vector>
#include <memory>
#include <string_view>
#include <iostream>

class MovieContainer {
    std::vector<std::unique_ptr<Movie>> movies_;

public:
    void add(std::unique_ptr<Movie> movie);
    size_t remove(std::string_view field, std::string_view value);
    void printAll(std::ostream& os = std::cout) const;

    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
};
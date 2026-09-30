#pragma once
#include "CreationMethod.h"
#include <string>
#include <string_view>
#include <ostream>

class Movie {
protected:
    std::string title_;

public:
    explicit Movie(std::string title);
    virtual ~Movie() = default;

    [[nodiscard]] const std::string& title() const noexcept;

    [[nodiscard]] virtual std::string_view type() const noexcept = 0;
    virtual void print(std::ostream& os) const = 0;

    [[nodiscard]] virtual bool matches(std::string_view field, std::string_view value) const;
};

// ---------- FeatureFilm ----------
class FeatureFilm final : public Movie {
    std::string director_;

public:
    FeatureFilm(std::string title, std::string director);

    [[nodiscard]] std::string_view type() const noexcept override;
    void print(std::ostream& os) const override;
    [[nodiscard]] bool matches(std::string_view field, std::string_view value) const override;
};

// ---------- Cartoon ----------
class Cartoon final : public Movie {
    CreationMethod method_;

public:
    Cartoon(std::string title, CreationMethod method);

    [[nodiscard]] std::string_view type() const noexcept override;
    void print(std::ostream& os) const override;
    [[nodiscard]] bool matches(std::string_view field, std::string_view value) const override;
};

// ---------- Series ----------
class Series final : public Movie {
    std::string director_;
    int episodes_;

public:
    Series(std::string title, std::string director, int episodes);

    [[nodiscard]] std::string_view type() const noexcept override;
    void print(std::ostream& os) const override;
    [[nodiscard]] bool matches(std::string_view field, std::string_view value) const override;
};
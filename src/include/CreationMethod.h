#pragma once
#include <string_view>
#include <string>
#include <stdexcept>

enum class CreationMethod {
    Drawn,
    Puppet,
    Plasticine,
    CGI,
    StopMotion
};

[[nodiscard]] std::string_view methodToString(CreationMethod method) noexcept;
[[nodiscard]] CreationMethod stringToMethod(std::string_view str);
#include "include/CreationMethod.h"
#include <algorithm>
#include <cctype>

std::string_view methodToString(CreationMethod method) noexcept {
    switch (method) {
        case CreationMethod::Drawn:      return "рисованный";
        case CreationMethod::Puppet:     return "кукольный";
        case CreationMethod::Plasticine: return "пластилиновый";
        case CreationMethod::CGI:        return "CGI";
        case CreationMethod::StopMotion: return "покадровая анимация";
    }
    return "неизвестно";
}

CreationMethod stringToMethod(std::string_view str) {
    std::string lower(str);
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    if (lower == "drawn" || lower == "рисованный")         return CreationMethod::Drawn;
    if (lower == "puppet" || lower == "кукольный")         return CreationMethod::Puppet;
    if (lower == "plasticine" || lower == "пластилиновый") return CreationMethod::Plasticine;
    if (lower == "cgi")                                    return CreationMethod::CGI;
    if (lower == "stopmotion" || lower == "stop-motion")   return CreationMethod::StopMotion;

    throw std::runtime_error("Неизвестный способ создания мультфильма: " + std::string(str));
}
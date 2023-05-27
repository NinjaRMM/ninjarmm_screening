#include "step1/Programmer.hpp"

namespace step1 {

Programmer::Programmer(std::initializer_list<std::string> languages)
    : Job<>("Programmer", 8),
    languages_(languages)
{}

std::string Programmer::GetDescription() const {
    std::stringstream ss;
    ss << "writing code in ";
    for (auto& language: languages_) {
        ss << language << ", ";
    }

    auto result = ss.str();
    if (!languages_.empty()) {
        result.resize(result.length() - 2);
    }

    return result;
}

const std::set<std::string>& Programmer::getLanguages() const {
    return languages_;
}

void Programmer::AddLanguage(const std::string& language) {
    languages_.insert(language);
}

bool Programmer::RemoveLanguage(const std::string& language) {
    return languages_.erase(language) > 0;
}

} // namespace step1


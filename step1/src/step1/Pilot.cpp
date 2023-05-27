#include "step1/Pilot.hpp"

namespace step1 {

Pilot::Pilot(unsigned requiredHours, std::initializer_list<Category> categories)
    : Job<>("Pilot", requiredHours) {
    for (auto category : categories) {
        AddCategory(category);
    }
}

std::string Pilot::GetCategoryText(Category category) {
    switch (category) {
        case Category::WEC:
            return "WEC";
        case Category::Formula1:
            return "Formula1";
        case Category::Formula2:
            return "Formula2";
        case Category::Formula3:
            return "Formula3";
        case Category::FormulaE:
            return "FormulaE";
        case Category::WRC:
            return "WRC";
    }

    return "";
}

std::string Pilot::GetDescription() const {
    std::stringstream ss;
    ss << "racing a car in ";

    const auto categories = GetCategories();
    for (auto category: categories) {
        ss << GetCategoryText(category) << ", ";
    }

    auto result = ss.str();
    if (!categories.empty()) {
        result.resize(result.size() - 2);
    }

    return result;
}

bool Pilot::HasCategory(Category category) const {
    return ((uint8_t)category_) & ((uint8_t)category);
}

void Pilot::AddCategory(Category category) {
    category_ = ((uint8_t)category_) | ((uint8_t)category);
}

void Pilot::RemoveCategory(Category category) {
    category_ = ((uint8_t)category_) & ~((uint8_t)category);
}

std::vector<Pilot::Category> Pilot::GetCategories() const {
    std::vector<Pilot::Category> categories;
    for (unsigned i = 1; i <= (uint8_t)Category::WEC; i *= 2) {
        if (auto category = (Pilot::Category)i; HasCategory(category)) {
            categories.push_back(category);
        }
    }

    return categories;
}

} // namespace step1

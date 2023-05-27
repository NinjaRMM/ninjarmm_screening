#pragma once

#include "step1/Job.hpp"
#include <vector>

namespace step1 {

class Pilot : public Job<> {
public:
    enum class Category: uint8_t {
        Formula1 = 1 << 1,
        Formula2 = 1 << 2,
        Formula3 = 1 << 3,
        FormulaE = 1 << 4,
        WRC      = 1 << 5,
        WEC      = 1 << 6,
    };

    static std::string GetCategoryText(Category category);

    Pilot(unsigned requiredHours, std::initializer_list<Category>);

    std::string GetDescription() const override;

    bool HasCategory(Category category) const;

    void AddCategory(Category category);

    void RemoveCategory(Category category);

    std::vector<Category> GetCategories() const;

private:
    uint8_t category_{0};
};

} // namespace step1

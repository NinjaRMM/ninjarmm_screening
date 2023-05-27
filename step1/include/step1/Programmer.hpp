#pragma once

#include "step1/Job.hpp"

#include <initializer_list>
#include <string>
#include <set>

namespace step1 {

class Programmer : public Job<> {
public:
    explicit Programmer(std::initializer_list<std::string> languages);

    std::string GetDescription() const override;

    const std::set<std::string>& getLanguages() const;

    void AddLanguage(const std::string& language);

    bool RemoveLanguage(const std::string& language);

private:
    std::set<std::string> languages_;
};

} // namespace step1

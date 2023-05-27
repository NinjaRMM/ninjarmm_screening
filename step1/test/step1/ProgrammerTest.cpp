#include "step1/Programmer.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(ProgrammerSimpleTest, Construction) {
    step1::Programmer p1({"C++", "Rust", "Go"});
    step1::Programmer p2({"JavaScript", "TypeScript", "PHP", "Python"});

    EXPECT_EQ(p1.getLanguages(), std::set<std::string>({"C++", "Rust", "Go"}));
    EXPECT_EQ(
        p2.getLanguages(),
        std::set<std::string>({"JavaScript", "TypeScript", "PHP", "Python"}));
}

#define LANGUAGES {"C++", "Rust", "Go"}

struct ProgrammerTest: public ::testing::Test {
    std::set<std::string> languages;
    step1::Programmer p;

    ProgrammerTest():
        languages(LANGUAGES),
        p(LANGUAGES) {}
};

TEST_F(ProgrammerTest, AddLanguage) {
    const auto& languagesReference = p.getLanguages();

    EXPECT_EQ(languagesReference, languages);

    p.AddLanguage("Zig");

    EXPECT_NE(languagesReference, languages);
    EXPECT_EQ(languagesReference.size(), languages.size() + 1);

    EXPECT_NE(std::ranges::find(languagesReference, "Zig"),
              languagesReference.end());

    p.AddLanguage("Zig");
    EXPECT_EQ(languagesReference.size(), languages.size() + 1);
}

TEST_F(ProgrammerTest, RemoveLanguage) {
    const auto& languagesReference = p.getLanguages();

    EXPECT_EQ(languagesReference, languages);

    ASSERT_TRUE(p.RemoveLanguage("C++"));

    EXPECT_NE(languagesReference, languages);
    EXPECT_EQ(languagesReference.size(), languages.size() - 1);

    EXPECT_EQ(std::ranges::find(languagesReference, "Zig"),
              languagesReference.end());

    EXPECT_FALSE(p.RemoveLanguage("C++"));
    EXPECT_EQ(languagesReference.size(), languages.size() - 1);
}

TEST_F(ProgrammerTest, GetDescription) {
    EXPECT_EQ(p.GetDescription(), "writing code in C++, Go, Rust");

    step1::Programmer p2({"JavaScript", "TypeScript", "PHP",
                           "Python"});

    EXPECT_EQ(p2.GetDescription(),
              "writing code in JavaScript, PHP, Python, TypeScript");
}

#include "step1/Pilot.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using step1::Pilot;

using Category = Pilot::Category;

struct PilotTest: public ::testing::Test {
    Pilot p;

    PilotTest()
        : p(8, {Category::Formula1, Category::Formula3, Category::WRC}) {}
};

TEST_F(PilotTest, Constructor) {
    EXPECT_EQ(p.GetRequiredHours(), 8);
    EXPECT_TRUE(p.HasCategory(Category::Formula1));
    EXPECT_TRUE(p.HasCategory(Category::Formula3));
    EXPECT_TRUE(p.HasCategory(Category::WRC));

    EXPECT_FALSE(p.HasCategory(Category::Formula2));
    EXPECT_FALSE(p.HasCategory(Category::FormulaE));
    EXPECT_FALSE(p.HasCategory(Category::WEC));
}

TEST_F(PilotTest, GetCategories) {
    std::vector<Category> expected = {
        Category::Formula1,
        Category::Formula3,
        Category::WRC
    };

    EXPECT_EQ(expected, p.GetCategories());
}

TEST_F(PilotTest, AddCategory) {
    std::vector<Category> expected = {
        Category::Formula1,
        Category::Formula2,
        Category::Formula3,
        Category::FormulaE,
        Category::WRC,
        Category::WEC
    };

    p.AddCategory(Category::WEC);

    EXPECT_TRUE(p.HasCategory(Category::WEC));
    EXPECT_EQ(p.GetCategories().size(), 4);

    p.AddCategory(Category::Formula2);
    EXPECT_TRUE(p.HasCategory(Category::Formula2));
    EXPECT_EQ(p.GetCategories().size(), 5);

    p.AddCategory(Category::FormulaE);
    EXPECT_TRUE(p.HasCategory(Category::FormulaE));
    EXPECT_EQ(p.GetCategories().size(), 6);

    EXPECT_EQ(expected, p.GetCategories());
}

TEST_F(PilotTest, RemoveCategory) {
    std::vector<Category> expected;

    p.RemoveCategory(Category::WRC);

    EXPECT_FALSE(p.HasCategory(Category::WRC));
    EXPECT_EQ(p.GetCategories().size(), 2);

    p.RemoveCategory(Category::Formula1);
    EXPECT_FALSE(p.HasCategory(Category::Formula1));
    EXPECT_EQ(p.GetCategories().size(), 1);

    p.RemoveCategory(Category::Formula3);
    EXPECT_FALSE(p.HasCategory(Category::Formula3));
    EXPECT_EQ(p.GetCategories().size(), 0);

    EXPECT_EQ(expected, p.GetCategories());
}

TEST_F(PilotTest, GetDescription) {
    EXPECT_EQ(p.GetDescription(), "racing a car in Formula1, Formula3, WRC");
}

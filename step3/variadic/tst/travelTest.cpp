#include "gtest/gtest.h"
#include "travel.h"

TEST(TravelTestSuite, CalculateCosts) {
    Travel travel;

    travel.addData("Mary", 2.3, 1);
    travel.addData("Tom", 1, 4, 134.2, 9);
    travel.addData("Brad", 10, 12);
    travel.addData("Catherine");
    travel.addData("Rosalia", 30, 10040.6);

    // with floats comparison is not precise and gets error
    ASSERT_NEAR(travel.getTotal(), 10244.1, 0.001);
}

TEST(TravelTestSuite, CalculateEmpty) {
    Travel travel;
    std::string ind = "Mary";

    travel.addData(ind);
    ASSERT_NEAR(travel.getInd(ind), 0.0, 0.001);
}

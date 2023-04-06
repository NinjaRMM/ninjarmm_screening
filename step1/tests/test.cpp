/**
 * @file test.cpp
 *
 * @brief Entry point for unit tests.
 *
 * @author   gustavo.garcia
 * @date     Apr 02, 2023
 * @revision 1
 *
 */

/* INCLUDES ******************************************************************/

#include <gtest/gtest.h>

/* DEFINITION ****************************************************************/

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

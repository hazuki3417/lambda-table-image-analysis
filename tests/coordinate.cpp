#include <gtest/gtest.h>
#include "coordinate.hpp"


TEST(coordinateTest, status)
{
    selen::coordinate coordinate = *new selen::coordinate(1, 2);

    EXPECT_EQ(coordinate.get_x(), 1);
    EXPECT_EQ(coordinate.get_y(), 2);
}

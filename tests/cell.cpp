#include <gtest/gtest.h>
#include "cell.hpp"


TEST(cellTest, status)
{
    selen::coordinate *coordinate = new selen::coordinate(1, 2);
    selen::cell cell = *new selen::cell(coordinate, "testing");

    EXPECT_EQ(cell.get_str(), "testing");
    EXPECT_EQ(cell.get_col_index(), 1);
    EXPECT_EQ(cell.get_row_index(), 2);
    
    delete coordinate;
}

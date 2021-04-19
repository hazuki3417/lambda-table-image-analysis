#include <iostream>
#include "cell.hpp"
#include "coordinate.hpp"

namespace selen
{
    coordinate* coord;
    std::string str;

    cell::cell(selen::coordinate* coord, std::string str)
    {
        this->coord = coord;
        this->str = str;
    }

    int cell::get_row_index()
    {
        return this->coord->get_y();
    }

    int cell::get_col_index()
    {
        return this->coord->get_x();
    }

    std::string cell::get_str()
    {
        return this->str;
    }
}

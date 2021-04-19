#pragma once

#include <iostream>
#include "coordinate.hpp"

namespace selen
{
    class cell
    {
        private:
            coordinate* coord;
            std::string str;
        public:
            cell(coordinate* coord, std::string str);
            int get_row_index();
            int get_col_index();
            std::string get_str();
    };
}

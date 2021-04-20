#ifndef CELL_H
#define CELL_H

#include <iostream>

namespace selen{
    typedef struct
    {
        std::string path;
        std::string str;
        int top;
        int left;
        int right;
        int bottom;
        int width;
        int height;
    } cell;
}

#endif



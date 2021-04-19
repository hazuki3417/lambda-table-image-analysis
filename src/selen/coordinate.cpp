#include <coordinate.hpp>

namespace selen{
    int x;
    int y;
    
    coordinate::coordinate(int x, int y) {
        this->x = x;
        this->y = y;
    }

    int coordinate::get_x(){
        return this->x;
    }
    
    int coordinate::get_y(){
        return this->y;
    }
}

#pragma once

namespace selen{
    class coordinate
    {
        private:
            int x;
            int y;

        public:
            coordinate(int x, int y);
            int get_x();
            int get_y();
    };
}


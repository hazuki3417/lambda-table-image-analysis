#pragma once

#include <iostream>

namespace selen
{
    class workspace
    {
        private:
            std::string path;
        public:
            workspace(std::string path);
            bool create();
            bool remove();
            bool exists();
            std::string get_path();
            ~workspace();
    };
}

#pragma once

#include <iostream>

namespace selen{
    class base64
    {
        public:
            static std::string encode(const std::string &str);
            static std::string decode(const std::string &str);
    };
}


#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "workspace.hpp"

namespace selen
{
    std::string path;

    workspace::workspace(std::string path)
    {
        this->path = path;
    }

    bool workspace::create()
    {
        return boost::filesystem::create_directory(this->path.c_str());
    }

    bool workspace::remove(){
        return boost::filesystem::remove_all(this->path.c_str());
    }

    bool workspace::exists()
    {
        return boost::filesystem::exists(this->path.c_str());
    }

    std::string workspace::get_path()
    {
        return this->path;
    }

    workspace::~workspace()
    {
        this->remove();
    }
}

#include "string_manipulation.hpp"

using namespace aelib;

#include <sstream>
/*
 * Split a string with a delimiter and put the tokens inside a vector
 * */
std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}



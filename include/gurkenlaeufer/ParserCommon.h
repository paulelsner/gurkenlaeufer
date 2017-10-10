#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace gurkenlaeufer {
    namespace detail {
        // from http://stackoverflow.com/a/236803
        inline void split(const std::string& s, char delim, std::vector<std::string>& elems)
        {
            std::stringstream ss;
            ss.str(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                elems.push_back(item);
            }
        }

        inline std::vector<std::string> split(const std::string& s, char delim)
        {
            std::vector<std::string> elems;
            split(s, delim, elems);
            return elems;
        }

        inline std::string toLower(const std::string& input)
        {
            std::string output(input);
            std::transform(input.begin(), input.end(), output.begin(), ::tolower);
            return output;
        }

        //from http://stackoverflow.com/a/17976541
        std::string trim(const std::string& s)
        {
            auto wsfront = std::find_if_not(s.begin(), s.end(), ::isspace);
            return std::string(wsfront, std::find_if_not(s.rbegin(), std::string::const_reverse_iterator(wsfront), ::isspace).base());
        }
    }
}

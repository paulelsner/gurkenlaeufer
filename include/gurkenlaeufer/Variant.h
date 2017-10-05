#pragma once

#include <string>
#include <cstdint>

class Variant {
public:
    Variant(const std::string& input)
        : _input(input)
    {
    }
    int64_t getInt() const
    {
        return std::stol(_input);
    }
    double getDouble() const
    {
        return std::stod(_input);
    }
    const std::string& getString() const
    {
        return _input;
    }

private:
    const std::string _input;
};
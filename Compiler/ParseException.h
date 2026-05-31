#pragma once

#include <stdexcept>
#include <string>
using namespace std;

class ParseException : public runtime_error
{
public:
    ParseException(const string& message)
        : runtime_error(message)
    {
    }
};

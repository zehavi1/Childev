#pragma once  
#include <iostream>  
#include <string>  
#include <exception>  
using namespace std;

class ErrorHandler : public exception
{
private:
    std::string errorMessage;
    int lineNumber;
    mutable std::string formattedMessage; // Store formatted message  

public:
    ErrorHandler(const std::string errorMessage, int lineNumber = 0) {
        this->errorMessage = errorMessage;
        this->lineNumber = lineNumber;
    }

    virtual const char* what() const noexcept override {
        formattedMessage = errorMessage + "\nLine: " + std::to_string(lineNumber);
        return formattedMessage.c_str();
    }
};

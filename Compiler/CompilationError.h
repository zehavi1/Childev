#pragma once

#include <string>
#include <iostream>
using namespace std;

enum class ErrorKind
{
    Lexical,
    Syntax,
};

enum class ErrorSeverity
{
    Warning,
    Error
};

struct CompilationError
{
    ErrorKind kind;
    ErrorSeverity severity;

    string message;
    string lexeme;

    int lineNumber;
    int columnNumber;

    CompilationError(
        ErrorKind kind,
        ErrorSeverity severity,
        string message,
        string lexeme,
        int lineNumber,
        int columnNumber = 0
    )
        : kind(kind),
        severity(severity),
        message(message),
        lexeme(lexeme),
        lineNumber(lineNumber),
        columnNumber(columnNumber)
    {
    }
};

inline string ErrorKindToString(ErrorKind kind)
{
    switch (kind)
    {
    case ErrorKind::Lexical:
        return "Lexical error";
    case ErrorKind::Syntax:
        return "Syntax error";
    case ErrorKind::Semantic:
        return "Semantic error";
    default:
        return "Unknown error";
    }
}

inline string ErrorSeverityToString(ErrorSeverity severity)
{
    switch (severity)
    {
    case ErrorSeverity::Warning:
        return "Warning";
    case ErrorSeverity::Error:
        return "Error";
    default:
        return "Unknown";
    }
}
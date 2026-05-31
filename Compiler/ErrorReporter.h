#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "CompilationError.h"

using namespace std;

class ErrorReporter
{
private:
    vector<CompilationError> errors;

public:
    void report(const CompilationError& error)
    {
        errors.push_back(error);
    }

    void report(
        ErrorKind kind,
        string message,
        string lexeme,
        int lineNumber,
        int columnNumber = 0,
        ErrorSeverity severity = ErrorSeverity::Error
    )
    {
        errors.emplace_back(
            kind,
            severity,
            message,
            lexeme,
            lineNumber,
            columnNumber
        );
    }

    bool hasErrors() const
    {
        return !errors.empty();
    }

    int count() const
    {
        return static_cast<int>(errors.size());
    }

    const vector<CompilationError>& getErrors() const
    {
        return errors;
    }

    void clear()
    {
        errors.clear();
    }

    void printErrors() const
    {
        if (errors.empty())
        {
            cout << "No compilation errors found." << endl;
            return;
        }

        cout << endl;
        cout << "==============================" << endl;
        cout << "COMPILATION ERRORS" << endl;
        cout << "==============================" << endl;

        for (size_t i = 0; i < errors.size(); i++)
        {
            const CompilationError& e = errors[i];

            cout << i + 1 << ". ";
            cout << "[" << ErrorKindToString(e.kind) << "] ";
            cout << "line " << e.lineNumber;

            if (e.columnNumber > 0)
            {
                cout << ", column " << e.columnNumber;
            }

            cout << ": " << e.message;

            //if (!e.lexeme.empty())
            //{
            //    cout << " near '" << e.lexeme << "'";
            //}

            cout << endl;
        }
    }
};
//
// Created by wjc on 2023/3/21.
//

#ifndef CALCULATOR_EXCEPTION_H
#define CALCULATOR_EXCEPTION_H

#include <exception>
#include <string>

class Exception : public std::exception
{
public:
    explicit Exception(const char* message) : message_(message)
    {
        FillStackTrace();
    }
    explicit Exception(const std::string& message) : message_(message)
    {
        FillStackTrace();
    }
    virtual ~Exception() throw()
    {

    }
    virtual const char* what() const throw();
    const char* StackTrace() const throw();
private:
    void FillStackTrace();
    std::string message_;
    std::string stackTrace_;
};

class SyntaxError : public Exception
{
public:
    explicit SyntaxError(const char* message) : Exception(message)
    {

    }

    explicit SyntaxError(const std::string& message) : Exception(message)
    {

    }

    virtual ~SyntaxError() throw()
    {

    }
};

class FileStreamError : public Exception
{
public:
    explicit FileStreamError(const char* message) : Exception(message)
    {

    }

    explicit FileStreamError(const std::string& message) : Exception(message)
    {

    }

    virtual ~FileStreamError() throw()
    {

    }
};

class CheckNumberError : public Exception
{
public:
    explicit CheckNumberError(const char* message) : Exception(message)
    {

    }

    explicit CheckNumberError(const std::string& message) : Exception(message)
    {

    }

    virtual ~CheckNumberError() throw()
    {

    }
};

#endif //CALCULATOR_EXCEPTION_H

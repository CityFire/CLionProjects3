//
// Created by wjc on 2023/3/23.
//

#ifndef CALCULATOR_COMMANDPARSER_H
#define CALCULATOR_COMMANDPARSER_H

#include <string>
#include "Parser.h"
class Scanner;
class Calc;

class CommandParser {
    enum ECommand
    {
        CMD_HELP,
        CMD_QUIT,
        CMD_VAR,
        CMD_FUN,
        CMD_LOAD,
        CMD_SAVE,
        CMD_ERROR
    };
public:
    CommandParser(Scanner& scanner, Calc& calc);
    STATUS Execute();
private:
    void Help() const;
    void ListVar() const;
    void ListFun() const;
    STATUS Load(const std::string& fileName);
    STATUS Save(const std::string& fileName);
    Scanner& scanner_;
    Calc& calc_;
    ECommand cmd_;
    std::string cmdName_;
};


#endif //CALCULATOR_COMMANDPARSER_H

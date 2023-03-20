//
// Created by wjc on 2023/3/20.
//

#include "Parser.h"
#include "Scanner.h"
#include "Node.h"

#include <assert.h>

Parser::Parser(Scanner& scanner) : scanner_(scanner), tree_(0)
{

}

void Parser::Parse()
{
    Expr();
}

Node* Parser::Expr()
{
    return 0;
}

Node* Parser::Term()
{
    return 0;
}

Node* Parser::Factor()
 {
    return 0;
}

double Parser::Calculate() const
{
    assert(tree_ != 0);
    return tree_->Calc();
}
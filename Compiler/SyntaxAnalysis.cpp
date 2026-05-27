#include "SyntaxAnalysis.h"
#include <iostream>

using namespace std;

SyntacticAnalysis::SyntacticAnalysis()
{
}

SyntacticAnalysis::SyntacticAnalysis(const vector<Lexema>& tokens)
    : tokens(tokens)
{
}

SyntacticAnalysis::~SyntacticAnalysis()
{
}

Lexema SyntacticAnalysis::currentToken()
{
    if (currentTokenIndex >= tokens.size())
    {
        Lexema endToken;
        endToken.typeToken = Tok_count;
        endToken.lex = "";
        endToken.nextlex = nullptr;
        return endToken;
    }

    return tokens[currentTokenIndex];
}

Lexema SyntacticAnalysis::peekNextToken()
{
    if (currentTokenIndex + 1 >= tokens.size())
    {
        Lexema endToken;
        endToken.typeToken = Tok_count;
        endToken.lex = "";
        endToken.nextlex = nullptr;
        return endToken;
    }

    return tokens[currentTokenIndex + 1];
}

void SyntacticAnalysis::nextToken()
{
    if (currentTokenIndex < tokens.size())
    {
        currentTokenIndex++;
    }
}

bool SyntacticAnalysis::isEnd()
{
    return currentTokenIndex >= tokens.size()
        || currentToken().typeToken == Tok_count;
}

bool SyntacticAnalysis::isCurrent(Token token)
{
    return currentToken().typeToken == token;
}

Token SyntacticAnalysis::currentTokenType()
{
    return currentToken().typeToken;
}

string SyntacticAnalysis::currentLex()
{
    return currentToken().lex;
}

bool SyntacticAnalysis::isSeparator()
{
    Token t = currentToken().typeToken;

    return t == Tok_newline
        || t == Tok_semicolon
        || t == Tok_comment;
}

bool SyntacticAnalysis::isStatementStart()
{
    switch (currentToken().typeToken)
    {
    case Tok_identifier:
    case Tok_read:
    case Tok_write:
    case Tok_if:
    case Tok_while:
    case Tok_for:
        return true;

    default:
        return false;
    }
}

bool SyntacticAnalysis::isMathOp(const string& op)
{
    return currentToken().typeToken == Tok_math
        && currentToken().lex == op;
}

bool SyntacticAnalysis::isCompOp()
{
    return currentToken().typeToken == Tok_comp;
}

shared_ptr<TokenNode> SyntacticAnalysis::match(Token token, string msg)
{
    if (currentToken().typeToken != token)
    {
        string error = msg;
        error += "\nGot token text: ";
        error += currentToken().lex;
        throw runtime_error(error);
    }

    Lexema t = currentToken();
    nextToken();

    return make_shared<TokenNode>(t);
}

shared_ptr<TokenNode> SyntacticAnalysis::matchMath(const string& op, string msg)
{
    if (currentToken().typeToken != Tok_math || currentToken().lex != op)
    {
        string error = msg;
        error += "\nExpected math operator: ";
        error += op;
        error += "\nGot: ";
        error += currentToken().lex;
        throw runtime_error(error);
    }

    Lexema t = currentToken();
    nextToken();

    return make_shared<TokenNode>(t);
}

shared_ptr<TokenNode> SyntacticAnalysis::matchComp(string msg)
{
    if (currentToken().typeToken != Tok_comp)
    {
        string error = msg;
        error += "\nGot: ";
        error += currentToken().lex;
        throw runtime_error(error);
    }

    Lexema t = currentToken();
    nextToken();

    return make_shared<TokenNode>(t);
}

void SyntacticAnalysis::skipSeparators()
{
    while (isSeparator())
    {
        nextToken();
    }
}
shared_ptr<ASTNode> SyntacticAnalysis::parse()
{
    return program();
}

shared_ptr<ASTNode> SyntacticAnalysis::program()
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("program");

    skipSeparators();

    node->addChild(statementList(Tok_count));

    if (currentToken().typeToken == Tok_count)
    {
        node->addChild(match(Tok_count));
    }

    return node;
}

shared_ptr<ASTNode> SyntacticAnalysis::statementList(Token stopToken)
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("statementList");

    skipSeparators();

    while (!isEnd() && currentToken().typeToken != stopToken)
    {
        if (!isStatementStart())
        {
            throw runtime_error("Expected statement");
        }

        node->addChild(statement());

        if (isSeparator())
        {
            skipSeparators();
        }
        else if (!isEnd() && currentToken().typeToken != stopToken)
        {
            throw runtime_error("Missing separator between statements");
        }
    }

    return node;
}
shared_ptr<ASTNode> SyntacticAnalysis::statement()
{
    switch (currentToken().typeToken)
    {
    case Tok_identifier:
        return assignment();

    case Tok_read:
        return readStatement();

    case Tok_write:
        return writeStatement();

    case Tok_if:
        return ifStatement();

    case Tok_while:
        return whileStatement();

    case Tok_for:
        return forStatement();

    default:
        throw runtime_error("Unexpected token in statement: " + currentToken().lex);
    }
}
shared_ptr<ASTNode> SyntacticAnalysis::assignment()
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("assignment");

    node->addChild(identifierList());
    node->addChild(match(Tok_assign, "Expected '<-' in assignment"));
    node->addChild(expression());

    return node;
}

shared_ptr<ASTNode> SyntacticAnalysis::identifierList()
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("identifierList");

    node->addChild(match(Tok_identifier, "Expected identifier"));

    while (currentToken().typeToken == Tok_comma)
    {
        node->addChild(match(Tok_comma));
        node->addChild(match(Tok_identifier, "Expected identifier after comma"));
    }

    return node;
}
shared_ptr<ASTNode> SyntacticAnalysis::readStatement()
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("read");

    node->addChild(match(Tok_read));
    node->addChild(match(Tok_Left_paren, "Expected '(' after read"));
    node->addChild(argsOpt());
    node->addChild(match(Tok_Right_paren, "Expected ')' after read arguments"));

    return node;
}

shared_ptr<ASTNode> SyntacticAnalysis::writeStatement()
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("write");

    node->addChild(match(Tok_write));
    node->addChild(match(Tok_Left_paren, "Expected '(' after write"));
    node->addChild(argsOpt());
    node->addChild(match(Tok_Right_paren, "Expected ')' after write arguments"));

    return node;
}

shared_ptr<ASTNode> SyntacticAnalysis::argsOpt()
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("argsOpt");

    if (currentToken().typeToken == Tok_Right_paren)
    {
        return node;
    }

    node->addChild(expressionList());

    return node;
}

shared_ptr<ASTNode> SyntacticAnalysis::expressionList()
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("expressionList");

    node->addChild(expression());

    while (currentToken().typeToken == Tok_comma)
    {
        node->addChild(match(Tok_comma));
        node->addChild(expression());
    }

    return node;
}





shared_ptr<ASTNode> SyntacticAnalysis::ifStatement()
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("if");

    node->addChild(match(Tok_if));
    node->addChild(expression());
    node->addChild(block());
    node->addChild(elsePartOpt());

    return node;
}

shared_ptr<ASTNode> SyntacticAnalysis::elsePartOpt()
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("elsePartOpt");

    size_t savedIndex = currentTokenIndex;

    skipSeparators();

    if (currentToken().typeToken == Tok_else)
    {
        node->addChild(match(Tok_else));
        node->addChild(block());
        return node;
    }

    currentTokenIndex = savedIndex;
    return node;
}

shared_ptr<ASTNode> SyntacticAnalysis::whileStatement()
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("while");

    node->addChild(match(Tok_while));
    node->addChild(expression());
    node->addChild(block());

    return node;
}

shared_ptr<ASTNode> SyntacticAnalysis::forStatement()
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("for");

    node->addChild(match(Tok_for));

    if (currentToken().typeToken == Tok_Left_paren)
    {
        node->addChild(match(Tok_Left_paren));

        node->addChild(match(Tok_identifier, "Expected loop variable"));
        node->addChild(match(Tok_assign, "Expected '<-' in for loop"));
        node->addChild(expression());
        node->addChild(match(Tok_to, "Expected 'to' in for loop"));
        node->addChild(expression());

        node->addChild(match(Tok_Right_paren, "Expected ')' after for header"));
    }
    else
    {
        node->addChild(match(Tok_identifier, "Expected loop variable"));
        node->addChild(match(Tok_assign, "Expected '<-' in for loop"));
        node->addChild(expression());
        node->addChild(match(Tok_to, "Expected 'to' in for loop"));
        node->addChild(expression());
    }

    node->addChild(block());

    return node;
}

shared_ptr<ASTNode> SyntacticAnalysis::block()
{
    shared_ptr<ParentNode> node = make_shared<ParentNode>("block");

    node->addChild(match(Tok_Colon, "Expected ':' at the beginning of block"));

    node->addChild(statementList(Tok_block_end));

    node->addChild(match(Tok_block_end, "Expected '||' at the end of block"));

    return node;
}
shared_ptr<ASTNode> SyntacticAnalysis::expression()
{
    return exprOr();
}

shared_ptr<ASTNode> SyntacticAnalysis::exprOr()
{
    shared_ptr<ASTNode> left = exprAnd();

    while (currentToken().typeToken == Tok_or)
    {
        Lexema op = currentToken();
        nextToken();

        shared_ptr<ASTNode> right = exprAnd();

        left = make_shared<BinaryOpNode>("or", op, left, right);
    }

    return left;
}

shared_ptr<ASTNode> SyntacticAnalysis::exprAnd()
{
    shared_ptr<ASTNode> left = exprComparison();

    while (currentToken().typeToken == Tok_and)
    {
        Lexema op = currentToken();
        nextToken();

        shared_ptr<ASTNode> right = exprComparison();

        left = make_shared<BinaryOpNode>("and", op, left, right);
    }

    return left;
}

shared_ptr<ASTNode> SyntacticAnalysis::exprComparison()
{
    shared_ptr<ASTNode> left = exprAdd();

    if (isCompOp())
    {
        Lexema op = currentToken();
        nextToken();

        shared_ptr<ASTNode> right = exprAdd();

        left = make_shared<BinaryOpNode>("comparison", op, left, right);
    }

    return left;
}

shared_ptr<ASTNode> SyntacticAnalysis::exprAdd()
{
    shared_ptr<ASTNode> left = exprMul();

    while (isMathOp("+") || isMathOp("-"))
    {
        Lexema op = currentToken();
        nextToken();

        shared_ptr<ASTNode> right = exprMul();

        left = make_shared<BinaryOpNode>("add", op, left, right);
    }

    return left;
}

shared_ptr<ASTNode> SyntacticAnalysis::exprMul()
{
    shared_ptr<ASTNode> left = exprPower();

    while (isMathOp("*") || isMathOp("/"))
    {
        Lexema op = currentToken();
        nextToken();

        shared_ptr<ASTNode> right = exprPower();

        left = make_shared<BinaryOpNode>("mul", op, left, right);
    }

    return left;
}

shared_ptr<ASTNode> SyntacticAnalysis::exprPower()
{
    shared_ptr<ASTNode> left = exprUnary();

    if (isMathOp("^"))
    {
        Lexema op = currentToken();
        nextToken();

        shared_ptr<ASTNode> right = exprPower();

        left = make_shared<BinaryOpNode>("power", op, left, right);
    }

    return left;
}

shared_ptr<ASTNode> SyntacticAnalysis::exprUnary()
{
    if (isMathOp("-"))
    {
        Lexema op = currentToken();
        nextToken();

        shared_ptr<ASTNode> expr = exprUnary();

        return make_shared<UnaryOpNode>(op, expr);
    }

    return primary();
}

shared_ptr<ASTNode> SyntacticAnalysis::primary()
{
    switch (currentToken().typeToken)
    {
    case Tok_num:
    case Tok_string:
    case Tok_char:
    case Tok_true:
    case Tok_false:
    case Tok_identifier:
    {
        Lexema t = currentToken();
        nextToken();
        return make_shared<TokenNode>(t);
    }

    case Tok_Left_paren:
    {
        shared_ptr<ParentNode> node = make_shared<ParentNode>("parenExpression");

        node->addChild(match(Tok_Left_paren));
        node->addChild(expression());
        node->addChild(match(Tok_Right_paren, "Expected ')' after expression"));

        return node;
    }

    default:
        throw runtime_error("Unexpected token in expression: " + currentToken().lex);
    }
}
void SyntacticAnalysis::printASTNodes(const shared_ptr<ASTNode>& node)
{
    if (node != nullptr)
    {
        node->printASTNode();
    }
}


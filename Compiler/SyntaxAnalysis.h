#pragma once

#include <vector>
#include <string>
#include <memory>
#include "enums.h"
#include "ASTNode.h"
#include "Token.h"
#include "ErrorReporter.h"
#include "ParseException.h"

using namespace std;

class SyntacticAnalysis
{
public:
    SyntacticAnalysis(const Token* tokens, shared_ptr<ErrorReporter> errorReporter);

    // הפונקציה הראשית
    shared_ptr<ASTNode> parse();

    void printASTNodes(const shared_ptr<ASTNode>& node);

private:
    const Token* tokens;   // רשימת הלקסמות
    const Token* current;
    shared_ptr<ErrorReporter> errorReporter;

    void reportSyntaxError(const string& message);
    void recoverToNextStatementOrStop(TokenType stopToken);
    void failSyntax(const string& message);
    // פעולות עזר כלליות
    
    Token currentToken();


    void nextToken();

    bool isEnd();

    bool isCurrent(TokenType token);

    bool isSeparator();

    bool isStatementStart();

    string currentLex();

    TokenType currentTokenType();

    // התאמה לפי סוג טוקן
    shared_ptr<TokenNode> match(TokenType token, string msg = "Unexpected token");

    bool isMathOp(const string& op);

    bool isCompOp();

    // Separators
    //
    //
    //

    // Separator ::= Tok_newline | Tok_semicolon | Tok_comment
    shared_ptr<ASTNode> separator();

    // Separators ::= Separator { Separator }
    shared_ptr<ASTNode> separators();

    // SeparatorsOpt ::= { Separator }
    shared_ptr<ASTNode> separatorsOpt();

    void skipSeparators();

    
    // Program
    //
    //
    //

    // Program ::= SeparatorsOpt StatementList Tok_count
    shared_ptr<ASTNode> program();

    // StatementList ::= SeparatorsOpt [ Statement { Separators Statement } SeparatorsOpt ]
    shared_ptr<ASTNode> statementList(TokenType stopToken);

    
    // Statements
    //
    //
    //
    //
    //


    // Statement ::= Assignment
    //             | ReadStatement
    //             | WriteStatement
    //             | IfStatement
    //             | WhileStatement
    //             | ForStatement
    shared_ptr<ASTNode> statement();

    // Assignment ::= IdentifierList Tok_assign Expression
    shared_ptr<ASTNode> assignment();

    // IdentifierList ::= Tok_identifier { Tok_comma Tok_identifier }
    shared_ptr<ASTNode> identifierList();

    // ReadStatement ::= Tok_read Tok_Left_paren ArgsOpt Tok_Right_paren
    shared_ptr<ASTNode> readStatement();

    // WriteStatement ::= Tok_write Tok_Left_paren ArgsOpt Tok_Right_paren
    shared_ptr<ASTNode> writeStatement();

    // IfStatement ::= Tok_if Expression Block ElsePartOpt
    shared_ptr<ASTNode> ifStatement();

    // ElsePartOpt ::= ε
    //               | SeparatorsOpt Tok_else Block
    shared_ptr<ASTNode> elsePartOpt();

    // WhileStatement ::= Tok_while Expression Block
    shared_ptr<ASTNode> whileStatement();

    // ForStatement ::= Tok_for Tok_identifier Tok_assign Expression Tok_to Expression Block
    //                | Tok_for Tok_Left_paren Tok_identifier Tok_assign Expression Tok_to Expression Tok_Right_paren Block
    shared_ptr<ASTNode> forStatement();

    // Block ::= Tok_Colon StatementList Tok_block_end
    shared_ptr<ASTNode> block();

    // ArgsOpt ::= ε | ExpressionList
    shared_ptr<ASTNode> argsOpt();

    // ExpressionList ::= Expression { Tok_comma Expression }
    shared_ptr<ASTNode> expressionList();

    
    // Expressions
    // 
    //
    //
    //
    //
    //
    //

    
    // Expression ::= OrExpression
    shared_ptr<ASTNode> expression();

    // OrExpression ::= AndExpression { Tok_or AndExpression }
    shared_ptr<ASTNode> exprOr();

    // AndExpression ::= CompareExpression { Tok_and CompareExpression }
    shared_ptr<ASTNode> exprAnd();

    // CompareExpression ::= AddExpression [ Tok_comp AddExpression ]
    shared_ptr<ASTNode> exprComparison();

    // AddExpression ::= MulExpression { Tok_math("+") MulExpression
    //                                 | Tok_math("-") MulExpression }
    shared_ptr<ASTNode> exprAdd();

    // MulExpression ::= PowerExpression { Tok_math("*") PowerExpression
    //                                   | Tok_math("/") PowerExpression }
    shared_ptr<ASTNode> exprMul();

    // PowerExpression ::= UnaryExpression [ Tok_math("^") PowerExpression ]
    shared_ptr<ASTNode> exprPower();

    // UnaryExpression ::= Tok_math("-") UnaryExpression
    //                   | Primary
    shared_ptr<ASTNode> exprUnary();

    // Primary ::= Tok_num
    //           | Tok_string
    //           | Tok_char
    //           | Tok_true
    //           | Tok_false
    //           | Tok_identifier
    //           | Tok_Left_paren Expression Tok_Right_paren
    shared_ptr<ASTNode> primary();
};
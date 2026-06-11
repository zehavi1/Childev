#include "SyntaxAnalysis.h"
#include <iostream>

using namespace std;


SyntacticAnalysis::SyntacticAnalysis(
	const Token* tokens,
	shared_ptr<ErrorReporter> errorReporter
)
	: tokens(tokens),
	errorReporter(errorReporter), current(tokens)
{
}



Token SyntacticAnalysis::currentToken()
{
	if (current->nextlex == nullptr)
	{
		Token endToken;
		endToken.typeToken = Tok_EOF;
		endToken.lex = "";
		endToken.nextlex = nullptr;
		return endToken;
	}
	return *current;
}

void SyntacticAnalysis::nextToken()
{
	if (current->nextlex)
	{
		current = current->nextlex;
	}
}

bool SyntacticAnalysis::isEnd()
{
	return current->nextlex == nullptr
		|| currentToken().typeToken == Tok_EOF;
}

bool SyntacticAnalysis::isCurrent(TokenType token)
{
	return currentToken().typeToken == token;
}

TokenType SyntacticAnalysis::currentTokenType()
{
	return currentToken().typeToken;
}

string SyntacticAnalysis::currentLex()
{
	return currentToken().lex;
}

bool SyntacticAnalysis::isSeparator()
{
	TokenType t = currentToken().typeToken;

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

shared_ptr<TokenNode> SyntacticAnalysis::match(TokenType expected, string msg)
{
	if (currentToken().typeToken != expected)
	{
		string fullMsg = msg + ". Got '" + currentToken().lex + "'";
		failSyntax(fullMsg);
	}

	Token t = currentToken();
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

	node->addChild(statementList(Tok_EOF));

	if (currentToken().typeToken == Tok_EOF)
	{
		node->addChild(match(Tok_EOF));
	}

	return node;
}


shared_ptr<ASTNode> SyntacticAnalysis::statementList(TokenType stopToken)
{
	auto node = make_shared<ParentNode>("statementList");

	skipSeparators();

	while (!isEnd())
	{
		TokenType t = currentToken().typeToken;

		// סוף הרשימה שהקורא ביקש.
		// לא אוכלים אותו. הקורא יאכל.
		if (t == stopToken || t == Tok_EOF)
		{
			break;
		}

		// אם אנחנו בתוך בלוק ופתאום הגיע else,
		// כנראה חסר || לפני else.
		// עוצרים כדי ש-block ידווח שחסר ||.
		if (stopToken == Tok_block_end && t == Tok_else)
		{
			break;
		}

		try
		{
			if (!isStatementStart())
			{
				string msg = "Expected statement, got '" + currentToken().lex + "'";
				failSyntax(msg);
			}

			node->addChild(statement());

			if (isSeparator())
			{
				skipSeparators();
			}
			else if (!isEnd() &&
				currentToken().typeToken != stopToken &&
				currentToken().typeToken != Tok_EOF)
			{
				string msg = "Missing separator between statements";
				failSyntax(msg);
			}
		}
		catch (const ParseException& ex)
		{
			auto errorNode = make_shared<ParentNode>("errorStatement");
			errorNode->addChild(make_shared<SentenceNode>(ex.what()));
			node->addChild(errorNode);

			recoverToNextStatementOrStop(stopToken);
			skipSeparators();
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

	skipSeparators();
	if (currentToken().typeToken == Tok_else)
	{
		node->addChild(match(Tok_else));
		node->addChild(block());
		return node;
	}
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
		Token op = currentToken();
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
		Token op = currentToken();
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
		Token op = currentToken();
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
		Token op = currentToken();
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
		Token op = currentToken();
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
		Token op = currentToken();
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
		Token op = currentToken();
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
		Token t = currentToken();
		nextToken();
		return make_shared<TokenNode>(t);
	}

	case Tok_Left_paren:
	{
		auto node = make_shared<ParentNode>("parenExpression");

		node->addChild(match(Tok_Left_paren));
		node->addChild(expression());
		node->addChild(match(Tok_Right_paren, "Expected ')' after expression"));

		return node;
	}

	default:
	{
		string msg = "Expected expression, got '" + currentToken().lex + "'";
		failSyntax(msg);
	}
	}
}
void SyntacticAnalysis::printASTNodes(const shared_ptr<ASTNode>& node)
{
	if (node != nullptr)
	{
		node->printASTNode();
	}
}
void SyntacticAnalysis::reportSyntaxError(const string& message)
{
	Token t = currentToken();

	if (errorReporter != nullptr)
	{
		errorReporter->report(
			ErrorKind::Syntax,
			message,
			t.lex,
			t.lineNumber
		);
	}
}

void SyntacticAnalysis::failSyntax(const string& message)
{
	reportSyntaxError(message);
	throw ParseException(message);
}
void SyntacticAnalysis::recoverToNextStatementOrStop(TokenType stopToken)
{
	while (!isEnd())
	{
		TokenType t = currentToken().typeToken;

		// הגענו לסוף שהקורא מחכה לו — לא אוכלים.
		if (t == stopToken || t == Tok_EOF)
		{
			return;
		}

		// סוף פקודה — אוכלים מפרידים וחוזרים.
		if (t == Tok_newline || t == Tok_semicolon)
		{
			skipSeparators();
			return;
		}

		// || בתוך בלוק — לא אוכלים, block צריך לקבל אותו.
		// || מחוץ לבלוק — אוכלים אותו כדי לא להיתקע.
		if (t == Tok_block_end)
		{
			if (stopToken == Tok_block_end)
			{
				return;
			}

			nextToken();
			return;
		}

		// else בתוך בלוק — לא אוכלים, כדי ש-block ידווח שחסר ||
		// ואז if/else יוכלו להמשיך.
		// else מחוץ לבלוק — אוכלים כדי לא להיתקע.
		if (t == Tok_else)
		{
			if (stopToken == Tok_block_end)
			{
				return;
			}

			nextToken();
			return;
		}

		nextToken();
	}
}


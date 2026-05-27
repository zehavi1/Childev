#pragma once
#include <map>
#include <string>


//struct expression
//{
//	string type;
//	string value;
//};
enum Token {
	Tok_identifier,

	Tok_else,
	Tok_false,
	Tok_for,
	Tok_if,
	Tok_read,
	Tok_to,
	Tok_true,
	Tok_write,
	Tok_while,

	Tok_num,
	Tok_char,
	Tok_string,

	Tok_assign,      // <-
	Tok_math,        // + - * / ^
	Tok_comp,        // = != < > <= >= => =<
	Tok_or,          // |
	Tok_and,         // &

	Tok_block_end,   // ||
	Tok_Left_paren,  // (
	Tok_Right_paren, // )
	Tok_comma,       // ,
	Tok_Colon,       // :
	Tok_comment,     // { ... }

	Tok_newline,     // ירידת שורה
	Tok_semicolon,   // ; אם כן כתבו
	Tok_error,
	Tok_count
};
extern const char* TokenNames[Tok_count];


enum ASTNodeType {
	RANGE,
	ASTNODE,
	EXPRESSION,
	TERM,
	FACTOR,
	NUMBER,
	TYPE,
	DECLARATION,
	VARIABLE,
	VARIABLE_LIST,
	PRINT_STATEMENT,
	VARIABLE_DECLARATION,
	EXPR_PRINT,
	EXPRESSION_IN_PRINT,
	ASSIGNMENT,
	STATEMENT,
	IF_ELSE_STATEMENT,
	ELIF_STATEMENT,
	EXPR_OR,
	EXPR_AND,
	EXPR_COMPARISON,
	EXPR_ARITHMETIC,
	COMPARISON_OPERATOR,
	BLOCK,
	FOR_LOOP,
	WHILE_LOOP,
	FOREACH_LOOP,
	FUNCTION_DEFINITION,
	COLLECTION,
	PARAMETER_LIST,
	FULL_ARRAY,
	STATIC_ARRAY_DECLARATION,
	DYNAMIC_ARRAY_DECLARATION,
	POINTER_ARRAY_DECLARATION,
	ALLOCATION_FUNCTION_CALL,
	STRING_DECLARATION,
	RETURN_STATEMENT,
	PRINTF_STATEMENT
};
extern std::string ASTNodeTypeNames[];





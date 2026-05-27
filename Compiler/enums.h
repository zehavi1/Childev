#pragma once
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
	Tok_math,
	Tok_comp,
	Tok_eq,// שווה
	Tok_log,
	Tok_end,
	Tok_Right_paren,//(
	Tok_Left_paren,//)
	Tok_comma,//,
	Tok_Colon,//:
	Tok_coment,//הערה
	Tok_error
};

enum Pattern {
	// מילים שמורות
	//סוגי משתנים
	TOK_TYPE_PRINT,
	TOK_NUM_TYPE,
	TOK_CHAR_TYPE,
	TOK_BOOL_TYPE,
	TOK_FLOAT_TYPE,
	TOK_STRING_TYPE,
	TOK_LONG_TYPE,
	//מילים שמורות נוספות

	TOK_DOUBLE,

	TOK_TRUE,
	TOK_RETURN,
	TOK_PRINT,
	TOK_READ,
	TOK_WRITE,
	TOK_NEW,

	TOK_IF,
	TOK_FOR,
	TOK_FLOAT,
	TOK_FALSE,
	TOK_ELSE,
	TOK_ELIF,
	TOK_CHAR,
	TOK_BOOL,
	TOK_WHILE,
	TOK_FOREACH,
	TOK_IN,
	//טוקנים ללא מילה שמורה
	TOK_ERROR,
	TOK_ID,
	TOK_STRING,
	// טוקנים לא אלפביתיים
	TOK_EQUAL,
	TOK_NOT_EQUAL,
	TOK_GREATER_EQUAL,
	TOK_LESS_EQUAL,
	TOK_AND,
	TOK_OR,
	TOK_SEMICOLON,
	TOK_COMMA,
	TOK_OPEN_CURLY,
	TOK_CLOSE_CURLY,
	TOK_OPEN_PAREN,
	TOK_CLOSE_PAREN,
	TOK_PLUS,
	TOK_MINUS,
	TOK_ASTERISK,
	TOK_SLASH,
	TOK_ASSIGN,
	TOK_LESS,
	TOK_GREATER,
	TOK_STRING_LITERAL,
	TOK_RIGHT_ARRAY,
	TOK_LEFT_ARRAY,
	TOK_PERCENT,
	TOK_EOF,
	TOK_UNKNOWN,
	TOK_INCREMENT,
	TOK_DECREMENT,
	TOK_MALLOC,
	TOK_CALLOC,
	TOK_REALLOC
};
const std::string tokenNames[] = {
	"TOK_TYPE_PRINT",
	"TOK_DOUBLE_TYPE",
	"TOK_INT_TYPE",
	"TOK_CHAR_TYPE",
	"TOK_BOOL_TYPE",
	"TOK_FLOAT_TYPE",
	"TOK_STRING_TYPE",
	"TOK_LONG_TYPE",

	"TOK_DOUBLE",

	"TOK_TRUE",
	"TOK_RETURN",
	"TOK_PRINT",
	"TOK_PRINTF",
	"TOK_SCANF",
	"TOK_NEW",
	"TOK_LONG",
	"TOK_INT",
	"TOK_IFRANGE",
	"TOK_IF",
	"TOK_FOR",
	"TOK_FLOAT",
	"TOK_FALSE",
	"TOK_ELSE",
	"TOK_ELIF",
	"TOK_CHAR",
	"TOK_BOOL",
	"TOK_WHILE",
	"TOK_FOREACH",
	"TOK_IN",
	"TOK_ERROR",
	"TOK_ID",
	"TOK_STRING",
	"TOK_EQUAL",
	"TOK_NOT_EQUAL",
	"TOK_GREATER_EQUAL",
	"TOK_LESS_EQUAL",
	"TOK_AND",
	"TOK_OR",
	"TOK_SEMICOLON",
	"TOK_COMMA",
	"TOK_OPEN_CURLY",
	"TOK_CLOSE_CURLY",
	"TOK_OPEN_PAREN",
	"TOK_CLOSE_PAREN",
	"TOK_PLUS",
	"TOK_MINUS",
	"TOK_ASTERISK",
	"TOK_SLASH",
	"TOK_ASSIGN",
	"TOK_LESS",
	"TOK_GREATER",
	"TOK_STRING_LITERAL",
	"TOK_RIGHT_ARRAY",
	"TOK_LEFT_ARRAY",
	"TOK_PERCENT",
	"TOK_EOF",
	"TOK_UNKNOWN",
	"TOK_INCREMENT",
	"TOK_DECREMENT",
	"TOK_MALLOC",
	"TOK_CALLOC",
	"TOK_REALLOC"
};
const std::map<std::string, Pattern> mapAlphaTokens = {
	
	{"string", TOK_STRING_TYPE},
	{"return", TOK_RETURN},
	{"print", TOK_PRINT},
	
	{"new", TOK_NEW},
	{"long", TOK_LONG_TYPE},
	
	{"if", TOK_IF},
	{"for", TOK_FOR},
	{"while", TOK_WHILE},
	{"foreach", TOK_FOREACH},
	{"in", TOK_IN},
	{"float", TOK_FLOAT_TYPE},
	{"else", TOK_ELSE},
	{"elif", TOK_ELIF},
	{"char", TOK_CHAR_TYPE},
	{"bool", TOK_BOOL_TYPE},
	{"true", TOK_BOOL},
	{"false", TOK_BOOL},
	{"malloc",TOK_MALLOC},
	{"calloc",TOK_CALLOC},
	{"realloc",TOK_REALLOC}
};
const std::map<std::string, Pattern> mapNonAlphaTokens = {
{"==", TOK_EQUAL},
{"!=", TOK_NOT_EQUAL},
{">=", TOK_GREATER_EQUAL},
{"<=", TOK_LESS_EQUAL},
{"&&", TOK_AND},
{"||", TOK_OR},
{";", TOK_SEMICOLON},
{",", TOK_COMMA},
{"{", TOK_OPEN_CURLY},
{"}", TOK_CLOSE_CURLY},
{"(", TOK_OPEN_PAREN},
{")", TOK_CLOSE_PAREN},
{"+", TOK_PLUS},
{"-", TOK_MINUS},
{"*", TOK_ASTERISK},
{"/", TOK_SLASH},
{"=", TOK_ASSIGN},
{"<", TOK_LESS},
{">", TOK_GREATER},
{"\"", TOK_STRING_LITERAL},
{"[",TOK_LEFT_ARRAY},
{"]",TOK_RIGHT_ARRAY},
{"%",TOK_PERCENT},
{"++",TOK_INCREMENT},
{"--",TOK_DECREMENT}
};
const std::map<Pattern, Pattern> mapTypes = {
	{TOK_BOOL_TYPE, TOK_BOOL},
	{TOK_CHAR_TYPE, TOK_CHAR},
	
	{TOK_FLOAT_TYPE, TOK_FLOAT},
	{TOK_STRING_TYPE, TOK_STRING} };
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





#pragma once
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <iostream>
#include "enums.h"
#include "Lexema.h"
using namespace std;
// מחלקת בסיס לכל הצמתים

struct ASTNode {
public:
	ASTNodeType nodeType = ASTNODE;
	virtual void printASTNode(int depth = 0) = 0;
	virtual ~ASTNode() {}
	virtual string printOriginalCode(int tabs) const = 0;
	virtual void changeChild(shared_ptr<ASTNode> child, int index) = 0;

};
void printTabsDepth(int depth) {
	for (int i = 0; i < depth; ++i) {
		cout << "  ";
	}
}
struct TokenNode :ASTNode {
public:
	Lexema token;
	TokenNode() {}
	/*TokenNode(Variable v) {
		token = v.token;
	}*/
	TokenNode(const Lexema& token, ASTNodeType nodeType = ASTNODE) :token(token) { this->nodeType = nodeType; }
	void printASTNode(int depth = 0) {
		printTabsDepth(depth);
		cout << "TokenNode: " << token.lex << endl;
	}
	string printOriginalCode(int tabs) const override {
		//string s(tabs, ' ');
		//if (token.typeToken == Tok_newline)
		//	return token.lex + "\n" + s;
		////else if (token.typeToken == TOK_OPEN_CURLY)
		////	return "\n" + s + token.lex+ "\n" + s + " ";
		////else if (token.typeToken == TOK_CLOSE_CURLY)
		////	return  s + token.lex + "\n" + s;
		////else if (mapAlphaTokens.find(token.lex) != mapAlphaTokens.end())
		//	return token.lex + " ";
		//else
		//	return token.lex;
		return "aa";
	}
	void changeChild(shared_ptr<ASTNode> child, int index) override {};
};
struct ParentNode :ASTNode {
public:
	string name;
	vector<shared_ptr<ASTNode>> children;
	//map<string, Variable> variableScope;
	ParentNode(const string& name, vector<shared_ptr<ASTNode>> children, ASTNodeType nodeType = ASTNODE) :name(name), children(children) { this->nodeType = nodeType; }
	ParentNode(const string& name, ASTNodeType nodeType = ASTNODE) :name(name) { this->nodeType = nodeType; }
public:
	void printASTNode(int depth = 0) {
		printTabsDepth(depth);
		cout << "ParentNode: " << ASTNodeTypeNames[nodeType] << endl;
		for (auto child : children) {
			child->printASTNode(depth + 1);
		}
	}
	string printOriginalCode(int tabs) const override {
		string s = "";
		if (name == "block")
			tabs++;
		for (const auto& child : children) {
			s += child->printOriginalCode(tabs);
		}
		return s;
	}
	void addChild(shared_ptr<ASTNode> child) {
		children.push_back(child);
	}
	void changeChild(shared_ptr<ASTNode> child, int index)
	{
		children[index] = child;
	}
	void insertChild(shared_ptr<ASTNode> child, size_t index) {
		// בדוק אם האינדקס בתווך החוקי
		if (index <= children.size()) {
			// הוסף את הילד לאינדקס i
			children.insert(children.begin() + index, child);
		}
		else {
			throw out_of_range("Index is out of range");
		}
	}

};

// צומת עבור ביטויים בינאריים
struct BinaryOpNode : ASTNode {
public:
	string name = "BinaryOpNode";
	Lexema op;
	shared_ptr<ASTNode> left;
	shared_ptr<ASTNode> right;
	BinaryOpNode(const Lexema& op, shared_ptr<ASTNode> left, shared_ptr<ASTNode> right, ASTNodeType nodeType = ASTNODE)
		: op(op), left(left), right(right) {
		this->nodeType = nodeType;
	}
	BinaryOpNode(string name, const Lexema& op, shared_ptr<ASTNode> left, shared_ptr<ASTNode> right, ASTNodeType nodeType = ASTNODE)
		:name(name), op(op), left(left), right(right) {
		this->nodeType = nodeType;
	}
	string printOriginalCode(int tabs) const override {
		string s;
		s = left->printOriginalCode(tabs);
		s += op.lex;
		s += right->printOriginalCode(tabs);
		return s;
	}
	void printASTNode(int depth = 0) {
		printTabsDepth(depth + 1);
		cout << ASTNodeTypeNames[nodeType] << endl;
		printTabsDepth(depth + 1);
		cout << "Left: " << endl;
		left->printASTNode(depth + 1);
		printTabsDepth(depth);
		cout << "BinaryOpNode: " << op.lex << endl;
		printTabsDepth(depth + 1);
		cout << "Right: " << endl;
		right->printASTNode(depth + 1);
	}
	void changeChild(shared_ptr<ASTNode> child, int index)
	{
		if (index == 0)
			left = child;
		else
			right = child;
	}
};
struct SentenceNode :ASTNode
{
	string content;
	SentenceNode(string content, ASTNodeType nodeType = ASTNODE) :content(content) { this->nodeType = nodeType; };
	void printASTNode(int depth = 0) {
		printTabsDepth(depth);
		cout << "SentenceNode: " << content << endl;
	}
	string printOriginalCode(int tabs) const override {
		string s(tabs, ' ');
		return content;
	}
	void changeChild(shared_ptr<ASTNode> child, int index) override {};
};


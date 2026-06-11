#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>
#include "enums.h"
#include "Token.h"

using namespace std;

// מחלקת בסיס לכל הצמתים בעץ התחבירי
struct ASTNode {
public:
    virtual void printASTNode(int depth = 0) = 0;
    virtual ~ASTNode() {}
};

// חשוב שיהיה inline כי זו פונקציה שמוגדרת בתוך h
inline void printTabsDepth(int depth) {
    for (int i = 0; i < depth; ++i) {
        cout << "  ";
    }
}

// צומת שמייצג טוקן בודד
struct TokenNode : ASTNode {
public:
    Token token;

    TokenNode() {  }

    TokenNode(const Token& token) : token(token) {}

    void printASTNode(int depth = 0) override {
        printTabsDepth(depth);
        cout << "TokenNode: " << token.lex << endl;
    }

   
};

// צומת אב כללי שמחזיק רשימת ילדים
struct ParentNode : ASTNode {
public:
    string name;
    vector<shared_ptr<ASTNode>> children;

    ParentNode(const string& name)
        : name(name) {
    }

    ParentNode(const string& name, vector<shared_ptr<ASTNode>> children)
        : name(name), children(children) {
    }

    void printASTNode(int depth = 0) override {
        printTabsDepth(depth);
        cout << "ParentNode: " << name << endl;

        for (auto child : children) {
            if (child != nullptr)
                child->printASTNode(depth + 1);
        }
    }

    

    void addChild(shared_ptr<ASTNode> child) {
        children.push_back(child);
    }

  

    void insertChild(shared_ptr<ASTNode> child, size_t index) {
        if (index <= children.size()) {
            children.insert(children.begin() + index, child);
        }
        else {
            throw out_of_range("Index is out of range");
        }
    }
};

// צומת עבור ביטוי בינארי: x + y, x > y, a & b וכו'
struct BinaryOpNode : ASTNode {
public:
    string name = "BinaryOpNode";
    Token op;
    shared_ptr<ASTNode> left;
    shared_ptr<ASTNode> right;

    BinaryOpNode(const Token& op,
        shared_ptr<ASTNode> left,
        shared_ptr<ASTNode> right)
        : op(op), left(left), right(right) {
    }

    BinaryOpNode(string name,
        const Token& op,
        shared_ptr<ASTNode> left,
        shared_ptr<ASTNode> right)
        : name(name), op(op), left(left), right(right) {
    }

    void printASTNode(int depth = 0) override {
        printTabsDepth(depth);
        cout << name << ": " << op.lex << endl;

        printTabsDepth(depth + 1);
        cout << "Left:" << endl;
        if (left != nullptr)
            left->printASTNode(depth + 2);

        printTabsDepth(depth + 1);
        cout << "Right:" << endl;
        if (right != nullptr)
            right->printASTNode(depth + 2);
    }

   

    
};

// צומת עבור ביטוי אונרי, למשל: -x
struct UnaryOpNode : ASTNode {
public:
    string name = "UnaryOpNode";
    Token op;
    shared_ptr<ASTNode> expr;

    UnaryOpNode(const Token& op,
        shared_ptr<ASTNode> expr)
        : op(op), expr(expr) {
    }

    void printASTNode(int depth = 0) override {
        printTabsDepth(depth);
        cout << name << ": " << op.lex << endl;

        if (expr != nullptr)
            expr->printASTNode(depth + 1);
    }

   
    

   
};

// צומת פשוט לטקסט כללי / הודעות / תוכן מיוחד
struct SentenceNode : ASTNode {
public:
    string content;

    SentenceNode(string content)
        : content(content) {
    }

    void printASTNode(int depth = 0) override {
        printTabsDepth(depth);
        cout << "SentenceNode: " << content << endl;
    }
};
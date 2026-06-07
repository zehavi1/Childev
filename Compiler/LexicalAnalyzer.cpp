#include "LexicalAnalyzer.h"
#include <iostream>
#include <cctype>

void LexicalAnalyzer::addLexema(Token*& head, Token*& tail, TokenType type, const std::string& lex)
{
    Token* node = new Token();

    node->typeToken = type;
    node->lex = lex;
    node->nextlex = nullptr;

    if (head == nullptr)
    {
        head = node;
        tail = node;
    }
    else
    {
        tail->nextlex = node;
        tail = node;
    }
}

std::string LexicalAnalyzer::toLowerStr(const std::string& str)
{
    std::string result = str;

    for (char& c : result)
    {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }

    return result;
}

TokenType LexicalAnalyzer::getKeywordOrIdentifier(const std::string& word)
{
    std::string lowerWord = toLowerStr(word);

    if (lowerWord == "else")
        return Tok_else;

    if (lowerWord == "false")
        return Tok_false;

    if (lowerWord == "for")
        return Tok_for;

    if (lowerWord == "if")
        return Tok_if;

    if (lowerWord == "read")
        return Tok_read;

    if (lowerWord == "to")
        return Tok_to;

    if (lowerWord == "true")
        return Tok_true;

    if (lowerWord == "write")
        return Tok_write;

    if (lowerWord == "while")
        return Tok_while;

    return Tok_identifier;
}

bool LexicalAnalyzer::isMathOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

Token* LexicalAnalyzer::getLexemaList(const std::string& code)
{
    Token* head = nullptr;
    Token* tail = nullptr;

    int i = 0;
    int length = static_cast<int>(code.length());

    while (i < length)
    {
        char c = code[i];

        // רווחים וטאבים - מתעלמים
        if (c == ' ' || c == '\t' || c == '\r')
        {
            i++;
            continue;
        }

        // ירידת שורה
        if (c == '\n')
        {
            addLexema(head, tail, Tok_newline, "\\n");
            i++;
            continue;
        }

        // מילה שמורה או identifier
        // לפי חוקי השפה: identifier מורכב מאותיות בלבד
        if (isalpha(static_cast<unsigned char>(c)))
        {
            std::string word = "";

            while (i < length && isalpha(static_cast<unsigned char>(code[i])))
            {
                word += code[i];
                i++;
            }

            std::string lowerWord = toLowerStr(word);
            TokenType type = getKeywordOrIdentifier(lowerWord);

            // שומרים ב-lowercase כדי שלא יהיה הבדל בין X לבין x
            addLexema(head, tail, type, lowerWord);
            continue;
        }

        // מספר: 123 או 12.5
        if (isdigit(static_cast<unsigned char>(c)))
        {
            std::string number = "";

            while (i < length && isdigit(static_cast<unsigned char>(code[i])))
            {
                number += code[i];
                i++;
            }

            if (i < length && code[i] == '.')
            {
                number += code[i];
                i++;

                bool hasDigitAfterDot = false;

                while (i < length && isdigit(static_cast<unsigned char>(code[i])))
                {
                    hasDigitAfterDot = true;
                    number += code[i];
                    i++;
                }

                if (!hasDigitAfterDot)
                {
                    addLexema(head, tail, Tok_error, number);
                    continue;
                }
            }

            addLexema(head, tail, Tok_num, number);
            continue;
        }

        // מחרוזת: "..."
        if (c == '"')
        {
            std::string str = "";
            str += code[i];
            i++;

            while (i < length && code[i] != '"')
            {
                str += code[i];
                i++;
            }

            if (i < length && code[i] == '"')
            {
                str += code[i];
                i++;
                addLexema(head, tail, Tok_string, str);
            }
            else
            {
                addLexema(head, tail, Tok_error, str);
            }

            continue;
        }

        // תו: 'a' או ' '
        if (c == '\'')
        {
            std::string ch = "";
            ch += code[i];
            i++;

            while (i < length && code[i] != '\'')
            {
                ch += code[i];
                i++;
            }

            if (i < length && code[i] == '\'')
            {
                ch += code[i];
                i++;

                // בדיקה בסיסית: חייב להיות לפחות פתיחה וסגירה
                // בדיקה מדויקת של אורך התו אפשר לעשות בהמשך בניתוח סמנטי
                addLexema(head, tail, Tok_char, ch);
            }
            else
            {
                addLexema(head, tail, Tok_error, ch);
            }

            continue;
        }

        // הערה: { ... }
        if (c == '{')
        {
            std::string comment = "";
            comment += code[i];
            i++;

            while (i < length && code[i] != '}')
            {
                comment += code[i];
                i++;
            }

            if (i < length && code[i] == '}')
            {
                comment += code[i];
                i++;
                addLexema(head, tail, Tok_comment, comment);
            }
            else
            {
                addLexema(head, tail, Tok_error, comment);
            }

            continue;
        }

        // השמה: <-
        if (c == '<' && i + 1 < length && code[i + 1] == '-')
        {
            addLexema(head, tail, Tok_assign, "<-");
            i += 2;
            continue;
        }

        // סוף בלוק: ||
        if (c == '|' && i + 1 < length && code[i + 1] == '|')
        {
            addLexema(head, tail, Tok_block_end, "||");
            i += 2;
            continue;
        }

        // OR: |
        if (c == '|')
        {
            addLexema(head, tail, Tok_or, "|");
            i++;
            continue;
        }

        // AND: &
        if (c == '&')
        {
            addLexema(head, tail, Tok_and, "&");
            i++;
            continue;
        }

        // !=
        if (c == '!' && i + 1 < length && code[i + 1] == '=')
        {
            addLexema(head, tail, Tok_comp, "!=");
            i += 2;
            continue;
        }

        // ! לבד לא חוקי בשפה שלך
        if (c == '!')
        {
            addLexema(head, tail, Tok_error, "!");
            i++;
            continue;
        }

        // סימני השוואה כפולים: >=, =>, <=, =<
        if (i + 1 < length)
        {
            std::string two = "";
            two += code[i];
            two += code[i + 1];

            if (two == ">=" || two == "=>" || two == "<=" || two == "=<")
            {
                addLexema(head, tail, Tok_comp, two);
                i += 2;
                continue;
            }
        }

        // סימני השוואה בודדים: =, <, >
        if (c == '=' || c == '<' || c == '>')
        {
            std::string op = "";
            op += c;

            addLexema(head, tail, Tok_comp, op);
            i++;
            continue;
        }

        // אופרטורים חשבוניים: + - * / ^
        if (isMathOperator(c))
        {
            std::string op = "";
            op += c;

            addLexema(head, tail, Tok_math, op);
            i++;
            continue;
        }

        // סוגר פותח
        if (c == '(')
        {
            addLexema(head, tail, Tok_Left_paren, "(");
            i++;
            continue;
        }

        // סוגר סוגר
        if (c == ')')
        {
            addLexema(head, tail, Tok_Right_paren, ")");
            i++;
            continue;
        }

        // פסיק
        if (c == ',')
        {
            addLexema(head, tail, Tok_comma, ",");
            i++;
            continue;
        }

        // נקודתיים
        if (c == ':')
        {
            addLexema(head, tail, Tok_Colon, ":");
            i++;
            continue;
        }

        // נקודה פסיק - לא חובה, אבל אם כתבו נתמוך בזה
        if (c == ';')
        {
            addLexema(head, tail, Tok_semicolon, ";");
            i++;
            continue;
        }

        // כל תו אחר הוא שגיאה
        std::string unknown = "";
        unknown += c;

        addLexema(head, tail, Tok_error, unknown);
        i++;
    }

    return head;
}
const char* tokenToString(TokenType token)
{
    if (token < Tok_identifier || token > Tok_error)
        return "Unknown Token";

    return TokenNames[token];
}
void LexicalAnalyzer::printLexemaList(Token* head)
{
    Token* curr = head;

    while (curr != nullptr)
    {
        std::cout << "TokenType: " << tokenToString(curr->typeToken)
            << " | Token: " << curr->lex << std::endl;

        curr = curr->nextlex;
    }
}

void LexicalAnalyzer::freeLexemaList(Token* head)
{
    while (head != nullptr)
    {
        Token* temp = head;
        head = head->nextlex;
        delete temp;
    }
}

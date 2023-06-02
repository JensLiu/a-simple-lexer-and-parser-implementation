//
// Created by jens on 30/05/23.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include <unordered_map>
#include "Token.h"
#include "InputBuffer.h"
#include "SymbolTable.h"

class Lexer {
public:
    static std::unordered_map<std::string, Token::TokenType> KEYWORDS;
    static bool isDigit(char c);
    static bool isLetter(char c);
    static bool isWhitespace(char c);
    static bool isOperator(char c);
    static bool isDelimiter(char c);
    static const int TOKEN_BUFFER_SIZE = 1024;

private:
    InputBuffer *inputBuffer;
    char tokenBuffer[TOKEN_BUFFER_SIZE]{};
    SymbolTable *symbolTable;

    int forwardIdx = 0;
    char currentChar();
    char peek();
    void forward();
    void forwardIgnore();
    std::string commitLexeme();

    Token handleIdentifier();
    Token handleNumber();
    void handleOptionalFractionSubroutine();
    void handleOptionalExponentSubroutine();
    Token handleOperator();
    Token handleWhitespace();
    Token handleCharLiteral();
    Token handleStringLiteral();
    void handleEscapeSubroutine();
    Token handleDelimiter();
    void handleSingleLineCommentSubroutine();
    void handleMultiLineCommentSubroutine();

public:
    Lexer(InputBuffer *inputBuffer, SymbolTable *symbolTable);
    Token nextToken();

};

class LexicalError : public std::runtime_error {
private:
    int line;
    int column;
    std::string message;
public:
    LexicalError(const std::string& error, int line, int column)
        : std::runtime_error(error), line(line + 1), column(column + 1) {
        this->message = error + " at line " + std::to_string(line + 1)  + " at column " + std::to_string(column + 1);
    }
    [[nodiscard]] const char * what () const noexcept override {
        return message.c_str();
    }
    [[nodiscard]] int getLine() const { return this->line; }
    [[nodiscard]] int getColumn() const { return this->column; }
};


#endif //COMPILER_LEXER_H

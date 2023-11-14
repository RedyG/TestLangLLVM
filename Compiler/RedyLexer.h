#pragma once
#include "Lexer.h"

enum class TokenType {
    Add,
    Sub,
    Mul,
    Div,
    Equal,
    Dot,
    Coma,
    LParen,
    RParen,
    LCurly,
    RCurly,

    Pub,
    Class,

    Identifier,
    FloatLiteral
};

using RedyLexer = Lexer<TokenType>;

RedyLexer CreateRedyLexer(std::string_view input);
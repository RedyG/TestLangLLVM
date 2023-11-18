#pragma once
#include "Lexer.h"
#undef EOF;

enum class TokenType {
    Invalid,

    Add,
    Sub,
    Mul,
    Div,
    Equal,
    Dot,
    Comma,
    LParen,
    RParen,
    LCurly,
    RCurly,

    Pub,
    Class,

    Identifier,
    Double
};

using RedyLexer = Lexer<TokenType>;

RedyLexer CreateRedyLexer(std::string_view input);
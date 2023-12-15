#pragma once
#include "Lexer.h"
#undef EOF;

enum class TokenType {
    Invalid,

    Add,
    Sub,
    Mul,
    Div,

    Arrow,
    Equal,
    Dot,
    SemiColon,
    Comma,

    LParen,
    RParen,
    LCurly,
    RCurly,

    Pub,
    Struct,
    Return,

    Identifier,
    Double
};

using RedyLexer = Lexer<TokenType>;

RedyLexer CreateRedyLexer(std::string_view input);

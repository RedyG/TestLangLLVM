#pragma once
#include "Lexer.h"

enum class TokenType {
    Invalid,


    Or,
    And,
    Eq,

    Add,
    Sub,
    Mul,
    Div,

    Arrow,
    Equal,
    Dot,
    SemiColon,
    Colon,
    Comma,

    LParen,
    RParen,
    LCurly,
    RCurly,

    Trait,
    Pub,
    Struct,
    Return,
    If,
    Else,
    True,
    False,
    Use,
    Mod,

    Identifier,
    Float,
    Int,
};

using RedyLexer = Lexer<TokenType>;

RedyLexer CreateRedyLexer(std::string_view input);

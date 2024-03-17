#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include "TextPos.h"
#include "NodeAST.h"

template<class T> struct Token
{
public:
    T Type;
    std::string_view Content;
    TextPos Start;
    TextPos End;

    Token(T type, std::string_view content, TextPos start, TextPos end) : Type(type), Content(content), Start(start), End(end) {}
};

struct MatchResult
{
public:
    bool Matched;
    std::string_view Content;
    int Length;

    // If the LineOffsetY is 0, then the length is used to increment the current LineX in the Lexer and this field is ignored, otherwise, this is the new LineX
    int LineX;
    int LineOffsetY;

    MatchResult() : MatchResult(false, 0) {}

    MatchResult(bool matched, int length) : MatchResult(matched, length, "", 0, 0) {}

    MatchResult(bool matched, int length, std::string_view content) : MatchResult(matched, length, content, 0, 0) {}

    MatchResult(bool matched, int length, std::string_view content, int lineX, int lineOffsetY)
        : Matched(matched), Length(length), Content(content), LineX(lineX), LineOffsetY(lineOffsetY) {}
};

template<class T> struct TokenMatcher
{
public:
    T Type;

    //TokenMatcher& operator=(TokenMatcher&&) noexcept = default;
    //TokenMatcher(TokenMatcher&&) noexcept = default;

    TokenMatcher(T type, std::function<MatchResult(std::string_view)> matcher)
    {
        Type = type;
        m_matcher = matcher;
    }

    inline MatchResult Match(std::string_view input) const
    {
        return m_matcher(input);
    }
private:
    std::function<MatchResult(std::string_view)> m_matcher;
};


template<class T> class Lexer
{
public:
    std::vector<TokenMatcher<T>>* TokenMatchers;

    Lexer(std::string_view input, std::vector<TokenMatcher<T>>* tokenMatchers, T invalid) :
        m_token(invalid, std::string_view(), TextPos(), TextPos()), TokenMatchers(tokenMatchers), m_input(input), m_invalid(invalid) {}


    // returns the current token.
    inline Token<T> Current() const {
        return m_token;
    }

    NodeAST Node() {
        return NodeAST(m_token.Start, m_token.End);
    }

    Token<T> Consume()
    {
        if (m_pos.Pos >= m_input.length())
            return m_token = Token(m_invalid, "", TextPos(), TextPos());

        while (std::isspace(m_input.at(m_pos.Pos))) {
            if (m_pos.Pos == m_input.length() - 1) {
                m_pos.Pos++;
                return m_token = Token(m_invalid, "", TextPos(), TextPos());
            }

            if (m_input.at(m_pos.Pos) == '\n') {
                m_pos.LineY++;
                m_pos.LineX = 0;
            }

            m_pos.Pos++;
        }
        
        auto trimmedInput = m_input.substr(m_pos.Pos);

        for (const TokenMatcher<T>& tokenMatcher : *TokenMatchers) {
            MatchResult match = tokenMatcher.Match(trimmedInput);

            if (match.Matched) {
                auto start = m_pos;
                m_pos.Pos += match.Length;
                m_pos.LineY += match.LineOffsetY;
                if (match.LineOffsetY == 0)
                    m_pos.LineX += match.Length;
                else
                    m_pos.LineX = match.LineX;

                m_token = Token(tokenMatcher.Type, match.Content, start, m_pos);
                return m_token;
            }
        }


        throw std::exception("couldn't parse token");
    }

    // If the current token matches the provided type, it will consume it and return true.
    bool ConsumeIf(T type) {
        if (m_token.Type == type) {
            Consume();
            return true;
        }

        return false;
    }
private:
    std::string_view m_input;
    TextPos m_pos;
    Token<T> m_token;
    T m_invalid;
};
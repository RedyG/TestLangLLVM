#pragma once
#include <iostream>
#include <vector>
#include <functional>

struct TextPos {
    int LineX;
    int LineY;
    int Pos;

    TextPos(int lineX, int lineY, int pos) : LineX(lineX), LineY(lineY), Pos(pos) {}
    TextPos() : LineX(0), LineY(0), Pos(0) {}
};

template<class T> struct Token
{
public:
    T Type;
    std::string_view Content;
    TextPos Start;
    TextPos End;

    Token(T type, std::string_view content)
    {
        Type = type;
        Content = content;
    }
};

struct MatchResult
{
public:
    bool Matched;
    std::string_view Content;
    TextPos Start;
    TextPos End;

    // No match
    MatchResult() : MatchResult(false, TextPos(), 0) {}

    // Single line match without content
    MatchResult(bool matched, TextPos start, int length) : MatchResult(matched, start, length, "") {}

    // Single line match
    MatchResult(bool matched, TextPos start, int length, std::string_view content) : Matched(matched), Start(start), Content(content) {
        End = start;
        End.Pos += length;
    }

    // Multi line match
    MatchResult(bool matched, TextPos start, TextPos end, std::string_view content) : Matched(matched), Start(start), End(end), Content(content) {}
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
        m_token(invalid, std::string_view()), TokenMatchers(tokenMatchers), m_input(input), m_invalid(invalid) {}


    // returns the current token.
    inline Token<T> Current() const
    {
        return m_token;
    }

    Token<T> Consume()
    {
        if (m_input.length() == 0)
            return m_token = Token(m_invalid, "");

        int i = 0;
        while (std::isspace(m_input.at(i)))
        {
            if (i == m_input.length() - 1)
                return m_token = Token(m_invalid, "");
            i++;
        }

        if (i > 0)
            m_input = m_input.substr(i);

        for (const TokenMatcher<T>& tokenMatcher : *TokenMatchers)
        {
            MatchResult match = tokenMatcher.Match(m_input);
            if (match.Matched)
            {
                m_token = Token(tokenMatcher.Type, match.Content);
                m_input = m_input.substr(match.Length);
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
    Token<T> m_token;
    T m_invalid;
};
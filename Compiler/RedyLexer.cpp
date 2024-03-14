#include "RedyLexer.h"
#include <string>
#include <cwctype>

MatchResult MatchKeyword(std::string_view input, std::string_view keyword) {
	if (input.starts_with(keyword) && std::isspace(input[keyword.length()]))
		return MatchResult(true, keyword.length() + 1);

	return MatchResult();
}

MatchResult MatchChar(std::string_view input, char c) {
	if (input.at(0) == c)
		return MatchResult(true, 1);

	return MatchResult();
}

MatchResult MatchString(std::string_view input, std::string_view string) {
	if (input.starts_with(string))
		return MatchResult(true, 2);

	return MatchResult();
}

MatchResult MatchIdentifier(std::string_view input) {
	if (!std::isalpha(input[0]) && input[0] != '_')
		return MatchResult();

	int i = 1;
	while (std::iswalnum(input[i]) || input[i] == '_') { i++; }

	return MatchResult(true, i, input.substr(0, i));
}


MatchResult MatchFloat(std::string_view input) {
	// 1515_15.51_5
	bool inDecimal = false;
	bool moreThanOneDecimal = false;
	for (int i = 0; i < input.length(); i++) {
		char c = input[i];
		if (c == '_')
			continue;

		if (std::isdigit(c)) {
			if (inDecimal)
				moreThanOneDecimal = true;
		}
		else {
			if (inDecimal && moreThanOneDecimal) 
				return MatchResult(true, i, input.substr(0, i));
			else if (c == '.')
				inDecimal = true;
			else
				return MatchResult();
		}
	}

	return MatchResult();
}

MatchResult MatchInt(std::string_view input) {
	// 1515_15
	int i = 0;
	while (std::isdigit(input[i]) || input[i] == '_')
		i++;

	return MatchResult(i > 0, i, input.substr(0, i));
}

std::vector<TokenMatcher<TokenType>> tokenMatchers {
	TokenMatcher<TokenType>(TokenType::Or, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "or"); }),
	TokenMatcher<TokenType>(TokenType::And, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "and"); }),
	TokenMatcher<TokenType>(TokenType::Eq, [](std::string_view input) -> MatchResult { return MatchString(input, "=="); }),
	TokenMatcher<TokenType>(TokenType::Add, [](std::string_view input) -> MatchResult { return MatchChar(input, '+'); }),
	TokenMatcher<TokenType>(TokenType::Sub, [](std::string_view input) -> MatchResult { return MatchChar(input, '-'); }),
	TokenMatcher<TokenType>(TokenType::Mul, [](std::string_view input) -> MatchResult { return MatchChar(input, '*'); }),
	TokenMatcher<TokenType>(TokenType::Div, [](std::string_view input) -> MatchResult { return MatchChar(input, '/'); }),
	TokenMatcher<TokenType>(TokenType::Arrow, [](std::string_view input) -> MatchResult { return MatchString(input, "=>"); }),
	TokenMatcher<TokenType>(TokenType::Equal, [](std::string_view input) -> MatchResult { return MatchChar(input, '='); }),
	TokenMatcher<TokenType>(TokenType::Dot, [](std::string_view input) -> MatchResult { return MatchChar(input, '.'); }),
	TokenMatcher<TokenType>(TokenType::SemiColon, [](std::string_view input) -> MatchResult { return MatchChar(input, ';'); }),
	TokenMatcher<TokenType>(TokenType::Colon, [](std::string_view input) -> MatchResult { return MatchChar(input, ':'); }),
	TokenMatcher<TokenType>(TokenType::Comma, [](std::string_view input) -> MatchResult { return MatchChar(input, ','); }),
	TokenMatcher<TokenType>(TokenType::LParen, [](std::string_view input) -> MatchResult { return MatchChar(input, '('); }),
	TokenMatcher<TokenType>(TokenType::RParen, [](std::string_view input) -> MatchResult { return MatchChar(input, ')'); }),
	TokenMatcher<TokenType>(TokenType::LCurly, [](std::string_view input) -> MatchResult { return MatchChar(input, '{'); }),
	TokenMatcher<TokenType>(TokenType::RCurly, [](std::string_view input) -> MatchResult { return MatchChar(input, '}'); }),
	TokenMatcher<TokenType>(TokenType::Trait, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "trait"); }),
	TokenMatcher<TokenType>(TokenType::Pub, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "pub"); }),
	TokenMatcher<TokenType>(TokenType::Struct, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "struct"); }),
	TokenMatcher<TokenType>(TokenType::Return, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "return"); }),
	TokenMatcher<TokenType>(TokenType::If, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "if"); }),
	TokenMatcher<TokenType>(TokenType::Else, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "else"); }),
	TokenMatcher<TokenType>(TokenType::True, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "true"); }),
	TokenMatcher<TokenType>(TokenType::False, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "false"); }),
	TokenMatcher<TokenType>(TokenType::Use, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "use"); }),
	TokenMatcher<TokenType>(TokenType::Mod, [](std::string_view input) -> MatchResult { return MatchKeyword(input, "mod"); }),
	TokenMatcher<TokenType>(TokenType::Identifier, MatchIdentifier),
	TokenMatcher<TokenType>(TokenType::Float, MatchFloat),
	TokenMatcher<TokenType>(TokenType::Int, MatchInt),

};

RedyLexer CreateRedyLexer(std::string_view input)
{
	return RedyLexer(input, &tokenMatchers, TokenType::Invalid);
}
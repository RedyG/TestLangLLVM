#pragma once
#include <string>

class TypeAST {
public:
	std::string_view Name;

	TypeAST(std::string_view name) : Name(name) {}
};

struct TypeASTComparer
{
	bool operator() (const TypeAST& lhs, const TypeAST& rhs) const
	{
		return lhs.Name < rhs.Name;
	}
};
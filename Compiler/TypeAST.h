#pragma once
#include <string>


class TypeAST {
public:
	std::string_view Name;

	TypeAST(std::string_view name) : Name(name) {}


	bool operator == (const TypeAST& other) const {
		return Name == other.Name;
	}
};

template <>
struct std::hash<TypeAST> {
    std::size_t operator()(const TypeAST& k) const {
		return hash<string_view>()(k.Name);
    }
};
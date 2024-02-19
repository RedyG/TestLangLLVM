#pragma once
#include <vector>
#include <string_view>

class UseDeclAST {
public:
	std::vector<std::string_view> Path;
	std::vector<std::string_view> Imports;
};
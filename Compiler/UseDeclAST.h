#pragma once
#include <vector>
#include <string_view>
#include "NodeAST.h"

class UseDeclAST{
public:
	NodeAST Node;
	std::vector<std::string_view> Path;
	std::vector<std::string_view> Imports;

	UseDeclAST(std::vector<std::string_view> path, std::vector<std::string_view> imports, NodeAST node)
		: Node(node), Path(std::move(path)), Imports(std::move(imports)) {}
};
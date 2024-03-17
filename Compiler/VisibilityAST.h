#pragma once
#include "NodeAST.h"

enum class VisibilityAST {
	Public,
	Private
};

class VisibilityNodeAST {
public:
	VisibilityAST Value;
	NodeAST Node;

	VisibilityNodeAST(VisibilityAST value, NodeAST node) : Node(node), Value(value) {}
};
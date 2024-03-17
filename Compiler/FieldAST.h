#pragma once
#include "VisibilityAST.h"
#include "VariableAST.h"

// TODO: add other visiblites like pub(get) and pub(set)
class FieldAST{
public:
	NodeAST Node;
	VisibilityNodeAST Visibility;
	VariableAST Variable;

	FieldAST(VisibilityNodeAST visibility, VariableAST variable, NodeAST node)
		: Visibility(visibility), Variable(std::move(variable)), Node(node) {}
};
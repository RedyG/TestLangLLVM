#pragma once
#include "VisibilityAST.h"
#include "VariableAST.h"

// TODO: add other visiblites like pub(get) and pub(set)
class FieldAST {
public:
	VisibilityAST Visibility;
	VariableAST Variable;

	FieldAST(VisibilityAST visibility, VariableAST variable)
		: Visibility(visibility), Variable(std::move(variable)) {}
};
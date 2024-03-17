#pragma once
#include "TextPos.h"

class NodeAST {
public:
	TextPos Start;
	TextPos End;

	NodeAST(TextPos start, TextPos end) : Start(start), End(end) {}
	NodeAST() : Start(TextPos()), End(TextPos()) {}
};
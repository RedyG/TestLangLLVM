#include "StatementAST.h"

bool StatementAST::IsBranch() {
	return false;
}

StatementAST::StatementAST(NodeAST node) : Node(node) {}
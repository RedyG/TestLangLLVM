#include "StructAST.h"
#include "TypeDeclAST.h"
TypeDeclAST* UnknownType = new StructAST({}, VisibilityAST::Public, "Unknown", {});
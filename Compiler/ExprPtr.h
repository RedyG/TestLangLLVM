#pragma once
#include <memory>
class ExprAST;

using ExprPtr = std::unique_ptr<ExprAST>;
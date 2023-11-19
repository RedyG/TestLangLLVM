#pragma once

#pragma warning(disable:4146)
#include "llvm/IR/IRBuilder.h"
#include <map>
using namespace llvm;

extern LLVMContext TheContext;
extern IRBuilder<> Builder;
extern Module* TheModule;
extern std::map<std::string_view, Value*> NamedValues;
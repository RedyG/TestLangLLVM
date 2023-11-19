#include "CodeGenerator.h"

LLVMContext TheContext;
IRBuilder<> Builder = IRBuilder<>(TheContext);
Module* TheModule;
std::map<std::string_view, Value*> NamedValues;
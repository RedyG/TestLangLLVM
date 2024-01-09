#include <string>
#include <vector>
enum class VisibilityAST;
class FuncAST;

class TypeDeclAST {
public:
	VisibilityAST Visibility;
	std::string_view Name;
	std::vector<FuncAST> Methods;

	TypeDeclAST(VisibilityAST visibility, std::string_view name, std::vector<FuncAST> methods)
		: Visibility(visibility), Name(name), Methods(std::move(methods)) {}
};
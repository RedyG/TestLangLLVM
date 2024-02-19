#include "Project.h"
#include "Logger.h"
#include <format>

RedyModule* Project::GetModule(std::string_view name) {
	auto mod = m_modules.find(name);
	if (mod == m_modules.end()) {
		Logger::Error(std::format("The modules {} does not exist.", name));
		return nullptr;
	}
	return &mod->second;
}

void Project::AddModule(RedyModule module) {
	m_modules.emplace(module.Name, std::move(module));
}
#ifndef __ARGUMENT_PARSER_H__
#define __ARGUMENT_PARSER_H__
#include <string>
#include <vector>
#include <map>
#include <algorithm>

class ArgumentParser
{
public:
	ArgumentParser(int argc, char* argv[]);
	inline const std::string& GetArgumentValue(const std::string& name) {
		return m_mContainer[name];
	}
	inline bool IsOptionArgument(const std::string& name) {
		return m_mContainer[name].find("OPTION") != std::string::npos;
	}

private:
	std::map<std::string, std::string> m_mContainer;
};

#endif
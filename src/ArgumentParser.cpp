#include "ArgumentParser.h"


ArgumentParser::ArgumentParser(int argc, char* argv[])
{
	std::string temp = "";
	std::string temp2 = "";

	// Parser all arguments.
	for(int i = 1; i < argc; i++) {
		temp = argv[i];

		// argument name.
		if(temp.find("-") != std::string::npos) {
			// try find value, check next arg.
			if(i+1 < argc) {
				temp2 = argv[i+1];
				// if it is no argument name, add (temp, value).
				if(temp2.find("-") == std::string::npos) {
					// convert to lower.
					std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
					m_mContainer.insert(std::make_pair(temp, temp2));
					
				} else { // options:
					// convert to lower.
					std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
					m_mContainer.insert(std::pair<std::string, std::string>(temp, "OPTION"));
				}
				i++; // skip next element.
			} else { // no value ( options? )
				// convert to lower.
				std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

				m_mContainer.insert(std::pair<std::string, std::string>(temp, "OPTION"));
			}
		}
	}
}
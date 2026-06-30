#ifndef FUNC_HPP
#define FUNC_HPP

#include <sys/stat.h>
#include <vector>
#include <map>
#include <string>

#include "dt_config.hpp"

namespace func {

/**
 * 
 *
 * @param 
 * @return
 */
void Version(std::ostream& stream, const std::string & cmd) {
    stream << cmd << " Version " << dt_VERSION_MAJOR << "." << dt_VERSION_MINOR
    			<< "." << dt_VERSION_PATCH << std::endl;
}

/**
 * 
 *
 * @param 
 * @return
 */
inline bool FileExists(const std::string& filename) {
	struct stat buffer;   
	return (stat (filename.c_str(), &buffer) == 0); 
}

/**
 * 
 *
 * @param 
 * @return
 */
inline std::string SplitFilename (const std::string& str) {
	std::string result;
	size_t found;
  	
  	found = str.find_last_of("/\\");
  	result = str.substr(found + 1);

  	return result;
}
/**
 * This checks individual arguments for validity and returns:
 *				0 if not valid
 *				1 if valid and has no arguments
 *				1 + n if valid and has n arguments
 *
 * @param arg The argument to check
 * @return
 */
int ArgRule(const std::string& arg) {
	int result(0);

	if(arg == "--load") {
		result = 2;
	}else if(arg == "--log") {
		result = 2;
	}else if(arg == "--save") {
		result = 2;
	}else if(arg == "--testing") {
		result = 2;
	}else if(arg == "--training") {
		result = 2;
	}

	return result;
}

/**
 * A helper method to print when invalid arguments are passed.
 *
 * @param stream Stream to print to
 * @param cmd Name of the command being used
 */
 void Invalid(std::ostream& stream, const std::string & cmd) {
	Version(stream, cmd);
	stream << "usage: " << cmd << " [--log level] [--save file]"
				<< " [--testing file] [--training file]" << std::endl;
	stream << "usage: " << cmd << " [--load file] [--log level]"
				<< " [--testing file]" << std::endl;
 }

/**
 * A helper method to check and verify syntax of the arguments as well as the
 * correct data to go with them.
 *
 * @param argc The count of the arguments in argv
 * @param argv The argument data stored in a 2d array
 * @param argus The return string for the arguments
 * @return bool Whether or not the arguments passed in are valid
 */
bool Args(int argc, char * argv[], std::map<std::string, std::vector<std::string>>& argus) {
	bool passed(true);
	std::string cmd(SplitFilename(argv[0]));

	//Checks if arguments were passed.
	if(argc <= 1) {
		passed = false;
		Invalid(std::cerr, cmd);
		return passed;
	}

	//Collects all arguments
	std::map<std::string, std::vector<std::string>> args;
	for(int z = 1; z < argc; z++) {
		int res = ArgRule(argv[z]);
		if(!res) {
			passed = false;
			Invalid(std::cerr, cmd);
			std::cerr << "[ERROR]: invalid flag: " << argv[z] << std::endl;
			return passed;
		}else {
			std::string key(argv[z]);
			std::vector<std::string> arguments;
			for(int y = z + 1; y <= z + (res - 1); y++) {
				if(y >= argc) {
					passed = false;
					Invalid(std::cerr, cmd);
					std::cerr << "[ERROR]: argument count: " << res - 1
								<< " for flag: " << argv[z]<< std::endl;
					return passed;
				}
				arguments.push_back(argv[y]);
			}
			args[key] = arguments;
			z += res - 1;
		}
	}

	//Validates the arguments
	for(auto const& map : args) {
		if(map.first == "--load") {
			if(!FileExists(map.second.front())) {
				std::cerr << map.first << " incorrect argument: "
							<< map.second.front() << std::endl;
				passed = false;
			}
		}else if(map.first == "--log") {
		}else if(map.first == "--save") {
		}else if(map.first == "--testing") {
			if(!FileExists(map.second.front())) {
				std::cerr << map.first << " incorrect argument: "
							<< map.second.front() << std::endl;
				passed = false;
			}
		}else if(map.first == "--training") {
			if(!FileExists(map.second.front())) {
				std::cerr << map.first << " incorrect argument: "
							<< map.second.front() << std::endl;
				passed = false;
			}
		}
	}

	argus = args;
	return passed;
}
}
#endif
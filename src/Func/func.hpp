#ifndef FUNC_HPP
#define FUNC_HPP

#include <sys/stat.h>
namespace nbdc {
inline bool FileExists(const std::string & filename) {
	struct stat buffer;   
	return (stat (filename.c_str(), &buffer) == 0); 
};
}

#endif
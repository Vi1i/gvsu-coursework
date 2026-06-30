#ifndef PARSE_HPP
#define PARSE_HPP

#include <sstream>
#include <climits>

namespace parse {
	namespace detail {
        int LINESTART = INT_MAX;
    }

	enum STR2INT_ERROR { SUCCESS, OVERFLOW, UNDERFLOW, INCONVERTIBLE };

	template<typename Out>
	void Split(const std::string &s, char delim, Out result) {
    	std::stringstream ss;
	    ss.str(s);
	    std::string item;
	    while (std::getline(ss, item, delim)) {
	        *(result++) = item;
	    }
	}

	std::vector<std::string> Split(const std::string &s, char delim) {
	    std::vector<std::string> elems;
	    parse::Split(s, delim, std::back_inserter(elems));
	    return elems;
	}

	/**
	 * 
	 *
	 * @param 
	 * @return
	 */
	parse::STR2INT_ERROR str2int (int &i, char const *s, int base) {
	    char *end;
	    long  l;
	    errno = 0;
	    l = strtol(s, &end, base);
	    if((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
	        return OVERFLOW;
	    }
	    if((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
	        return UNDERFLOW;
	    }
	    if(*s == '\0' || *end != '\0') {
	        return INCONVERTIBLE;
	    }
	    i = l;
	    return SUCCESS;
	}
	void Header(const std::string& filename,
					std::map<std::string, std::vector<std::string>>& header){
		std::ifstream ifs(filename);
		std::string line;
		char deliminator = ',';
		int line_count = 0;

		if(ifs.is_open()) {
			while(std::getline(ifs, line)) {
				line_count++;
				if(line_count == 2) {
					header["Oracle"] = parse::Split(line, deliminator);
				}else if(line_count == 3) {
					parse::str2int(parse::detail::LINESTART, line.c_str(), 10);
					parse::detail::LINESTART += 4;
				}else if(line_count > 3 && line_count < parse::detail::LINESTART) {
					 std::vector<std::string> temp = parse::Split(line, deliminator);
					 std::string name = temp.front();
					 header["ORDER"].push_back(name);
					 std::vector<std::string> newTemp;
					 int count = 0;
					 for(auto const& str : temp) {
					 	if(count > 1) {
					 		newTemp.push_back(str);
					 	}
					 	count++;
					 }
					 header[name] = newTemp;
				}else if(line_count >= parse::detail::LINESTART) {
					break;
				}
			}
			ifs.close();
		}
	}

	void Data(const std::string& filename, const std::vector<std::string>& order,
					std::vector<std::map<std::string, std::string>>& data){
		std::ifstream ifs(filename);
		std::string line;
		char deliminator = ',';
		int line_count = 0;

		if(ifs.is_open()) {
			while(std::getline(ifs, line)) {
				line_count++;
				if(line_count > parse::detail::LINESTART) {
					std::map<std::string, std::string> map;
					std::vector<std::string> temp = parse::Split(line, deliminator);
					for(auto z = 0; z < temp.size(); z++) {
						if(z >= order.size()) {
							map["Oracle"] = temp.at(z);
						}else{
							map[order.at(z)] = temp.at(z);
						}
					}
					data.push_back(map);
				}
			}
			ifs.close();
		}
	}
}

#endif

#include <iostream>
#include <fstream>
#include <string>

#include "Func/func.hpp"
#include "Parse/parse.hpp"
#include "DT/dt.hpp"

int main(int argc, char * argv[]) {
	std::map<std::string, std::vector<std::string>> args;
	if(!func::Args(argc, argv, args)) {
		exit(EXIT_FAILURE);
	}
	std::map<std::string, std::vector<std::string>> header;
	parse::Header(args["--training"].front(), header);
	
	std::vector<std::map<std::string, std::string>> data;
	parse::Data(args["--training"].front(),header["ORDER"], data);

	dt::DT dt(header, data);
	dt.CreateTree();

	exit(EXIT_SUCCESS);
}
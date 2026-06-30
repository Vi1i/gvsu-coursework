#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include <iomanip>

#include "dt.hpp"
	
dt::DT::DT(const std::map<std::string, std::vector<std::string>>& header, const std::vector<std::map<std::string, std::string>>& data) {
	this->header = header;
	this->data = data;
}

std::vector<std::string> dt::DT::Split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    dt::DT::Split(s, delim, std::back_inserter(elems));
    return elems;
}

void dt::DT::Entropy(const std::map<std::string, std::map<std::string, int>>& counts, std::map<std::string, double>& entropy) {
	//Calculates the entropy
    for(auto const& map : counts) {
        //std::cout << map.first << std::endl;
        double sum = 0;
        for(auto const& map_i : map.second) {
            if(map_i.first != "0_TOTAL") {
            	if(map_i.second != 0) {
    	        	int total = counts.at(map.first).at("0_TOTAL");
 	            	double prob = map_i.second / (double)total;
 	            	sum += (-1) *prob * (double)std::log2(prob);
            	}
            }
        }
        //sum = -1 * sum;
        entropy[map.first] = sum;
    }
}

void dt::DT::FixCounts(std::map<std::string, std::map<std::string, int>>& counts){
	//Adds in the missing data
    for(auto const& map : counts) {
        for(auto const& str : this->header["Oracle"]) {
            if(counts[map.first].find(str) == counts[map.first].end()) {
                counts[map.first][str] = 0;
            }
        }
    }
}

void dt::DT::CalcCounts(std::map<std::string, std::map<std::string, int>>& counts) {
	//Collects all the found points
    int pos = 0;
	for(auto const& map : this->data) {
		for(auto const& datum : map) {
            if(datum.first != "Oracle") {
                std::string key(datum.first + "+" + datum.second);
			    counts[key][this->data[pos]["Oracle"]]++;
			    counts[key]["0_TOTAL"]++;
            }
		}
		counts["SET"][this->data[pos]["Oracle"]]++;
		counts["SET"]["0_TOTAL"]++;

        pos++;
	}
}
void dt::DT::Gain(const std::map<std::string, std::map<std::string, int>>& counts, const std::map<std::string, double>& entropy, std::map<std::string, double>& gain) {
	char deliminator = '+';
	std::string prev = "";
	double summation;

	int count = 0;
	for(auto const& map : entropy) {
		if(map.first != "SET") {
			std::vector<std::string> split = dt::DT::Split(map.first, deliminator);
			if(prev != split[0]) {
				if(count == 0) {
					gain[split[0]] = entropy.at("SET") - summation;
					count++;
				}else {
					gain[prev] = entropy.at("SET") - summation;
				}
				prev = split[0];
				summation = 0;
			}
			//std::cout << "(" << counts.at(map.first).at("0_TOTAL") << "/" << counts.at("SET").at("0_TOTAL") << ") * " << map.second << std::endl;

			summation += ((double)counts.at(map.first).at("0_TOTAL") / (double)counts.at("SET").at("0_TOTAL")) * (double)map.second;
		}
	}
	gain[prev] = entropy.at("SET") - summation;
}

void dt::DT::CreateTree() {
	std::vector<std::string> ignore_list;
	std::map<std::string, std::map<std::string, int>> counts;
    dt::DT::CalcCounts(counts);
    dt::DT::FixCounts(counts);

	std::map<std::string, double> entropy;
	dt::DT::Entropy(counts, entropy);
    
	// int precision = std::numeric_limits<double>::max_digits10;
	// for(auto const& map : entropy) {
	// 	std::cout << std::setprecision(precision) << map.first << ": "<< map.second << std::endl;
	// }

	std::map<std::string, double> gain;
	dt::DT::Gain(counts, entropy, gain);

	std::pair<std::string, double> top("", 0);
	for(auto const& map : gain) {
		if(top.second < map.second) {
			top.first = map.first;
			top.second = map.second;
		}
	}
	ignore_list.push_back(top.first);

	//struct dt::Tree root;
	struct dt::Node root;
	root.attr = top.first;
	root.attr_val = top.second;
	root.leaf = false;

	std::map<std::string, std::vector<std::map<std::string, std::string>>> split_data;

	for(auto const& map : this->data) {
		split_data[map.at(top.first)].push_back(map);
	}

	//std::map<std::string, struct Node *> link;
	//std::map<std::string, double> link_vals;
	for(auto const& map : split_data) {
		std::string key(top.first + "+" + map.first);
		root.link_vals[map.first] = entropy[key];
		
		struct dt::Node node;
		std::string ignore(dt::DT::CreateTree(node, map.second, ignore_list));

		root.link[map.first] = node;
		if(ignore != "") {
			ignore_list.push_back(ignore);
		}
	}

	for(auto const& map : root.link) {
		std::cout << map.second.attr << std::endl;
	}

	dt::DT::PrintTree(root, 0);

	std::cout << std::endl;

	std::cout << "Node Count: " << gNode << std::endl
			  << "Leaf Count: " << gLeaf << std::endl;

}

void dt::DT::PrintTree(const struct dt::Node& root, int indent) {
		if(root.leaf) {
			gLeaf++;
		}else{
			gNode++;
		}
		for(auto const& map : root.link) {
			dt::DT::PrintTree(map.second, indent + 4);
		}
		if(indent) {
			std::cout << std::setw(indent) << ' ';
		}
		std::cout << root.attr << std::endl << " ";
}


std::string dt::DT::CreateTree(struct dt::Node& root,
				const std::vector<std::map<std::string, std::string>>& data,
				std::vector<std::string> ignore_list) {
	if(data.empty()) {
		root.attr = "Empty";
		root.leaf = true;
		return "";
	}
	std::string prev = data[0].at("Oracle");
	bool finished = true;

	for(auto const& map : data) {
		if(prev != map.at("Oracle")) {
			finished = false;
			break;
		}
	}
	if(finished) {
		root.attr = prev;
		root.leaf = true;
		return "";
	}

	std::map<std::string, std::map<std::string, int>> counts;
    dt::DT::CalcCounts(counts);
    dt::DT::FixCounts(counts);

	std::map<std::string, double> entropy;
	dt::DT::Entropy(counts, entropy);

	std::map<std::string, double> gain;
	dt::DT::Gain(counts, entropy, gain);

	std::pair<std::string, double> top("", std::numeric_limits<double>::lowest());
	for(auto const& map : gain) {
		bool in = false;
		for(auto const& str : ignore_list) {
			if(str == map.first) {
				in = true;
			}
		}
		if(in) {
			continue;
		}
		if(top.second <= map.second) {
			top.first = map.first;
			top.second = map.second;
		}
	}
	if(top.first == "") {
		root.attr = "Empty";
		root.leaf = true;
		return "";
	}
	ignore_list.push_back(top.first);

	root.attr = top.first;
	root.attr_val = top.second;
	root.leaf = false;

	std::map<std::string, std::vector<std::map<std::string, std::string>>> split_data;

	for(auto const& map : data) {
		split_data[map.at(top.first)].push_back(map);
	}

	//std::map<std::string, struct Node *> link;
	//std::map<std::string, double> link_vals;
	for(auto const& map : split_data) {
		std::string key(top.first + "+" + map.first);
		root.link_vals[map.first] = entropy[key];
		
		struct dt::Node node;
		dt::DT::CreateTree(node, map.second, ignore_list);
		root.link[map.first] = node;
	}
	return top.first;
}

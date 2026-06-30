#ifndef DT_HPP
#define DT_HPP

namespace dt {
struct Node {
	std::string attr;
	double attr_val;
	bool leaf;
	std::map<std::string, struct Node> link;
	std::map<std::string, double> link_vals;
};

struct Tree {
	struct Node * root;
};

class DT {
public:
	DT(const std::map<std::string, std::vector<std::string>>& header,
		const std::vector<std::map<std::string, std::string>>& data);
	void CreateTree();

private:
	int gLeaf;
	int gNode;
	std::map<std::string, std::vector<std::string>> header;
 	std::vector<std::map<std::string, std::string>> data;

    std::vector<std::string> Split(const std::string &s, char delim);
    void Entropy(const std::map<std::string, std::map<std::string, int>>& counts,
    				std::map<std::string, double>& entropy);
    void CalcCounts(std::map<std::string, std::map<std::string, int>>& counts);
    void FixCounts(std::map<std::string, std::map<std::string, int>>& counts);
    void Gain(const std::map<std::string, std::map<std::string, int>>& counts,
    				const std::map<std::string, double>& entropy,
    				std::map<std::string, double>& gain);
    void SplitData(const std::map<std::string, double>& gain);
	std::string CreateTree(struct dt::Node& root,
					const std::vector<std::map<std::string, std::string>>& data,
					std::vector<std::string> ignore_list);
	void PrintTree(const struct dt::Node& root, int indent);
	template<typename Out>
	void Split(const std::string &s, char delim, Out result) {
    	std::stringstream ss;
	    ss.str(s);
	    std::string item;
	    while (std::getline(ss, item, delim)) {
	        *(result++) = item;
	    }
	}
};
}

#endif

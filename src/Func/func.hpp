#ifndef FUNC_HPP
#define FUNC_HPP

#include <sys/stat.h>
namespace nbdc {
std::string total_cat("F1vs8KymEXNmmIod");

inline bool FileExists(const std::string & filename) {
	struct stat buffer;   
	return (stat (filename.c_str(), &buffer) == 0); 
};

template<typename T>
inline void PrintElement(T t, std::ostringstream& os, const int& width) {
	const char separator = ' ';
	os << std::left << std::setw(width) << std::setfill(separator) << t;
}

void ReadFile(const std::string& filename,
			  	std::map<std::string, std::vector<std::vector<std::string>>>& data,
			  	std::map<std::string, std::map<std::string, int>>& data_info) {
	//Opening the file
	std::ifstream in_file(filename);
	//Where the lines from the file will be stored
	std::string line;

	//While loop to get the line from the file
	while(std::getline(in_file, line)) {
		//A line from the file
		std::istringstream in_line(line);
		//Where the word in the line will be stored
		std::string token;
		//Temp vector variable
		std::vector<std::string> row;
		//Counter varaible for pos
		int count(0);
		//File seperator
		char sep = ' ';
		//Category string
		std::string cat("");

		//Loop to get each "word" out of the line
		while(std::getline(in_line, token, sep)) {
			//The first "word" is the category, that I can't use and is only for
			//training sets
			if(count == 0) {
				cat += token;
				count++;
				continue;
			}

			//Adds the token to the local stream
			row.push_back(token);
		}
		data_info[cat]["Words"] += row.size();
		//Add the string vector to the category
		data[cat].push_back(row);
	}
	int temp_docs(0);
	int temp_words(0);
	for(auto const& map : data) {
		temp_docs += map.second.size();
		data_info[map.first]["Docs"] = map.second.size();

		temp_words += map.second.size();
	}

	data_info[total_cat]["Docs"] = temp_docs;
	data_info[total_cat]["Words"] = temp_words;
};

void ProcessData(const std::map<std::string, std::vector<std::vector<std::string>>>& data,
				 const std::map<std::string, std::map<std::string, int>> & data_info,
				 std::map<std::string, std::map<std::string, double>>& processed_data,
				 std::map<std::string, std::map<std::string, double>>& processed_data_info,
				 std::map<std::string, std::map<std::string, int>>& cat_vocab,
				 std::map<std::string, int>& vocab) {

	//Gets the indivudal category vocab
	for(auto const& map : data) {
		std::map<std::string, int> c_v;
		for(auto const& vector : map.second) {
			for(auto const& word : vector) {
				// std::cout << word << std::endl;
				c_v[word]++;
			}
		}
		cat_vocab[map.first] = c_v;
	}

	//Gets the total counts of all words found
	for(auto const& map : cat_vocab) {
		for(auto const& word_counts : map.second) {
			vocab[word_counts.first] += word_counts.second;
		}
	}

	int total_u_words = vocab.size();

	//Calculates the probability of the word for each category
	for(auto const& word : vocab) {
		for(auto const& map : cat_vocab) {
			if(!(map.second.find(word.first) == map.second.end())) {
				double top = (double)map.second.at(word.first) + (double)1;
				double bottom = (double)data_info.at(map.first).at("Words") + total_u_words;
				processed_data[map.first][word.first] = top / bottom;
			}else{
				double top = 1;
				double bottom = (double)data_info.at(map.first).at("Words") + total_u_words;
				processed_data[map.first][word.first] = top / bottom;
			}
		}
	}

	int total_docs = data_info.at(nbdc::total_cat).at("Docs");
	for(auto const& map : data_info) {
		if(map.first == total_cat) {
			continue;
		}
		std::map<std::string, double> p_info;
		for(auto const& info : map.second) {
			if(info.first == "Words") {
				continue;
			}
			p_info[info.first] = (double)info.second / (double) total_docs;
		}
		processed_data_info[map.first] = p_info;
	}
};

void AnalyzeData(const std::map<std::string, std::map<std::string, double>>& processed_data,
				 const std::map<std::string, std::map<std::string, double>>& processed_data_info,
				 const std::map<std::string, std::vector<std::vector<std::string>>>& data,
				 const std::map<std::string, std::map<std::string, int>>& data_info,
				 std::map<std::string, std::map<std::string, int>>& confusion_matrix) {

	int total(data_info.at(nbdc::total_cat).at("Docs"));
	int correct(0);	
	
	for(auto const& map : data) {
		for(auto const& vector : map.second) {
			std::vector<std::pair<std::string, double>> prob_class;
			for(auto const& query: processed_data) {
				double line_prob(0.0);
				for(auto const& word : vector) {
					double temp(0.0);
					try {
						temp = query.second.at(word);
						line_prob += std::log(temp);
					}catch(const std::out_of_range) {

					}
				}
				double temp(processed_data_info.at(query.first).at("Docs"));
				line_prob += std::log(temp);
				std::pair<std::string, double> p(query.first, line_prob);
				prob_class.push_back(p);
			}
			auto highest_prob = std::max_element(begin(prob_class), end(prob_class),
            [](const std::pair<std::string, double>& lhs,
            	const std::pair<std::string, double>& rhs){
            return lhs.second < rhs.second;});

            //std::cout << "(" << (*highest_prob).first << ","<< map.first << ")" << std::endl;
            confusion_matrix[map.first][(*highest_prob).first]++;

            if((*highest_prob).first == map.first) {
            	correct++;
            }

		}
	}

	std::cout << "Total              : " << total << std::endl;
	std::cout << "Correct            : " << correct << std::endl;
	std::cout << "Accuracy           : " << (double)correct / (double)total * 100 
				<< "%" << std::endl;
};

void PrintConfusionMatrix(const std::map<std::string, std::map<std::string, int>>& confusion_matrix) {
	std::ostringstream rows;
	std::ostringstream head;
	int count(0);
	int width(17);
	head << "| ";
	nbdc::PrintElement<std::string>("", head, width);

	for(auto const& matrix : confusion_matrix) {
		width = 17;
		head << "| ";
		nbdc::PrintElement<std::string>(matrix.first, head, matrix.first.size() + 1);
		rows << "| ";
		//head << "| ";
		nbdc::PrintElement<std::string>(matrix.first, rows, width);
		for(auto const& el : matrix.second){
			width = el.first.size() + 1;
			rows << "| ";
			nbdc::PrintElement<int>(el.second, rows, width);
		}
		rows << "|" << std::endl;
	}
	head << "| ";
	std::cout << std::left<< std::setw(head.str().size() - 1) << std::setfill('-') << "" << std::endl;;
	std::cout << head.str() << std::endl;
	std::cout << std::left<< std::setw(head.str().size() -1 ) << std::setfill('-') << "" << std::endl;;
	std::cout << rows.str() << std::endl;
};
}

#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <chrono>
#include <complex>

#include <omp.h>

#include "nbdc_config.hpp"
#include "Func/func.hpp"


std::string total_category_word_count("lyj7MepVPo8m8N8o");
std::string total_category_doc_count("F1vs8KymEXNmmIod");

void version(const std::string & cmd) {
    fprintf(stderr,"%s Version %d.%d.%d\n", cmd.c_str(), nbdc_VERSION_MAJOR,
  			nbdc_VERSION_MINOR, nbdc_VERSION_PATCH);
}

void ReadTrFile(std::string filename,
			  std::map<std::string, std::ostringstream>& data) {
	//Opening the file
	std::ifstream in_file(filename);
	//Where the lines from the file will be stored
	std::string line;

	//While loop to get the line from the file
	while(std::getline(in_file, line)) {
		//A line from the file
		std::istringstream in_line(line);
		//Buffer to build up string. Using it locally, saves ~1s
		std::ostringstream os;
		//Where the word in the line will be stored
		std::string token;
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
			//Eliminates unwanted spaces in my stream
			if(count > 1) {
				os << " ";
			}

			//Adds the token to the local stream
			os << token;
			count++;
		}
		//Add the stream to the passed in stream
		data[cat] << " " << os.str();
	}
}

void ProcessData(const std::map<std::string, std::ostringstream>& data,
				 std::map<std::string, std::map<std::string, double>>&
				 processed_data,
				 std::map<std::string, std::map<std::string, int>>& cat_vocab,
				 std::map<std::string, int>& vocab,
				 std::map<std::string, int>& cat_doc_count) {
	int total_words;
	//Gets the indivudal category vocab
	for(auto const& map : data) {
		std::istringstream iss(map.second.str());
		std::string word;
		while(iss >> word) {
			cat_vocab[map.first][word]++;
			//cat_vocab[map.first][total_category_word_count]++;
		}
		for(auto const& word : cat_vocab[map.first]) {
			cat_vocab[map.first][total_category_word_count] += word.second;
		}
		cat_vocab[map.first][total_category_doc_count]++;
	}

	//Gets the total counts of all words found
	for(auto const& map_o : cat_vocab) {
		for(auto const& map_i : map_o.second) {
			vocab[map_i.first] += map_i.second;
		}
	}

	total_words = vocab.size();

	//Calculaltes the probability of the word for each category
	for(auto const& map_o : cat_vocab) {
		//map_o.first is the category string
		//map_o.second is the map of word counts
		double cat_total_words = 
					cat_vocab[map_o.first][total_category_word_count];
		double cat_total_docs = 
					cat_vocab[map_o.first][total_category_doc_count];
		cat_doc_count[map_o.first] = cat_total_docs;

		for(auto const& map_i : map_o.second) {
			//map_i.first is the word string
			//map_i.second is the word count for the category

			processed_data[map_i.first][map_o.first] =
						((double)map_i.second + (double)1) /
						(cat_total_words + (double)total_words);
		}
	}
}

void ReadTeFile(std::string filename,
			  std::map<std::string, std::vector<std::string>>& data) {
	//Opening the file
	std::ifstream in_file(filename);
	//Where the lines from the file will be stored
	std::string line;

	//While loop to get the line from the file
	while(std::getline(in_file, line)) {
		//A line from the file
		std::istringstream in_line(line);
		//Buffer to build up string. Using it locally, saves ~1s
		std::ostringstream os;
		//Where the word in the line will be stored
		std::string token;
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
			//Eliminates unwanted spaces in my stream
			if(count > 1) {
				os << " ";
			}

			//Adds the token to the local stream
			os << token;
			count++;
		}
		//Add the stream to the passed in stream
		data[cat].push_back(os.str());
	}
}

void AnalyzeData(const std::map<std::string, std::map<std::string, double>>&
				 processed_data,
				 const std::map<std::string, std::vector<std::string>>& data,
				 const std::map<std::string, int>& cat_doc_count) {

	int total(0);
	int correct(0);
	for(auto const& map : data) {
		for(auto const& line : map.second) {
			std::istringstream iss(line);
			std::string word;
			int count(0);
			std::map<std::string, double> probability_classification;

			auto start = std::chrono::high_resolution_clock::now();
			while(iss >> word) {
				if(count == 0) {
					count++;
					continue;
				}
				for(auto const& map_p : processed_data) {
					if(!(map_p.second.find(word) == map_p.second.end())) {
						double w_p(map_p.second.at(word));
						probability_classification[map.first] += std::log(w_p);
					}
				}
			}

			for(auto const& map_d : cat_doc_count) {
				double c_p(map_d.second);
				probability_classification[map_d.first] += log(c_p);
			}

			std::pair<std::string, double> result("", 0.0);
			for(auto const& map_r : probability_classification) {
				if(result.second < map_r.second) {
					result.first = map_r.first;
					result.second = map_r.second;
				}
			}
			if(result.first == map.first) {
				correct++;
			}
			total++;
		}
	}

	std::cout << "Total              : " << total << std::endl;
	std::cout << "Correct            : " << correct << std::endl;
	std::cout << "Accuracy           : " << (double)correct / (double)total << std::endl;
}


int main(int argc, char * argv[]){

	std::string cmd(argv[0]);

	if(argc != 3) {
		std::cerr << "Usage: " << cmd << " [TRAINING_FILEIN] [TEST_FILEIN]"
					<< std::endl;
		version(cmd);
		exit(EXIT_FAILURE);
	}

	std::string tr_filename(argv[1]);
	std::string te_filename(argv[2]);

	if(!nbdc::FileExists(tr_filename)) {
		version(cmd);
		std::cerr << "Training file doens't exist"<< std::endl;
		exit(EXIT_FAILURE);
	}
	if(!nbdc::FileExists(te_filename)) {
		version(cmd);
		std::cerr << "Test file doens't exist"<< std::endl;
		exit(EXIT_FAILURE);
	}

	std::map<std::string, std::ostringstream> tr_data;
	std::map<std::string, int> vocab;
	std::map<std::string, std::map<std::string, int>> cat_vocab;
	std::map<std::string, std::map<std::string, double>> processed_data;
	std::map<std::string, int> cat_doc_count;

	/**
	 *	Read the training data in to a data type to easily process
	 */
	auto start = std::chrono::high_resolution_clock::now();
	ReadTrFile(tr_filename, tr_data);
	auto end = std::chrono::high_resolution_clock::now();
	auto time_read1 = end - start;
	std::cout << "Training Read Time : " <<
				(std::chrono::duration_cast<std::chrono::nanoseconds>
				(time_read1).count()) / 1000000000.0 << "s" << std::endl;

	/**
	 *	Process data out into a data type to easily manage
	 */
	start = std::chrono::high_resolution_clock::now();
	ProcessData(tr_data, processed_data, cat_vocab, vocab, cat_doc_count);
	end = std::chrono::high_resolution_clock::now();
	auto time_process = end - start;
	std::cout << "Process Time       : " <<
				(std::chrono::duration_cast<std::chrono::nanoseconds>
				(time_process).count()) / 1000000000.0 << "s" << std::endl;

	/**
	 *	Read the Test data in to a data type to easily process
	 */
	std::map<std::string, std::vector<std::string>> te_data;
	start = std::chrono::high_resolution_clock::now();
	ReadTeFile(te_filename, te_data);
	end = std::chrono::high_resolution_clock::now();
	auto time_read2 = end - start;
	std::cout << "Test Read Time     : " <<
				(std::chrono::duration_cast<std::chrono::nanoseconds>
				(time_read2).count()) / 1000000000.0 << "s" << std::endl;

	/**
	 *	Analyze the data based on the training data.
	 */
	start = std::chrono::high_resolution_clock::now();
	AnalyzeData(processed_data, te_data, cat_doc_count);
	end = std::chrono::high_resolution_clock::now();
	auto time_analyze = end - start;
	std::cout << "Analyze Time       : " <<
				(std::chrono::duration_cast<std::chrono::nanoseconds>
				(time_analyze).count()) / 1000000000.0 << "s" << std::endl;

	auto total_run_time = time_analyze + time_read2 + time_process + time_read1;
	std::cout << "Total Run Time     : " <<
				(std::chrono::duration_cast<std::chrono::nanoseconds>
				(total_run_time).count()) / 1000000000.0 << "s" << std::endl;
	exit(EXIT_SUCCESS);
}

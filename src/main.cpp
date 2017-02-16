#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <chrono>
#include <complex>
#include <algorithm>
#include <iomanip>

#include <omp.h>

#include "nbdc_config.hpp"
#include "Func/func.hpp"

void version(const std::string & cmd) {
    fprintf(stderr,"%s Version %d.%d.%d\n", cmd.c_str(), nbdc_VERSION_MAJOR,
  			nbdc_VERSION_MINOR, nbdc_VERSION_PATCH);
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

	//Base training data
	std::map<std::string, std::vector<std::vector<std::string>>> tr_data;
	std::map<std::string, std::map<std::string, int>> tr_data_info;

	//Processed data
	std::map<std::string, int> vocab;
	std::map<std::string, std::map<std::string, int>> cat_vocab;
	std::map<std::string, std::map<std::string, double>> processed_data;
	std::map<std::string, std::map<std::string, double>> processed_data_info;

	/**
	 *	Read the training data in to a data type to easily process
	 */
	auto start = std::chrono::high_resolution_clock::now();
	nbdc::ReadFile(tr_filename, tr_data, tr_data_info);
	auto end = std::chrono::high_resolution_clock::now();
	auto time_read1 = end - start;
	std::cout << "Training Read Time : " <<
				(std::chrono::duration_cast<std::chrono::nanoseconds>
				(time_read1).count()) / 1000000000.0 << "s" << std::endl;

	/**
	 *	Process data out into a data type to easily manage
	 */
	start = std::chrono::high_resolution_clock::now();
	nbdc::ProcessData(tr_data, tr_data_info, processed_data, processed_data_info, cat_vocab, vocab);
	end = std::chrono::high_resolution_clock::now();
	auto time_process = end - start;
	std::cout << "Process Time       : " <<
				(std::chrono::duration_cast<std::chrono::nanoseconds>
				(time_process).count()) / 1000000000.0 << "s" << std::endl;

	/**
	 *	Read the Test data in to a data type to easily process
	 */
	//Base test data
	std::map<std::string, std::vector<std::vector<std::string>>> te_data;
	std::map<std::string, std::map<std::string, int>> te_data_info;
	start = std::chrono::high_resolution_clock::now();
	nbdc::ReadFile(te_filename, te_data, te_data_info);
	end = std::chrono::high_resolution_clock::now();
	auto time_read2 = end - start;
	std::cout << "Test Read Time     : " <<
				(std::chrono::duration_cast<std::chrono::nanoseconds>
				(time_read2).count()) / 1000000000.0 << "s" << std::endl;

	/**
	 *	Analyze the data based on the training data.
	 */
	std::map<std::string, std::map<std::string, int>> confusion_matrix;
	start = std::chrono::high_resolution_clock::now();
	nbdc::AnalyzeData(processed_data, processed_data_info, te_data, te_data_info, confusion_matrix);
	end = std::chrono::high_resolution_clock::now();
	auto time_analyze = end - start;
	std::cout << "Analyze Time       : " <<
				(std::chrono::duration_cast<std::chrono::nanoseconds>
				(time_analyze).count()) / 1000000000.0 << "s" << std::endl;

	for(auto const& matrix1 : confusion_matrix) {
		for(auto const& matrix2 : confusion_matrix) {
			if(matrix1.second.find(matrix2.first) == matrix1.second.end()) {
				confusion_matrix[matrix1.first][matrix2.first] = 0;
			}
		}

	}

	auto total_run_time = time_analyze + time_read2 + time_process + time_read1;
	std::cout << "Total Run Time     : " <<
				(std::chrono::duration_cast<std::chrono::nanoseconds>
				(total_run_time).count()) / 1000000000.0 << "s" << std::endl;

	nbdc::PrintConfusionMatrix(confusion_matrix);
	exit(EXIT_SUCCESS);
}

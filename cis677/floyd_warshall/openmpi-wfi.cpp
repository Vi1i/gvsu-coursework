/*******************************************************************************
 *
 ******************************************************************************/
#include <vector>
#include <iostream>
#include <climits>
#include <chrono>
#include <omp.h>

#define INF 99999
#define MAX_WEIGHT 20

std::vector<std::vector<int>> graph;
bool par_enable = false;
bool print = false;

void FloydWarshall(std::vector<std::vector<int>> * dist) {
    unsigned int size = graph.size();
    unsigned int i;
    unsigned int j;
    unsigned int k;

    #pragma omp parallel for if(par_enable) \
        default(shared) private(i, j, k)
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            for(k = 0; k < size; k++){
                if(graph[i][k] + graph[k][j] < graph[i][j]) {
                    (*dist)[i][j] = graph[i][k] + graph[k][j];
                }
            }
         }
    }
}

void PrintGraph(std::vector<std::vector<int>> dist) {
    for(std::vector<std::vector<int>>::const_iterator it = dist.begin();
            it != dist.end(); ++it) {
        for(std::vector<int>::const_iterator point = it->begin();
                point != it->end(); ++point) {
            if(*point == INF){
                std::cout << "INF\t";
            }else{
                std::cout << *point << "\t";
            }
        }
        std::cout << std::endl;
    }
}

void GenAdjMatrix(int size, int connectivity) {
    int i;
    int j;

    srand((unsigned) time(0));
    for(i = 0; i < size; i++) {
        graph.push_back(std::vector<int>());
        for(j = 0; j < size; j++) {
            if(((rand() % size) < connectivity) && i != j) {
                graph[i].push_back(1 + (rand() % MAX_WEIGHT));
            }else{
                graph[i].push_back(INF);
            }
        }
    }
}


int main(int argc, char * argv[]) {
    int size = 100;
    int complexity = 100;
    for(auto z = 1; z < argc; z++) {
        if(std::string(argv[z]) == "--parallel") {
            par_enable = true;
        }else if(std::string(argv[z]) == "--print") {
            print = true;
        }else if(std::string(argv[z]) == "--size") {
            z++;
            size = std::stoi(argv[z]);
        }else if(std::string(argv[z]) == "--complexity") {
            z++;
            complexity = std::stoi(argv[z]);
        }
    }
    auto start = std::chrono::high_resolution_clock::now();
    GenAdjMatrix(size, complexity);
    auto stop = std::chrono::high_resolution_clock::now();
    auto time_gen = stop - start;
    std::vector<std::vector<int>> * dist =
        new std::vector<std::vector<int>>(graph);

    start = std::chrono::high_resolution_clock::now();
    FloydWarshall(dist);
    stop = std::chrono::high_resolution_clock::now();
    auto time_comp = stop - start;

    // size : complexity : Serial Time(s) : Computation Time(s)
    std::cout << size << " : " << complexity  << " : " <<
        (std::chrono::duration_cast<std::chrono::nanoseconds>
         (time_gen).count()) /1000000000.0 << "s : " <<
        (std::chrono::duration_cast<std::chrono::nanoseconds>
         (time_comp).count()) /1000000000.0 << "s" << std::endl;


    if(print) {
        PrintGraph(graph);
        PrintGraph(*dist);
    }
}

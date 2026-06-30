#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include "flesch-index_config.h"
#include "flesch-index.hpp"

int main (int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stdout,"%s Version %d.%d.%d\n",
                    argv[0], flesch_index_VERSION_MAJOR,
                    flesch_index_VERSION_MINOR, flesch_index_VERSION_PATCH);

        fprintf(stdout,"Usage: %s [FILEIN]\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    std::string filename = argv[1];

    fi::Flesch_Index fi = fi::Flesch_Index(filename);
    fi.Read();
    fi.Analyze();
    fi.Print();
    exit(EXIT_SUCCESS);
}

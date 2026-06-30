/*******************************************************************************
 * readWrite-bmp.cpp
 *
 ******************************************************************************/
#include <iostream>
#include <fstream>
//#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
//#include <omp.h>
#include <sys/stat.h>
#include <pthread.h>
#include <cstring>
#include <chrono>

namespace {
#pragma pack(1)
    typedef struct {
        int row_count;
        int start_row;
        int threshold;
    } helper_data;

#pragma pack(1)
    typedef struct {
        char id[2]; 
        int file_size;
        int reserved;
        int offset;
    }  header_type;

#pragma pack(1)
    typedef struct {
        int header_size;
        int width;
        int height; 
        unsigned short int color_planes; 
        unsigned short int color_depth; 
        unsigned int compression; 
        int image_size;
        int xresolution;
        int yresolution; 
        int num_colors;
        int num_important_colors;
    } information_type;

    information_type information;
    header_type header;
    std::vector<std::vector <int> > data;
    std::vector<std::vector <int> > newData;
    int padding;


    /**
     * Simple check to make sure the file exists.
     */
    inline bool file_exists(const std::string& name) {
        struct stat buffer;   
        return (stat(name.c_str(), &buffer) == 0); 
    }

    inline int *  get_pixels(int row, int col) {
        int * pixels = new int[9];
        for(auto z = 0; z < 9; z++) {
            pixels[z] = -1;
        }
        for(auto z = 0; z < 9; z++) {
            if(pixels[z] == -1) {
                switch(z) {
                    case 0:
                        pixels[z] = data[row - 1][col - 1];
                        break;
                    case 1:
                        pixels[z] = data[row - 1][col];
                        break;
                    case 2:
                        pixels[z] = data[row - 1][col + 1];
                        break;
                    case 3:
                        pixels[z] = data[row][col - 1];
                        break;
                    case 4:
                        pixels[z] = data[row][col];
                        break;
                    case 5:
                        pixels[z] = data[row][col + 1];
                        break;
                    case 6:
                        pixels[z] = data[row + 1][col - 1];
                        break;
                    case 7:
                        pixels[z] = data[row + 1][col];
                        break;
                    case 8:
                        pixels[z] = data[row + 1][col + 1];
                        break;
                }
            }
        }
        return pixels;
    }

    inline double gradient_x(int pixels[9]) {
        double result = 0.0;

        result = pixels[0] + pixels[1] + pixels[2];
        result -= (pixels[6] + pixels[7] + pixels[8]);
        result = abs(result);

        return result;
    }

    inline double gradient_y(int pixels[9]) {
        double result = 0.0;
        result = pixels[2] + pixels[5] + pixels[8];
        result -= (pixels[0] + pixels[3] + pixels[6]);
        result = abs(result);

        return result;
    }

    inline void * sobel_helper(void * sobel_helper_data) {
        helper_data * args = (helper_data *)sobel_helper_data;

        for(auto row = args->start_row; row < args->start_row + args->row_count;
                row++) {
            for(auto col = 0; col < information.width; col++) {
                int * pixels = get_pixels(row + 1,col + 1);
                int sum = gradient_x(pixels) + gradient_y(pixels);
                sum = sum > 255 ? 255 : sum;
                sum = sum < 0 ? 0 : sum;
                sum = sum < args->threshold ? 255 : 0;
                newData[row][col] = sum;
            }
        }
        return NULL;
    }

    void read_image(const std::string image_filename) {
        std::ifstream image_file;
        //header_type header;
        int row_bytes;
        unsigned char tempData[3];

        image_file.open (image_filename.c_str(), std::ios::binary);
        if(!image_file) {
            std::cerr << "file not found" << std::endl;
            exit(EXIT_FAILURE);
        }
        //Check that the file exists
        if(!file_exists(image_filename)){
            std::cerr << "File " << image_filename << " doesn't exist!"
                << std::endl;
        }
        //Read file header
        image_file.read((char *) &header, sizeof(header_type));
        if(header.id[0] != 'B' || header.id[1] != 'M') {
            std::cerr << "Does not appear to be a .bmp file.  Goodbye."
                << std::endl;
            exit(-1);
        }
        //Read/compute image information
        image_file.read ((char *) &information, sizeof(information_type));
        row_bytes = information.width * 3;
        padding = row_bytes % 4;
        if(padding){
            padding = 4 - padding;
        }

        for(auto row = 0; row < information.height + 2; row++) {
            data.push_back(std::vector <int>());
            for(auto col = 0; col < information.width + 2; col++) {
                data[row].push_back(0);
            }
        }
        //Extract image data, initialize vectors
        for(auto row = 0; row < information.height; row++) {
            newData.push_back(std::vector <int>());
            for(auto col = 0; col < information.width; col++) {
                image_file.read((char *) tempData, 3 * sizeof(unsigned char));
                data[row + 1][col + 1] = ((int) tempData[0]);
                newData[row].push_back(0);
            }
            if(padding){
                image_file.read((char *) tempData, padding *
                        sizeof(unsigned char));
            }
        }
        image_file.close();
    }

    void run_threads(int thread_count, int threshold) {
        int status;
        void * result;
        pthread_t tid[thread_count];

        int extra_rows_to_calc = information.height % thread_count;
        int rows_to_calc = information.height / thread_count;
        int row_start = 0;

        for (auto thread = 0; thread < thread_count; thread++) {
            int tmp_rows;
            if(extra_rows_to_calc != 0){
                tmp_rows = rows_to_calc + 1;
                extra_rows_to_calc--;
            }else{
                tmp_rows = rows_to_calc;
            }
            helper_data * temp_data = new helper_data;
            temp_data->row_count =  tmp_rows;
            temp_data->start_row =  row_start;
            temp_data->threshold =  threshold;

            if((status = pthread_create (&tid[thread], NULL,
                            sobel_helper, temp_data)) != 0) {
                std::cerr << "Thread create error " << status << ":"
                    << std::strerror(status) << std::endl;
                exit(EXIT_FAILURE);
            }
            row_start += tmp_rows;
        }
        for(auto thread = 0; thread < thread_count; thread++) {
            if ((status = pthread_join (tid[thread], &result)) != 0) {
                std::cerr << "Join error " << status << strerror(status) << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    void write_data(const std::string new_image_filename) {
        int row;
        int col;
        std::ofstream new_image_file;
        unsigned char tempData[3];
        //Open file to write out now data
        new_image_file.open(new_image_filename.c_str(), std::ios::binary);

        // write header to new image file
        new_image_file.write ((char *) &header, sizeof(header_type));
        new_image_file.write ((char *) &information, sizeof(information_type));

        // write new image data to new image file
        for (row=0; row < information.height; row++) {
            for (col=0; col < information.width; col++) {
                unsigned char temp = newData[row][col];
                tempData[0] = temp;
                tempData[1] = temp;
                tempData[2] = temp;
                new_image_file.write ((char *) tempData, 3 *
                        sizeof(unsigned char));
            }
            if (padding) {
                tempData[0] = 0;
                tempData[1] = 0;
                tempData[2] = 0;
                new_image_file.write ((char *) tempData, padding * 
                        sizeof(unsigned char));
            }
        }
        new_image_file.close();
    }
}

int main(int argc, char* argv[])
{
    //Variables
    std::string image_filename;
    std::string new_image_filename;

    /**
     * Check to make sure that there are two arguments
     */
    if(argc != 5){
        std::cerr << "Usage: " << argv[0] << " [INFILE.bmp] [OUTFILE]"
            << "[THREAD_COUNT] [THRESHOLD]" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Read in image file and write to global data
    image_filename = argv[1];
    new_image_filename = argv[2];
    int thread_count = std::stoi(argv[3]);
    int threshold = std::stoi(argv[4]);

    read_image(image_filename);

    auto start = std::chrono::high_resolution_clock::now();
    run_threads(thread_count, threshold);
    auto stop = std::chrono::high_resolution_clock::now();

    write_data(new_image_filename);

    auto time = stop - start;

    std::cout << "Time : " << std::chrono::duration_cast<std::chrono::nanoseconds>(time).count() << std::endl;
    return 0;
}


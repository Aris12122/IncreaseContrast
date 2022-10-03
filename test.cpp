#include<iostream>
#include<vector>
#include<string>
#include<filesystem>
#include<fstream>
#include<sstream>
#include<omp.h>


int main() {
    int value = 123;
    int threads = 100;
    #pragma omp parallel num_threads(threads)
    {
        value++;
        #pragma omp critical(cout)
        {
            std::cout << value++ << std::endl;
        }
    }
    return 0;
}
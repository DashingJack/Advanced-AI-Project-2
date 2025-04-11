#include "input_handler.hpp"
#include <iostream>

int main()
{
    inputs::handle_inputfile("../../input_files/input_file_example.txt");
    std::cout << "Number of cooks " << inputs::cooks;
}
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // const char* file_name = argv[1];

    std::fstream fs;
    fs.open(std::string("acca.txt"), std::fstream::in | std::fstream::out);

    fs.seekg(0 * 4096, std::ios::beg);

    void* p = malloc(1024);
    // char buf[1024];

    fs.read((char*) p, 20);

    if (fs.fail()) {
        std::cout << "Unable to open file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << std::string((char*) p) << std::endl;

    delete (char*) p;
    fs.close();

    return 0;
}
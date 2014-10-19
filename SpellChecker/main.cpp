#include <iostream>
#include <fstream>
#include <string>
#include "hash.h"
#include "spellcheck.h"
#include <time.h>

int main(void)
{
    clock_t timer;
    std::string dictionary, input, output;

    std::cout<<"Enter name of dictionary: ";
    getline(std::cin, dictionary);

    timer = clock();
    spellcheck * sp = new spellcheck(dictionary);
    timer = clock() - timer;
    std::cout<<"Total time (in seconds> to load dictionary: "<<(((float)timer)/CLOCKS_PER_SEC)<<std::endl;

    std::cout<<"Enter name of input file: ";
    getline(std::cin, input);

    std::cout<<"Enter name of output file: ";
    getline(std::cin, output);

    timer = clock();
    sp->spellchecker(input, output);
    timer = clock() - timer;
    std::cout<<"Total time (in seconds> to check document: "<<(((float)timer)/CLOCKS_PER_SEC)<<std::endl;
}

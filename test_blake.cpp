#include "rainbow_table.hpp"


/*
argv[1]: start of the chain
argv[2]: length of the chain
*/
using namespace std;
int main(int argc, char** argv){
    print_chain(atoi(argv[2]), argv[1]);
}
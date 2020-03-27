#include "rainbow_table.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@";

/*
argv[1]: rainbow table file path
argv[2]: hashes to crack file path
*/

//Se que el primer string mide 6 y el segundo lo que mida BLAKE (puedo calcular tamaño de vector)

int main (int argc, char* argv[]){
	char rainbow_table[50];
	int table_number = atoi(argv[1]);
	string rainbow_path(argv[1]);
	string hash_path (argv[2]);
	ifstream file(rainbow_path);
	ifstream hash_file(hash_path);
	string pass, hash;
	
	unordered_map<string, string> hashMap;
	while(!file.eof()){	//Build Map. BUILD DIFFERENT MAP FOR EACH DIFFERENT THREAD - READ NUMBER OF LINES, DISTRIBUTE THEM BETWEEN PROCESSES
		file >> pass;
		file >> hash;
		hashMap.insert(pair<string, string>(hash,pass));
	}
	char str[65];
	
	while(!hash_file.eof()){ //Read hashes to crack
		string hash2;
		hash_file >> hash2;
		searchHash(hashMap, hash2);
	}
	return 0;
}


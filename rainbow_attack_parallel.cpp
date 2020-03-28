#include "mpi.h"
#include <vector>
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

int main (int argc, char* argv[]){
	char rainbow_table[50];
	int table_number = atoi(argv[1]);
	string rainbow_path(argv[1]);
	string hash_path (argv[2]);
	ifstream file(rainbow_path);
	string pass, hash;
	unordered_map<string, string> hashMap;
	int id, numprocs;
	int size, chunk;
	
	while(!file.eof()){	//Build Map. BUILD DIFFERENT MAP FOR EACH DIFFERENT THREAD - READ NUMBER OF LINES, DISTRIBUTE THEM BETWEEN PROCESSES
		file >> pass;
		file >> hash;
		hashMap.insert(pair<string, string>(hash,pass));
	}
	char str[65];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

	string hash2;
	MPI_Status status;
	vector<string> hashes;
	string buffer;
	ifstream hash_file(hash_path, fstream::binary);
	int lines;
	int lines_per_process;
	if (hash_file){
		//Get file size
		hash_file.seekg(0, hash_file.end);
		size = hash_file.tellg();
		hash_file.seekg(0, hash_file.beg);

		//Discard last lines
		lines = size / 65;
		lines_per_process = lines / numprocs;

		//Read your corresponding lines
		hash_file.seekg(id*lines_per_process*65, hash_file.beg);
		
		for (int i = 0; i < lines_per_process; i++){
			hash_file >> buffer;
			hashes.push_back(buffer);
		}

		//Debug
		string first_hash = hashes[0];
		string sencond_hash = hashes[1];
		cout << "SOY " + to_string(id) + " FIRST HASH: " + first_hash + "\n";
		cout << "SOY " + to_string(id) + " SECOND HASH: " + sencond_hash + "\n";
		
		//Crack hashes
		for (auto it = hashes.begin(); it != hashes.end(); ++it){
			searchHash(hashMap, *it);
		}

	}
	MPI_Finalize();
}


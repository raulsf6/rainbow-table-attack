#include <vector>
#include "rainbow_table_cuda.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@";

/*
argv[1]: rainbow table file path
argv[2]: hashes to crack file path
*/

// Abrir en init
// Copiar el fichero 3 veces
// Mandarlo

int main (int argc, char* argv[]){
	
	int id, numprocs;
	int size, chunk;
	int total_cracked;
	double init_execution;
	double finish_execution;
	double total_execution;

	string rainbow_path(argv[1]);
	string hash_path (argv[2]);
	ifstream file(rainbow_path);
	string pass, hash;
	vector<string> keys;
	vector<string> values;
	unordered_map<string, string> hashMap;

	vector<string> hashes;
	string buffer;
	ifstream hash_file(hash_path);
	int lines;
	int lines_per_process;
	int cracked = 0;

    
    // Build Map - Can't CUDA - Change to two vectors
	while(!file.eof()){	
		file >> pass;
		file >> hash;
		keys.push_back(pass);
		values.push_back(hash);
	}

	if (hash_file){
        // Read hashes - Can't CUDA
		while (!hash_file.eof()){
			hash_file >> buffer;
			hashes.push_back(buffer);
		}

        cout << "Readed " << hashes.size() << " hashes" << endl;

		//Crack hashes - maybe CUDA
		for (auto it = hashes.begin(); it != hashes.end(); ++it){
			if(searchHash(keys, values, *it)){
				cracked++;
			}
		}

        //Result
        cout << "Cracked " << cracked << " hashes" << endl;
        
	}
	else
	{
		cout << "Error reading hash file";
	}
    
}

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
	char* buffer;

	if (id == 0 ){
		//Read hashes to crack
		while(!hash_file.eof()){ 
			hash_file >> hash2;
			hashes.push_back(hash2);
		}
		
		//Get size and chunk
		size = hashes.size()*64;
		chunk = size/numprocs;
		
		//Debug
		int total_length = 0;
		int total_capacity = 0;
		for (auto it = hashes.begin(); it != hashes.end(); ++it){
			total_length += (*it).length();
			total_capacity += (*it).capacity();
		}
		cout << "TAMAÑO DE VECTOR " + to_string(total_length) + "\n";
		cout << "CAPACITY " + to_string(total_capacity) + "\n";
		
		//Send size
		for (int i = 1; i < numprocs; i++){
			MPI_Send(&chunk, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
		
		//Send vector
		for (int i = 0; i < numprocs; i++){
			int to_start = i * hashes.size() / numprocs;
			if (i == 0){ //Copy my own buffer
				buffer = new char[chunk];
				strcpy(buffer, hashes[to_start].c_str());
			}
			else{
				MPI_Send(hashes[to_start].c_str(), chunk, MPI_CHAR, i, 0, MPI_COMM_WORLD);
			}
			hashes.empty();
		}
	}
	else {
		MPI_Recv(&chunk, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		cout << "SOY " + to_string(id) + " SIZE A LEER " + to_string(chunk) + "\n";
		buffer = new char[chunk];
		cout << "RESERVO" << "\n";
		MPI_Recv(buffer, chunk, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	}

	cout << "SOY " + to_string(id) + " TAMAÑO DE VECTOR " + "\n";
		for (int i = 0; i < 64; i++){
			cout << buffer[i];
			
		}
		cout << "\n";
	
	MPI_Finalize();

	/*
	1 - Todos leen el hashfile
	2 - 0 lee hashfile y reparte ----> CHOSEN
	3 - Cada uno lee su parte de hashfile
	*/

	
}


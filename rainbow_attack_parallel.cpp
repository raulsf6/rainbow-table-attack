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
	int size;
	
	while(!file.eof()){	//Build Map. BUILD DIFFERENT MAP FOR EACH DIFFERENT THREAD - READ NUMBER OF LINES, DISTRIBUTE THEM BETWEEN PROCESSES
		file >> pass;
		file >> hash;
		hashMap.insert(pair<string, string>(hash,pass));
	}
	char str[65];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
	
	cout << "Soy " + to_string(id) + " " + (*hashMap.begin()).first + "\n";

	string hash2;
	MPI_Status status;
	vector<string> hashes;

	if (id == 0 ){
		//Read hashes to crack
		while(!hash_file.eof()){ 
			hash_file >> hash2;
			hashes.push_back(hash2);
		}
		
		//Get size
		size = hashes.size()*64;
		
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
			MPI_Send(&size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
		
		//Send vector
		for (int i = 1; i < numprocs; i++){
			MPI_Send(hashes[0].c_str(), size, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		}	
	}
	else {
		MPI_Recv(&size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		cout << "SOY " + to_string(id) + " SIZE A LEER " + to_string(size) + "\n";
		char* buffer = new char[size];
		//hashes.reserve(size);
		cout << "RESERVO" << "\n";
		MPI_Recv(buffer, size, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		cout << "SOY " + to_string(id) + " TAMAÑO DE VECTOR " + "\n";
		for (int i = 0; i < 64; i++){
			cout << buffer[i];
			
		}
		cout << "\n";
	}
	
	MPI_Finalize();

	/*
	1 - Todos leen el hashfile
	2 - 0 lee hashfile y reparte ----> CHOSEN
	3 - Cada uno lee su parte de hashfile
	*/

	
}


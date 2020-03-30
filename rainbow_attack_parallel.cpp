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

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Status status;

	if (id == 0)
		init_execution = MPI_Wtime();

	char rainbow_table[50];
	int table_number = atoi(argv[1]);
	string rainbow_path(argv[1]);
	string hash_path (argv[2]);
	ifstream file(rainbow_path);
	string pass, hash;
	unordered_map<string, string> hashMap;

	vector<string> hashes;
	string buffer;
	ifstream hash_file(hash_path, fstream::binary);
	int lines;
	int lines_per_process;
	int cracked = 0;

	while(!file.eof()){	//Build Map. BUILD DIFFERENT MAP FOR EACH DIFFERENT THREAD - READ NUMBER OF LINES, DISTRIBUTE THEM BETWEEN PROCESSES
		file >> pass;
		file >> hash;
		hashMap.insert(pair<string, string>(hash,pass));
	}
	char str[65];

	if (hash_file){
		//Get file size
		hash_file.seekg(0, hash_file.end);
		size = hash_file.tellg();
		hash_file.seekg(0, hash_file.beg);

		//Discard last lines
		lines = size / 65;
		lines_per_process = lines / numprocs;
		if (id == numprocs -1)
			lines_per_process += lines % numprocs;

		//Read your corresponding lines
		hash_file.seekg(id*lines_per_process*65, hash_file.beg);
		
		for (int i = 0; i < lines_per_process; i++){
			hash_file >> buffer;
			hashes.push_back(buffer);
		}

		//Debug
		string first_hash = hashes[0];
		string sencond_hash = hashes[1];
		cout << "SOY " + to_string(id) + " LEO: " + to_string(lines_per_process) + "\n";

		//Crack hashes
		double t_init_calculation = MPI_Wtime();
		for (auto it = hashes.begin(); it != hashes.end(); ++it){
			if(searchHash(hashMap, *it))
				cracked++;
			
		}
		double t_finish_calculation = MPI_Wtime();
		double t_elapsed_calculation = t_finish_calculation - t_init_calculation;

		

		MPI_Reduce( &cracked, &total_cracked, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		cout << "ELAPSED FOR " + to_string(id) + ": " + to_string(t_elapsed_calculation) + "\n";
		
		if (id == 0){
			finish_execution = MPI_Wtime();
			total_execution = finish_execution - init_execution;
			cout << "TOTAL CRACKED: " + to_string(total_cracked) + "\n";
			cout << "TOTAL TIME: " + to_string(total_execution) + "\n";
		}
	}
	else
	{
		if (id == 0){
			cout << "Error reading hash file";
		}
	}
	
	
	MPI_Finalize();
}


#include "rainbow_table.hpp"

using namespace std;
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@";


int main(int argc, char **argv){	
	srand(time(NULL)+getpid());
	int table_number = atoi(argv[1]);
	char filename[20];
	sprintf(filename,"table%d.csv",table_number);
	FILE* fp = fopen(filename, "w");
	for (int i = 0; i < NUM_OF_CHAINS; ++i){
		build_chain(CHAIN_LENGTH, fp);
	}
	fclose(fp);
	return 0;
}

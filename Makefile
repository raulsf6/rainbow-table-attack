#
# In order to execute this "Makefile" just type "make"
#

OBJSBUILD 	= rainbow_table_builder.o blake256.o base64.o rainbow_table.o MurmurHash3.o
OBJSATTACK 	= rainbow_attack.o rainbow_table.o blake256.o base64.o MurmurHash3.o
OBJSATTACKPAR 	= rainbow_attack_parallel.o rainbow_table.o blake256.o base64.o MurmurHash3.o
OBJSATTACKCUDA 	= rainbow_attack_cuda.o rainbow_table_cuda.o blake256.o base64.o MurmurHash3.o
OBJSBLAKE	= test_blake.o rainbow_table.o blake256.o base64.o MurmurHash3.o
SOURCEBUILD	= rainbow_table_builder.cpp blake256.cpp base64.cpp rainbow_table.cpp MurmurHash3.cpp
SOURCEATTACK	= rainbow_attack.cpp rainbow_table.cpp blake256.cpp base64.cpp MurmurHash3.cpp
HEADER  = blake.h rainbow_table.hpp MurmurHash3.h
OUTBUILD  	= build_rainbow_table
OUTATTACK  	= rainbow_attack
OUTBLAKE	= test_blake
OUTATTACKPAR	= rainbow_attack_parallel
OUTATTACKCUDA	= rainbow_attack_cuda
CC		= g++
MPI		= mpic++
FLAGS 	= -c -g -O2 -std=c++11 --static
LFLAGS 	= -L/gmp_install/lib -lgmp -lpthread -lm -lssl -lcrypto 
MPILFLAGS = -L/usr/lib/x86_64-linux-gnu/openmpi/lib -lgmp -lpthread -lm -lssl -lcrypto
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: BUILD ATTACK BLAKE ATTACK_PARALLEL ATTACK_CUDA

BUILD: $(OBJSBUILD)
	$(CC) $(OBJSBUILD) $(LFLAGS) -o $(OUTBUILD)

ATTACK: $(OBJSATTACK)
	$(CC) $(OBJSATTACK) $(LFLAGS) -o $(OUTATTACK)

BLAKE: $(OBJSBLAKE)
	$(CC) $(OBJSBLAKE) $(LFLAGS) -o $(OUTBLAKE)

ATTACK_PARALLEL: $(OBJSATTACKPAR)
	$(MPI) $(OBJSATTACKPAR) $(MPILFLAGS) -o $(OUTATTACKPAR)

ATTACK_CUDA: $(OBJSATTACKCUDA)
	$(CC) $(OBJSATTACKCUDA) $(LFLAGS) -o $(OUTATTACKCUDA)


# create/compile the individual files >>separately<< 
rainbow_table.o: rainbow_table.cpp rainbow_table.hpp
	$(CC) $(FLAGS) rainbow_table.cpp

rainbow_table_builder.o: rainbow_table_builder.cpp rainbow_table.hpp
	$(CC) $(FLAGS) rainbow_table_builder.cpp

rainbow_attack.o: rainbow_attack.cpp rainbow_table.hpp
	$(CC) $(FLAGS) rainbow_attack.cpp

rainbow_attack_parallel.o: rainbow_attack_parallel.cpp rainbow_table.hpp
	$(MPI) $(FLAGS) rainbow_attack_parallel.cpp

rainbow_attack_cuda.o: rainbow_attack_cuda.cpp rainbow_table_cuda.hpp
	$(CC) $(FLAGS) rainbow_attack_cuda.cpp

test_blake.o: test_blake.cpp
	$(CC) $(FLAGS) test_blake.cpp

blake256.o: blake256.cpp blake.h
	$(CC) $(FLAGS) blake256.cpp

base64.o: base64.cpp base64.h
	$(CC) $(FLAGS) base64.cpp

clean:
	rm -f $(OUTBUILD) $(OBJSATTACK) $(OBJSATTACKCUDA) $(OBJSATTACKPAR) $(OUT)
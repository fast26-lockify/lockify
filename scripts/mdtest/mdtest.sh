#!/bin/bash

TEST_DIR="/path/to/mnt"		
FILE_COUNT_PER_PROCESS=5000
TREE_DEPTH=2	
BRANCH_FACTOR=2		
NPROCS=1			
ITERATIONS=1				

sudo bash -c "echo 3 > /proc/sys/vm/drop_caches"

mpirun -np $NPROCS src/mdtest -i $ITERATIONS -I=$FILE_COUNT_PER_PROCESS -z $TREE_DEPTH -b $BRANCH_FACTOR -d $TEST_DIR -C 


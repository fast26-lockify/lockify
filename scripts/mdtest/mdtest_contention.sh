#!/bin/bash

TEST_DIR="/path/to/mnt"   
FILE_COUNT_PER_PROCESS=1000      
TREE_DEPTH=2                     
BRANCH_FACTOR=2                  
NPROCS=5                         
ITERATIONS=1                     
HOSTFILE="./hosts"         

mpirun \
  --hostfile "$HOSTFILE" \
  -np "$NPROCS" \
  --map-by node \
  -v \
  src/mdtest \
    -i "$ITERATIONS" \
    -I "$FILE_COUNT_PER_PROCESS" \
    -z "$TREE_DEPTH" \
    -b "$BRANCH_FACTOR" \
    -d "$TEST_DIR" \
    -C 


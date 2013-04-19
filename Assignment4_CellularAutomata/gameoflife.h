/*
 * gameoflife.h
 * Simulates Conway's Game of Life in MPI.
 *
 *  Created on: Apr 17, 2013
 *      Author: Kyle Mercer
 */

#ifndef GAMEOFLIFE_H_
#define GAMEOFLIFE_H_

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <mpi.h>

#define DIM 16 //This value should not change unless you change the global grid content accordingly
#define NUM_CYCLES 2


int globalGrid[DIM*DIM] = {	0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
							1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

//Holds data relevant to this process
typedef struct myMpiProcess {
	int rank;
	int comSize; //should be the same for all
} mpiProcess;

//Needed methods

void printGrid();
void iterateOnce();
void processChunk(int *, int);
int aliveNext(int);
int neightbors(int);

void debugPrint(int *, int);


#endif /* GAMEOFLIFE_H_ */

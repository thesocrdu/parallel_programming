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
#define NUM_CYCLES 64

//Starting board with DIM of 16
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
	int prev;
	int next;
} mpiProcess;

//Needed methods
void printGrid();
void printLocalGrid(int *arr, int size);
void fillRow(int *currRow, int *upperRow, int *lowerRow, int rowNum, int *temp);
int aliveNext(int *currRow, int col, int *upperRow, int *lowerRow, int leftCol, int rightCol);

#endif /* GAMEOFLIFE_H_ */

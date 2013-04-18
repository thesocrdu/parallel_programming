/*
 * gameoflife.h
 * Simulates Conway's Game of Life in MPI.
 *
 *  Created on: Apr 17, 2013
 *      Author: Kyle Mercer
 */

#include "gameoflife.h"

//Holds the worker's info
mpiProcess thisProcess;

int main(int argc, char **argv) {
	int i;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &(thisProcess.comSize));

	MPI_Comm_rank(MPI_COMM_WORLD, &(thisProcess.rank));
	printf("My Rank: %d\n", thisProcess.rank);

	for(i = 0; i < NUM_CYCLES; i++) {
		iterateOnce();
	}

	MPI_Finalize();

}

void printGrid() {
	int i,j;
	for(i = 0; i < DIM; i++) {
		for(j = 0; j < DIM; j++) {
			printf("%d ", globalGrid[DIM*i+j]);
		}
		printf("\n");
	}
}

void iterateOnce() {
	int rowsPerProcess = DIM / thisProcess.comSize;

	//STUCK HERE KYLE? SEE BOOKMARK YOU MADE YESTERDAY TO REFRESH YOUR MEMORY ON HOW TO COMPLETE THIS PART...

	if(thisProcess.rank == 0) {
		//send jobs
		//process 0's chunk .. with function call?
		//receive back data from other processes
	} else {
		//receive job from master
		//process its chunk .. with function call
		//send data back to master
	}
	//update global here?
}

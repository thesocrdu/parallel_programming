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

/**
 * Program entry point.
 */
int main(int argc, char **argv) {
	int i;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &(thisProcess.comSize));

	MPI_Comm_rank(MPI_COMM_WORLD, &(thisProcess.rank));
	printf("My Rank: %d\n", thisProcess.rank);

	for(i = 0; i < NUM_CYCLES; i++) {
		iterateOnce();
		printGrid();
	}

	MPI_Finalize();

}

/**
 * This function prints the entire
 * grid at the end of each generation.
 */
void printGrid() {
	int i,j;
	for(i = 0; i < DIM; i++) {
		for(j = 0; j < DIM; j++) {
			printf("%d ", globalGrid[DIM*i+j]);
		}
		printf("\n");
	}
}

///**
// * Runs a single iteration of the game of life.
// *
// * It divides up the work and distributes it
// * to each processes and waits for them to
// * complete it.  Once completed, process 0
// * receives the data and updates the global board.
// */
//void iterateOnce() {
//	MPI_Status status;
//	int i;
//	int rowsPerProcess = DIM / thisProcess.comSize;
//
//	if(thisProcess.rank == 0) {
//		//send jobs
//		for(i = 0; i < thisProcess.comSize; i++) {
//
//		}
//
//		//process 0's chunk .. with function call?
//		//receive back data from other processes
//	} else {
//		//receive job from master
//		//process its chunk .. with function call
//		//send data back to master
//	}
//	//update global here?
//}

/**
 * Runs a single iteration of the game of life.
 *
 * It divides up the work and distributes it
 * to each processes and waits for them to
 * complete it.  Once completed, process 0
 * receives the data and updates the global board.
 */
void iterateOnce() {
	MPI_Status status;
	int i;
	int rowsPerProcess = DIM / thisProcess.comSize;

	int newDataArrSize = rowsPerProcess * DIM;

	int *newData = malloc(sizeof(int) * newDataArrSize);

	if(thisProcess.rank == 0) {
		processChunk(newData, thisProcess.rank * DIM);
		memcpy(&globalGrid[0], newData, sizeof(int) * newDataArrSize);
		//collect data from others
		for(i = 1; i < thisProcess.comSize; i++) {
			MPI_Recv(newData, newDataArrSize, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			memcpy(&globalGrid[i * DIM], newData, sizeof(int) * newDataArrSize);
		}

		for(i = 0; i < thisProcess.comSize; i++) {
			MPI_Send(globalGrid, DIM*DIM, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	} else {
		processChunk(newData, thisProcess.rank * DIM);
		MPI_Send(newData, newDataArrSize, MPI_INT, 0, 0, MPI_COMM_WORLD);

		//Wait for updated grid
		MPI_Recv(globalGrid, DIM*DIM, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	}
}

/**
 * Fills the newData array of the next generation
 * for this processes assigned chunk.
 */
void processChunk(int *newData, int startIndex) {
	int arrSize = DIM*DIM / thisProcess.comSize;
	int endIndex = startIndex + arrSize;
	int i;
	int count = 0;

	for(i = startIndex; i < endIndex; i++) {
		if(aliveNext(i) == 1) {
			newData[count] = 1;
		} else {
			newData[count] = 0;
		}
		count++;
	}
}

/**
 * Returns 1 if the cell at index will
 * be alive in the next generation.
 * Returns 0 otherwise.
 */
int aliveNext(index) {
	int neighbors = numNeighbors(index);
	int currentState = globalGrid[index];

	if(currentState == 1) {
		if(neighbors == 2 || neighbors == 3) {
			return 1;
		} else {
			return 0;
		}
	} else {
		if(neighbors == 3) {
			return 1;
		}
	}
	return 0;
}

/**
 * Get the number of neighbors for
 * the cell at index
 */
int numNeighbors(int index) {
	int i,j;
	int x = index / DIM;
	int y = index % DIM;
	int neighbors = 0;

	for(i = -1; i <= 1; i++) {
		if(x + i < 0) {
			x = DIM - 1;
		} else if(x + i >= DIM) {
			x = 0;
		}
		for(j = -1; j <= 1; j++) {
			if(i == 0 && j == 0) {
				continue;
			}
			if(y + j < 0) {
				y = DIM - 1;
			} else if(y + j >= DIM) {
				y = 0;
			}
			neighbors += globalGrid[DIM*x + y];
		}
	}
	return neighbors;
}

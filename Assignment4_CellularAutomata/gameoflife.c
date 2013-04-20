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
	fflush(stdout);
	int i;

	//Initialize MPI.
	MPI_Init(&argc, &argv);
	//Get size and rank and store for each process.
	MPI_Comm_size(MPI_COMM_WORLD, &(thisProcess.comSize));
	MPI_Comm_rank(MPI_COMM_WORLD, &(thisProcess.rank));

	//Print starting board.
	if(thisProcess.rank == 0) {
		printf("Initial board:\n");
		printGrid();
	}

	for(i = 1; i <= NUM_CYCLES; i++) {
		//Step one generation.
		iterateOnce();
		//Print grid once per generation.
		if(thisProcess.rank == 0) {
			printf("Iteration %d\n---------------\n", i);
			printGrid();
		}
	}

	//Done with all calls to MPI.
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
	printf("\n");
}

/**
 * Runs a single iteration of the game of life.
 *
 * Each process knows what it needs to do.  Once
 * a process is done updating its piece of the
 * board, it sends the master process (rank 0)
 * the updated chunk of the board, after which
 * the master pieces them together.  The master
 * then distributes the updated board to all of
 * the workers so they can compute the next
 * generation properly.
 */
void iterateOnce() {
	MPI_Status status;
	int i;
	int rowsPerProcess = DIM / thisProcess.comSize;
	int newDataArrSize = rowsPerProcess * DIM;
	int *newData = malloc(sizeof(int) * newDataArrSize);

	if(thisProcess.rank == 0) {
		//Compute next generation chunk.
		processChunk(newData, newDataArrSize);

		//Copy the chunk directly to the globalGrid.
		memcpy(&globalGrid[0], newData, sizeof(int) * newDataArrSize);

		//Collect chunks from all workers and piece them together.
		for(i = 1; i < thisProcess.comSize; i++) {
			MPI_Recv(newData, newDataArrSize, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			memcpy(&globalGrid[rowsPerProcess * i * DIM], newData, sizeof(int) * newDataArrSize);
		}

		//Send updated board to all workers.
		for(i = 0; i < thisProcess.comSize; i++) {
			MPI_Send(&globalGrid[0], DIM*DIM, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	} else {
		//Compute next generation chunk.
		processChunk(newData, newDataArrSize);

		//Send the updated chunk to the master.
		MPI_Send(newData, newDataArrSize, MPI_INT, 0, 0, MPI_COMM_WORLD);

		//Wait for updated grid from master.
		MPI_Recv(&globalGrid[0], DIM*DIM, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	}
}

/**
 * Fills the newData array of the next generation
 * for this processes assigned chunk.
 * @param *newData
 * 				the pointer array to fill with new data.
 * @param arrSize
 * 				the alloted memory size for newData
 *
 */
void processChunk(int *newData, int arrSize) {
	int startIndex = thisProcess.rank * arrSize;
	int endIndex = startIndex + arrSize;
	int i;
	int count = 0;

	//Go through each cell in this process'
	//chunk and determine if it will be alive
	//in the next generation
	for(i = startIndex; i < endIndex; i++) {
		newData[count] = aliveNext(i);
		count++;
	}
}

/**
 * Determine if a cell will be alive (1)
 * in the next generation.
 *
 * @param index
 * 				the index of the cell to check.
 * @return 1 if the cell will be alive in the next
 * 			 generation, 0 otherwise.
 */
int aliveNext(int index) {
	int neighbors = numNeighbors(index);
	int currentState = globalGrid[index];

	//If we are currently alive, we must have
	//2 or 3 neighbors to continue living.
	if(currentState == 1) {
		if(neighbors == 2 || neighbors == 3) {
			return 1;
		} else {
			return 0;
		}
	} else {
		//Unoccupied space must have exactly
		//3 neighbors to turn on (be alive).
		if(neighbors == 3) {
			return 1;
		}
	}
	//Conditions are not right for
	//a new cell if we reach here.
	return 0;
}

/**
 * Get the number of neighbors for
 * the cell at index.
 *
 * @param index
 * 				the index of the cell to check.
 * @return the number of neighbors.
 */
int numNeighbors(int index) {
	int i,j;
	//We'll work in (x,y) coordinates for simplicity.
	int indexX = index / DIM;
	int indexY = index % DIM;
	int x = indexX;
	int y = indexY;

	int neighbors = 0;

	//Check all indexes surrounding index.
	//We take into account wrapping of the
	//array in the case that we go out of bounds.
	for(i = -1; i <= 1; i++) {
		if(x + i < 0) {
			x = DIM - 1;
		} else if(x + i >= DIM) {
			x = 0;
		} else {
			x += i;
		}
		for(j = -1; j <= 1; j++) {

			//cell can't count itself as a neighbor.
			if(i == 0 && j == 0) {
				continue;
			}

			if(y + j < 0) {
				y = DIM - 1;
			} else if(y + j >= DIM) {
				y = 0;
			} else {
				y += j;
			}
			neighbors += globalGrid[DIM*x + y];

			//Reset y to original form
			y = indexY;
		}
		//Reset x to original form
		x = indexX;
	}
	return neighbors;
}


/**
 * Debugging print statement used
 * throughout development.
 */
void debugPrint(int *arr, int size) {
	int i;
	for(i = 0; i < size; i++) {
		if(i % DIM == 0) {
			printf("\n");
		}
		printf("%d", arr[i]);
	}
	printf("\n\n");
}

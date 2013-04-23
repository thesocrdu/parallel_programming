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

	MPI_Status stat;
	int i, j;

	/* Simulation variables */
	/* Local data */
	int numEls;    //in local array
	int offset;     //local-> global
	int *lower;     //row from lower neighbor
	int *upper;     //row from upper neighbor
	int *grid, *tmpGrid;
	int gridSize;
	int rowsPerProcess;

	//Initialize MPI.
	MPI_Init(&argc, &argv);
	//Fill the process data
	MPI_Comm_size(MPI_COMM_WORLD, &(thisProcess.comSize));
	MPI_Comm_rank(MPI_COMM_WORLD, &(thisProcess.rank));
	thisProcess.prev = thisProcess.rank - 1;
	if(thisProcess.prev < 0) {
		thisProcess.prev = thisProcess.comSize - 1;
	}
	thisProcess.next = (thisProcess.rank + 1) % thisProcess.comSize;
	gridSize = (DIM * DIM) / thisProcess.comSize;

	lower = malloc(sizeof(int) * DIM);
	upper = malloc(sizeof(int) * DIM);
	grid = malloc(sizeof(int) * gridSize);
	tmpGrid = malloc(sizeof(int) * gridSize);

	numEls = (DIM*DIM) / thisProcess.comSize;
	offset = thisProcess.rank * numEls;
	rowsPerProcess = DIM / thisProcess.comSize;

	/* Fill local grid with data from global. */
	for (i = 0; i < numEls; i++) {
		grid[i] = globalGrid[i + offset];
	}

//	printf("Initial grid for rank %d:\n---------------------", thisProcess.rank);
//	printLocalGrid(grid, gridSize);
	if(thisProcess.rank == 0) {
		printf("Initial grid:\n----------------\n");
		printGrid();
	}


	for(i = 1; i <= NUM_CYCLES; i++) {
		/* Begin calculating next generation */

		/* Stagger sends and receives for edge rows */
		if(thisProcess.rank % 2 == 0 ) {
			MPI_Send(&grid[DIM * (rowsPerProcess - 1)], DIM, MPI_INT, thisProcess.next, 2, MPI_COMM_WORLD);
			MPI_Send(&grid[0], DIM, MPI_INT, thisProcess.prev, 2, MPI_COMM_WORLD );

//			printf("Rank %d sent rank %d row:\n", thisProcess.rank, thisProcess.prev);
//			printLocalGrid(&grid[0], DIM);

			MPI_Recv(upper, DIM, MPI_INT, thisProcess.prev, 2, MPI_COMM_WORLD, &stat );
			MPI_Recv(lower, DIM, MPI_INT, thisProcess.next, 2, MPI_COMM_WORLD, &stat );

//			printf("Rank %d got lower row to be:\n", thisProcess.rank);
//			printLocalGrid(lower, DIM);

		} else {
			MPI_Recv(upper, DIM, MPI_INT, thisProcess.prev, 2, MPI_COMM_WORLD, &stat);
			MPI_Recv(lower, DIM, MPI_INT, thisProcess.next, 2, MPI_COMM_WORLD, &stat);

//			printf("Rank %d got upper row to be:\n", thisProcess.rank);
//			printLocalGrid(upper, DIM);

			MPI_Send(&grid[DIM * (rowsPerProcess - 1)], DIM, MPI_INT, thisProcess.next, 2, MPI_COMM_WORLD);
			MPI_Send(&grid[0], DIM, MPI_INT, thisProcess.prev, 2, MPI_COMM_WORLD );

//			printf("Rank %d sent rank %d row:\n", thisProcess.rank, thisProcess.prev);
//			printLocalGrid(&grid[0], DIM);
		}

		/* Update values to tempGrid */

		/* Initial special case. */
//		printf("Rank: %d\n", thisProcess.rank);
//		printLocalGrid(&grid[DIM*2], DIM);
		if(thisProcess.comSize == 16) {
			fillRow(&grid[0], upper, lower, 0, &tmpGrid[0]);
		} else {
			fillRow(&grid[0], upper, &grid[DIM * 1], 0, &tmpGrid[0]);
			/* For each row in grid */
			for(j = 1; j < rowsPerProcess - 1; j++) {
				fillRow(&grid[DIM * j], &grid[DIM * (j-1)], &grid[DIM * (j+1)], j, &tmpGrid[DIM * j]);
			}
			/* Final special case. */
			fillRow(&grid[DIM * (rowsPerProcess - 1)], &grid[DIM * (rowsPerProcess - 2)], lower, rowsPerProcess - 1, &tmpGrid[DIM * (rowsPerProcess - 1)]);
		}
		//printLocalGrid(&grid[0], DIM);

		//Copy temp grid over to grid
		memcpy(&grid[0], &tmpGrid[0], sizeof(int) * gridSize);

		//printLocalGrid(grid, gridSize);

		//send grid to master? / receive grid from workers?
		if(thisProcess.rank == 0) {
			/* First copy master's work to the global grid. */
			memcpy(&globalGrid[0], &grid[0], sizeof(int) * gridSize);
			for(j = 1; j < thisProcess.comSize; j++) {
				MPI_Recv(&globalGrid[gridSize * j], gridSize, MPI_INT, j, 3, MPI_COMM_WORLD, &stat);
			}

			/* Print updated globalGrid */
			printf("Iteration %d\n---------------\n", i);
			printGrid();

		} else {
			MPI_Send(grid, gridSize, MPI_INT, 0, 3, MPI_COMM_WORLD);
		}
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
	printf("\n");
}

/**
 * Prints local grid.
 */
void printLocalGrid(int *arr, int size) {
	int i;
	for(i = 0; i < size; i++) {
		if(i % DIM == 0) {
			printf("\n");
		}
		printf("%d ", arr[i]);
	}
	printf("\n\n");
}


/**
 * Takes *currRow with current generation and fills the corresponding
 * rowNum in *temp using info from *upperRow and *lowerRow.
 */
void fillRow(int *currRow, int *upperRow, int *lowerRow, int rowNum, int *temp) {
	int i;
	//int numNeighbors;

	//numNeighbors = aliveInCol(DIM - 1) + );
	temp[0] = aliveNext(currRow, 0, upperRow, lowerRow, DIM - 1, 1);
	/* Compute every new cell in the row. */
	for(i = 1; i < DIM - 1; i++) {
		temp[i] = aliveNext(currRow, i, upperRow, lowerRow, i - 1, i + 1);
	}
	temp[DIM - 1] = aliveNext(currRow, DIM - 1, upperRow, lowerRow, DIM - 2, 0);
}

/**
 * Determines is a the cell at currRow[col] will be alive in the next generation.
 */
int aliveNext(int *currRow, int col, int *upperRow, int *lowerRow, int leftCol, int rightCol) {
	int neighbors;
	int currState = currRow[col];

	neighbors = upperRow[leftCol] + upperRow[col] + upperRow[rightCol]
	          + currRow[leftCol]         +          currRow[rightCol]
	          + lowerRow[leftCol] + lowerRow[col] + lowerRow[rightCol];

	//If we are currently alive, we must have
	//2 or 3 neighbors to continue living.
	if(currState == 1) {
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

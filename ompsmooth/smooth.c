/*******************************************************************************
 *
 *  Driver routine to operate a kernal smoother.
 *
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ompsmooth.h"
#include "omp.h"

#define MAT_DIM 2000
#define MAT_SIZE MAT_DIM*MAT_DIM

#define KERNEL_HALFWIDTH 2


int main()
{
	/* Variables for timing */
	struct timeval ta, tb;

	int i;
	float YXTot, XYTot = 0.0;
	float time = 0.0;
	printf("SerialYX\tSerialXY\n");
	float * m1in;
	float * m2out;
	int s,u;
	for ( i = 0; i < 0; i++) {
		/* Create two input matrixes */
		//  float * m1in;
		//  float * m2out;
		m1in = malloc ( sizeof(float)*MAT_SIZE );
		m2out = malloc ( sizeof(float)*MAT_SIZE );


		/*  random data for the input */
		// int fd = open ("/dev/urandom", O_RDONLY );
		//  read ( fd, m1in, MAT_SIZE*sizeof(float));
		/* instead, this pattern is preserved by the kernel smoother */
		int64_t x, y;
		for (y=0; y<MAT_DIM; y++) {
			for (x=0; x<MAT_DIM; x++) {
				m1in[y*MAT_DIM+x] = (float)(x+y);
			}
		}

		/* zero the output */
		memset ( m2out, 0, MAT_SIZE*sizeof(float) );


		/*********  Serial Test **********/


		/* run once to warm up the cache before measuring */
		smoothSerialYX ( MAT_DIM, KERNEL_HALFWIDTH, m1in, m2out );

		/* get initial time */
		gettimeofday ( &ta, NULL );

		smoothSerialYX ( MAT_DIM, KERNEL_HALFWIDTH, m1in, m2out );

		/* get initial time */
		gettimeofday ( &tb, NULL );

		/* Work out the time */
		s = tb.tv_sec - ta.tv_sec;
		u;
		if ( ta.tv_usec <= tb.tv_usec ) {
			u = tb.tv_usec - ta.tv_usec;
		} else {
			u = 1000000 - ta.tv_usec + tb.tv_usec;
			s = s-1;
		}

		time = (float)s + (float)u/1000000.0;
		YXTot += time;
		printf("%f\t", time);
		//printf ("Serial YX smoother took %d seconds and %d microseconds\n",s,u );

		/* get initial time */
		gettimeofday ( &ta, NULL );

		smoothSerialXY ( MAT_DIM, KERNEL_HALFWIDTH, m1in, m2out );

		/* get initial time */
		gettimeofday ( &tb, NULL );

		/* Work out the time */
		s = tb.tv_sec - ta.tv_sec;
		u;
		if ( ta.tv_usec <= tb.tv_usec ) {
			u = tb.tv_usec - ta.tv_usec;
		} else {
			u = 1000000 - ta.tv_usec + tb.tv_usec;
			s = s-1;
		}

		time = (float)s + (float)u/1000000.0;
		XYTot += time;
		time = (float)s + (float)u/1000000.0;
		printf("%f\n", time);
		//printf ("Serial XY smoother took %d seconds and %d microseconds\n",s,u );
		free(m1in);
		free(m2out);
	}
	printf("---------------------------------AVG\n%f\t%f\n\n\n",YXTot/(float)i, XYTot/(float)i);


	m1in = malloc ( sizeof(float)*MAT_SIZE );
	m2out = malloc ( sizeof(float)*MAT_SIZE );


	/*  random data for the input */
	// int fd = open ("/dev/urandom", O_RDONLY );
	//  read ( fd, m1in, MAT_SIZE*sizeof(float));
	/* instead, this pattern is preserved by the kernel smoother */
	int64_t x, y;
	for (y=0; y<MAT_DIM; y++) {
		for (x=0; x<MAT_DIM; x++) {
			m1in[y*MAT_DIM+x] = (float)(x+y);
		}
	}
	/* zero the output */
	memset ( m2out, 0, MAT_SIZE*sizeof(float) );


	/*********  Parallel Tests **********/
	printf("PARALLEL TESTS\n--------------------\n");
	int threads, iter;
	int count = 0;
	float avgs[6][3];
	printf("\tThds\tParallelYX\tParallelXY\tParaCoale\n");

	for (threads=1; threads <=0; threads*=2) {
		float totYX = 0.0, totXY = 0.0, totCoale = 0.0;
		omp_set_num_threads(threads);

		for(iter = 0; iter < 1; iter++) {
			printf("Run: %d\n", iter + 1);

			printf("\t%d\t", threads);

			/* get initial time */
			gettimeofday ( &ta, NULL );

			smoothParallelYXFor ( MAT_DIM, KERNEL_HALFWIDTH, m1in, m2out );

			/* get initial time */
			gettimeofday ( &tb, NULL );

			/* Work out the time */
			s = tb.tv_sec - ta.tv_sec;
			if ( ta.tv_usec < tb.tv_usec ) {
				u = tb.tv_usec - ta.tv_usec;
			} else {
				u = 1000000 - ta.tv_usec + tb.tv_usec;
				s = s-1;
			}
			time = (float)s + (float)u/1000000.0;
			totYX += time;
			printf("%f\t", time);
			//printf ("Parallel YX smoother took %d seconds and %d microseconds\n",s,u );

			/* get initial time */
			gettimeofday ( &ta, NULL );

			smoothParallelXYFor ( MAT_DIM, KERNEL_HALFWIDTH, m1in, m2out );

			/* get initial time */
			gettimeofday ( &tb, NULL );

			/* Work out the time */
			s = tb.tv_sec - ta.tv_sec;
			if ( ta.tv_usec < tb.tv_usec ) {
				u = tb.tv_usec - ta.tv_usec;
			} else {
				u = 1000000 - ta.tv_usec + tb.tv_usec;
				s = s-1;
			}
			time = (float)s + (float)u/1000000.0;
			totXY += time;
			printf("%f\t", time);
			//    printf ("Parallel XY smoother took %d seconds and %d microseconds\n",s,u );


			/* get initial time */
			gettimeofday ( &ta, NULL );

			smoothParallelCoalescedFor ( MAT_DIM, KERNEL_HALFWIDTH, m1in, m2out );

			/* get initial time */
			gettimeofday ( &tb, NULL );

			/* Work out the time */
			s = tb.tv_sec - ta.tv_sec;
			if ( ta.tv_usec < tb.tv_usec ) {
				u = tb.tv_usec - ta.tv_usec;
			} else {
				u = 1000000 - ta.tv_usec + tb.tv_usec;
				s = s-1;
			}
			time = (float)s + (float)u/1000000.0;
			totCoale += time;
			printf("%f\n", time);
			//    printf ("Parallel coalesced smoother took %d seconds and %d microseconds\n",s,u );
		}
		printf("-------------------------------------------------AVG\n\t\t%f\t%f\t%f\n\n", totYX/(float)iter, totXY/(float)iter, totCoale/(float)iter);
		avgs[count][0] = totYX/(float)iter;
		avgs[count][1] = totXY/(float)iter;
		avgs[count][2] = totCoale/(float)iter;
		count++;
	}

	/* Print parallel avg matrix */
	int row,col;
	printf("Thds\tParallelYX\tParallelXY\tParaCoale\n");
	for(row = 0; row < 6; row++) {
		printf("2^%d\t", row);
		for(col = 0; col < 3; col++) {
			printf("%f\t", avgs[row][col]);
		}
		printf("\n");
	}




	/* Start part 5 */
	/* Reset m2 and m3 */
	float * m3out = malloc ( sizeof(float)*MAT_SIZE );
	memset ( m2out, 0, MAT_SIZE*sizeof(float) );
	memset ( m3out, 0, MAT_SIZE*sizeof(float) );

	printf("\n\nStarting part 5...\n");
	printf("Thds\tProgram1\tProgram2\n");
	for (threads = 1; threads <=8; threads*=2) {

		float prog1Time = 0.0;
		float prog2Time = 0.0;
		omp_set_num_threads(threads);
		 for (iter = 0; iter < 15; iter++) {
			printf("Run %d\n", iter);

			/* Program 1 */
			printf("%d\t", threads);
			/* get initial time */
			gettimeofday ( &ta, NULL );

			smoothParallelYXFor( MAT_DIM, KERNEL_HALFWIDTH, m1in, m2out );
			smoothParallelYXFor( MAT_DIM, KERNEL_HALFWIDTH, m1in, m3out );

			/* get final time */
			gettimeofday ( &tb, NULL );
			/* Work out the time */
			s = tb.tv_sec - ta.tv_sec;
			if ( ta.tv_usec < tb.tv_usec ) {
				u = tb.tv_usec - ta.tv_usec;
			} else {
				u = 1000000 - ta.tv_usec + tb.tv_usec;
				s = s-1;
			}
			time = (float)s + (float)u/1000000.0;
			prog1Time += time;
			printf("%f\t", time);

			memset ( m2out, 0, MAT_SIZE*sizeof(float) );
			memset ( m3out, 0, MAT_SIZE*sizeof(float) );

			/* Program 2 */
			gettimeofday ( &ta, NULL );

			smoothParallelDoubleCoalescedFor (MAT_DIM, KERNEL_HALFWIDTH, m1in, m2out, m3out);

			gettimeofday ( &tb, NULL );
			/* Work out the time */
			s = tb.tv_sec - ta.tv_sec;
			if ( ta.tv_usec < tb.tv_usec ) {
				u = tb.tv_usec - ta.tv_usec;
			} else {
				u = 1000000 - ta.tv_usec + tb.tv_usec;
				s = s-1;
			}
			time = 0.0;
			time = (float)s + (float)u/1000000.0;
			prog2Time += time;
			printf("%f\n", time);
		}
		 printf("----------------------------------------AVG\n\t%f\t%f\n", prog1Time/iter, prog2Time/iter);
	}
}

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

#define MAT_DIM 6000
#define MAT_SIZE MAT_DIM*MAT_DIM

#define KERNEL_HALFWIDTH 2


int main()
{
  /* Variables for timing */
  struct timeval ta, tb;

  int i;
  float YXTot, XYTot = 0.0;
  printf("SerialYX\tSerialXY\n");
  float * m1in;
  float * m2out;
  int s,u;
  for ( i = 0; i < 20; i++) {
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

  float time = (float)s + (float)u/1000000.0;
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
  int threads;
  printf("Thds\tParallelYX\tParallelXY\tParaCoale\n");
  for (threads=1; threads <=32; threads*=2)
  {
    omp_set_num_threads(threads);
    printf("%d\t", threads);

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
    float time = (float)s + (float)u/1000000.0;
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
    printf("%f\n", time);
//    printf ("Parallel coalesced smoother took %d seconds and %d microseconds\n",s,u );
  }
}


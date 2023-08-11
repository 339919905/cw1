/***********************************/
/* XJCO 1921M Programming Project  */
/* 2022-23 Spring Semester         */
/*           			   */
/*                                 */
/* Version 0.2: Zheng Wang         */
/* Version 0.1: Hamish Carr        */
/***********************************/

/***********************************/
/* A first iteration to take a pgm */
/* file in ASCII and echo the data */
/***********************************/

/***********************************/
/* Main Routine                    */
/***********************************/

/* library for I/O routines        */
#include <stdio.h>

/* library for memory routines     */
#include <stdlib.h>
#include <math.h>
#include "pgmBase.h"

/***********************************/
/* main routine                    */
/*                                 */
/* CLI parameters:                 */
/* argv[0]: executable name        */
/* argv[1]: input file name        */
/* argv[2]: output file name       */
/* returns 0 on success            */
/* non-zero error code on fail     */
/***********************************/
int main(int argc, char **argv)
{ /* main() */
	if (argc == 1)	
	{ /* wrong arg count */
		/* print an error message        */
		printf("Usage: %s inputImage.pgm reduction_factor outputImage.pgm\n", argv[0]);
		/* and return an error code      */
		return EXIT_NO_ERRORS;
	} /* wrong arg count */

	/* check for correct number of arguments */
	if (argc != 4)
	{ /* wrong arg count */
		/* print an error message        */
		printf("ERROR: Bad Argument Count\n");
		/* and return an error code      */
		return EXIT_WRONG_ARG_COUNT;
	} /* wrong arg count */


    PgmFile pgm, reducepgm;
    int ret;

    if ((ret = read_and_check_pgm(&pgm, argv[1])) != EXIT_NO_ERRORS)
    {
        return ret;
    }

    int reduceFactor = atoi(argv[2]);

    reducepgm.width = 1 + (pgm.width - 1)/ reduceFactor;
    reducepgm.height = 1 + (pgm.height - 1)/ reduceFactor;
    reducepgm.maxGray = pgm.maxGray;
    reducepgm.isbinarypgm = pgm.isbinarypgm;
    reducepgm.commentLine = NULL;
    int bytes = reducepgm.width * reducepgm.height * sizeof(unsigned char);
    reducepgm.imageData = (unsigned char *) malloc(bytes);

    for (int i = 0; i < reducepgm.width; ++i)
    {
        for (int j = 0; j < reducepgm.height; ++j){
            reducepgm.imageData[i + j * reducepgm.width] = pgm.imageData[i * reduceFactor + j * reduceFactor * pgm.width];
            //printf("get %d at point(%d, %d)\n", pgm.imageData[i * reduceFactor + j * reduceFactor * pgm.height], i * reduceFactor, j * reduceFactor);
        }
    }

    if ((ret = output_pgm(&reducepgm, argv[3], pgm.isbinarypgm)) != EXIT_NO_ERRORS)
        return ret;

	/* at this point, we are done and can exit with a success code */
    printf("REDUCED\n");

	return EXIT_NO_ERRORS;
} /* main() */

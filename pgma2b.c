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
/* file in ASCII and echo BINARY   */
/***********************************/

/***********************************/
/* Main Routine                    */
/***********************************/

/* library for I/O routines        */
#include <stdio.h>

/* library for memory routines     */
#include <stdlib.h>
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
		printf("Usage: %s inputImage.pgm outputImage.pgm\n", argv[0]);
		/* and return an error code      */
		return EXIT_NO_ERRORS;
	} /* wrong arg count */

	/* check for correct number of arguments */
	if (argc != 3)	
	{ /* wrong arg count */
		/* print an error message        */
		printf("ERROR: Bad Argument Count\n");
		/* and return an error code      */
		return EXIT_WRONG_ARG_COUNT;
	} /* wrong arg count */

    PgmFile pgm;
    int ret;

    if ((ret = read_and_check_pgm(&pgm, argv[1])) != EXIT_NO_ERRORS)
    {
        return ret;
    }

    if (pgm.isbinarypgm)
	{ /* failed magic number check   */
		/* be tidy: close the file       */
        free(pgm.commentLine);
        free(pgm.imageData);

		/* print an error message */
		printf("ERROR: Bad Magic Number (%s)\n", argv[1]);	

		/* and return                    */
		return EXIT_INVALID_MAGIC_NUMBER;
	} /* failed magic number check   */

    if ((ret = output_pgm(&pgm, argv[2], 1)) != EXIT_NO_ERRORS)
    {
        return ret;
    }

	/* at this point, we are done and can exit with a success code */
    printf("CONVERTED\n");

	return EXIT_NO_ERRORS;
} /* main() */

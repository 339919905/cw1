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
#include "pgmBase.h"

#define MAGIC_NUMBER_RAW_PGM 0x3550
#define MAGIC_NUMBER_ASCII_PGM 0x3250
#define MIN_IMAGE_DIMENSION 1
#define MAX_IMAGE_DIMENSION 65536
#define MAX_COMMENT_LINE_LENGTH 128

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
		printf("ERROR: Bad Arguments\n");
		/* and return an error code      */
		return EXIT_WRONG_ARG_COUNT;
	} /* wrong arg count */


    PgmFile pgm;
    int ret;

    if ((ret = read_and_check_pgm(&pgm, argv[1])) != EXIT_NO_ERRORS)
    {
        return ret;
    }

    if ((ret = output_pgm(&pgm, argv[2], pgm.isbinarypgm)) != EXIT_NO_ERRORS)
    {
        return ret;
    }

	/* at this point, we are done and can exit with a success code */
    printf("ECHOED\n");

	return EXIT_NO_ERRORS;
} /* main() */

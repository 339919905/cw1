/***********************************/
/* XJCO 1921M Programming Project  */
/* 2022-23 Spring Semester         */
/*           			   */
/*                                 */
/* Version 0.2: Zheng Wang         */
/* Version 0.1: Hamish Carr        */
/***********************************/

/***********************************/
/* A first iteration to take two   */
/* pgm file to compare             */
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
		printf("Usage: %s inputImage.pgm inputImage.pgm\n", argv[0]);
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

    PgmFile pgm1, pgm2;
    int ret;

    if ((ret = read_and_check_pgm(&pgm1, argv[1])) != EXIT_NO_ERRORS)
    {
        return ret;
    }

    if ((ret = read_and_check_pgm(&pgm2, argv[2])) != EXIT_NO_ERRORS)
    {
        return ret;
    }

    if (pgm1.width != pgm2.width || pgm1.height != pgm2.height || pgm1.maxGray != pgm2.maxGray)
    {
        printf("DIFFERENT\n");
        return EXIT_NO_ERRORS;
    }

    int len = pgm1.width * pgm1.height;
    int issame = 1;

    for (int i = 0; i < len; ++i)
    {
        if (pgm1.imageData[i] != pgm2.imageData[i])
        {
            issame = 0;
            break;
        }
    }

	printf(issame ? "IDENTICAL\n": "DIFFERENT\n");

	return EXIT_NO_ERRORS;
}


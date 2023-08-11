#include <stdio.h>
#include <stdlib.h>
#include "pgmBase.h"


int read_and_check_pgm(PgmFile *pgm, char *filename)
{
	/* variables for storing the image       */
	/* this is NOT good modularisation       */
	/* and you will eventually replace it    */
	/* for now, leave it here                */

	int isbinarypgm;

	/* we will store ONE comment	         */
	char *commentLine = NULL;

	/* the logical width & height	         */
	/* note: cannot be negative	         */
	unsigned int width = 0, height = 0;

	/* maximum gray value (assumed)	         */
	/* make it an integer for ease	         */
	unsigned int maxGray = 255;

	/* pointer to raw image data	         */
	unsigned char *imageData = NULL;

	/* now start reading in the data         */
	/* try to open the file for text I/O     */
	/* in ASCII mode b/c the header is text  */
	FILE *inputFile = fopen(filename, "r");

	/* if it fails, return error code        */
	if (inputFile == NULL)
    {
        printf("ERROR: Bad File Name (%s)\n", filename);
		return EXIT_BAD_INPUT_FILE;
    }

	/* sanity check on the magic number      */
	if (read_and_check_magic_number(inputFile, &isbinarypgm) != EXIT_NO_ERRORS)
	{ /* failed magic number check   */
		/* be tidy: close the file       */
		fclose(inputFile);

		/* print an error message */
		printf("ERROR: Bad Magic Number (%s)\n", filename);	

		/* and return                    */
		return EXIT_INVALID_MAGIC_NUMBER;
	} /* failed magic number check   */

	/* scan whitespace if present            */
	fscanf(inputFile, " ");

	/* check for a comment line              */
	if (read_and_check_comment(inputFile, commentLine) != EXIT_NO_ERRORS)
	{
		/* free memory           */
		free(commentLine);
		/* close file            */
		fclose(inputFile);

		/* print an error message */
		printf("ERROR: Bad Comment Line (%s)\n", filename);	

		/* and return            */
		return EXIT_INVALID_COMMENT_LINE;
	}

	if (read_and_check_pgm_info(inputFile, &width, &height, &maxGray) != EXIT_NO_ERRORS)
	{ /* failed size sanity check    */
		/* free up the memory            */
		free(commentLine);

		/* be tidy: close file pointer   */
		fclose(inputFile);

		/* print an error message */
		printf("ERROR: Bad Dimensions (%s)\n", filename);	

		/* and return                    */
		return EXIT_INVALID_DIMENSIONS;
	}

	/* allocate the data pointer             */
	long nImageBytes = width * height * sizeof(unsigned char);
	imageData = (unsigned char *) malloc(nImageBytes);

	/* sanity check for memory allocation    */
	if (imageData == NULL)
	{ /* malloc failed */
		/* free up memory                */
		free(commentLine);

		/* close file pointer            */
		fclose(inputFile);

		/* print an error message */
		printf("Error: Malloc failed to allocate memory\n");	

		/* return error code             */
		return EXIT_MALLOC_FAILED;
	} /* malloc failed */

	/* sanity check	                 */
	if (read_and_check_pgm_data(inputFile, nImageBytes, maxGray, isbinarypgm, imageData))
	{ /* fscanf failed */
		/* free memory           */
		free(commentLine);
		free(imageData);	

		/* close file            */
		fclose(inputFile);

		/* print error message   */
		printf("Error: Bad Max Gray Value (%s)\n", filename);	

		/* and return            */
		return EXIT_INVALID_MAX_GRAY_VALUE;
	} 

	/* we're done with the file, so close it */
	fclose(inputFile);

    pgm->maxGray = maxGray;
    pgm->imageData = imageData;
    pgm->width = width;
    pgm->height = height;
    pgm->commentLine = commentLine;
    pgm->isbinarypgm = isbinarypgm;

    return EXIT_NO_ERRORS;
}


int output_pgm(PgmFile *pgm, char *filename, int isoutputbinary)
{
	/* open a file for writing               */
	FILE *outputFile = fopen(filename, "w");

	/* check whether file opening worked     */
	if (outputFile == NULL)
	{ /* NULL output file */
		/* print an error message        */
		printf("Error: Bad File Name (%s)\n", filename);	

		/* free memory                   */
        if (pgm->commentLine)
		    free(pgm->commentLine);

		free(pgm->imageData);

		/* return an error code          */
		return EXIT_BAD_INPUT_FILE;
	} /* NULL output file */

	/* write magic number, size & gray value */
	size_t nBytesWritten = fprintf(
        outputFile,
        "%s\n%d %d\n%d\n",
        isoutputbinary ? "P5": "P2", pgm->width, pgm->height, pgm->maxGray
    );

	/* check that dimensions wrote correctly */
	if (nBytesWritten < 0)
	{ /* dimensional write failed    */
		/* print an error message        */
		printf("Error: Output Failed (%s)\n", filename);	
		/* free memory                   */
        if (pgm->commentLine)
		    free(pgm->commentLine);

		free(pgm->imageData);

		/* return an error code          */
		return EXIT_OUTPUT_FAILED;
	} /* dimensional write failed    */

	long nBytes = pgm->width * pgm->height * sizeof(unsigned char);
    unsigned char *imageVal = pgm->imageData;

	/* pointer for efficient write code      */
	for (unsigned char *grayval = imageVal; grayval < imageVal + nBytes; grayval++)
	{ /* per gray value */
		/* get next char's column        */
        if (isoutputbinary)
        {
            nBytesWritten = fwrite(grayval, 1, 1, outputFile);
        }
        else
        {
            int nextCol = (grayval - imageVal + 1) % pgm->width;
            /* write the entry & whitespace  */
            nBytesWritten = fprintf(outputFile, "%d%c", *grayval, (nextCol ? ' ' : '\n') );
        }

		/* sanity check on write         */
		if (nBytesWritten < 0)
		{ /* data write failed   */
			/* free memory           */
            if (pgm->commentLine)
                free(pgm->commentLine);

			free(pgm->imageData);

			/* print error message   */
			printf("Error: Output Failed (%s)\n", filename);	

			/* return an error code  */
			return EXIT_OUTPUT_FAILED;
		} /* data write failed   */
	} /* per gray value */

    return EXIT_NO_ERRORS;
}


int read_and_check_magic_number(FILE* pgmfile, int *isbinarypgm)
{
	/* the magic number		         */
	/* stored as two bytes to avoid	         */
	/* problems with endianness	         */
	/* Raw:    0x5035 or P5		         */
	/* ASCII:  0x5032 or P2		         */
	unsigned char magic_number[2] = {'0','0'};
	unsigned short *magic_Number = (unsigned short *) magic_number;
	/* read in the magic number              */
	magic_number[0] = getc(pgmfile);
	magic_number[1] = getc(pgmfile);

    *isbinarypgm = (*magic_Number == MAGIC_NUMBER_RAW_PGM);

	/* sanity check on the magic number      */
	if (!*isbinarypgm && *magic_Number != MAGIC_NUMBER_ASCII_PGM)
	{ 
		return EXIT_INVALID_MAGIC_NUMBER;
	} /* failed magic number check   */
	return EXIT_NO_ERRORS;
}

int read_and_check_comment(FILE* pgmFile, char *commentLine)
{
	/* check for a comment line              */
	char nextChar = fgetc(pgmFile);
	if (nextChar == '#')
	{ /* comment line                */
		/* allocate buffer               */
		commentLine = (char *) malloc(MAX_COMMENT_LINE_LENGTH);
		/* fgets() reads a line          */
		/* capture return value          */
		char *commentString = fgets(commentLine, MAX_COMMENT_LINE_LENGTH, pgmFile);
		/* NULL means failure            */
		if (commentString == NULL)
		{
			return EXIT_INVALID_COMMENT_LINE;
		} /* NULL comment read   */
	} /* comment line */
	else
	{ /* not a comment line */
		/* put character back            */
		ungetc(nextChar, pgmFile);
	} /* not a comment line */
	
    return EXIT_NO_ERRORS;
}

int read_and_check_pgm_info(FILE* pgmFile, unsigned int *width, unsigned int *height, unsigned int *maxGray)
{
	/* read in width, height, grays          */
	/* whitespace to skip blanks             */
	int scanCount = fscanf(pgmFile, " %u %u %u", width, height, maxGray);

	/* sanity checks on size & grays         */
	/* must read exactly three values        */
	if 	(
			(scanCount != 3				)	||
			(*width 	< (unsigned int)MIN_IMAGE_DIMENSION	) 	||
			(*width 	> (unsigned int)MAX_IMAGE_DIMENSION	) 	||
			(*height < (unsigned int)MIN_IMAGE_DIMENSION	) 	||
			(*height > (unsigned int)MAX_IMAGE_DIMENSION	) 	||
			(*maxGray	!= 255		)
		)
	{ /* failed size sanity check    */
		return EXIT_INVALID_DIMENSIONS;
	} /* failed size sanity check    */
	
    return EXIT_NO_ERRORS;
}

int read_and_check_pgm_data(FILE* pgmFile, long nImageBytes, unsigned int maxGray, int isbinarypgm, unsigned char *imageData)
{
	if (isbinarypgm)
    {
        char tmp;
        fscanf(pgmFile, "%c", &tmp);
    }

	/* pointer for efficient read code       */
	for (unsigned char *grayval = imageData; grayval < imageData + nImageBytes; grayval++)
	{ /* per gray value */
		/* read next value               */
		int grayValue = -1;
		int scanCount = 0;

        if (isbinarypgm)
        {
            grayValue = 0;
            scanCount = fscanf(pgmFile, "%c", (char *)(&grayValue));
        }
        else
        {
            scanCount = fscanf(pgmFile, " %u", &grayValue);
        }

		/* sanity check	                 */
		if ((scanCount != 1) || (grayValue < 0) || (grayValue > maxGray))
		{
			return EXIT_INVALID_MAX_GRAY_VALUE;
		} /* fscanf failed */

		/* set the pixel value           */
		*grayval = (unsigned char) grayValue;
	} /* per gray value */
	
    return EXIT_NO_ERRORS;
}
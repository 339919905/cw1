#ifndef __PGM_BASE__
#define __PGM_BASE__

#define MAGIC_NUMBER_RAW_PGM 0x3550
#define MAGIC_NUMBER_ASCII_PGM 0x3250
#define MIN_IMAGE_DIMENSION 1
#define MAX_IMAGE_DIMENSION 65536
#define MAX_COMMENT_LINE_LENGTH 128

#define EXIT_NO_ERRORS 0
#define EXIT_WRONG_ARG_COUNT 1
#define EXIT_BAD_INPUT_FILE 2
#define EXIT_INVALID_MAGIC_NUMBER 3
#define EXIT_INVALID_COMMENT_LINE 4
#define EXIT_INVALID_DIMENSIONS 5
#define EXIT_INVALID_MAX_GRAY_VALUE 6
#define EXIT_MALLOC_FAILED 7
#define EXIT_INVALID_DATA 8
#define EXIT_OUTPUT_FAILED 9
#define EXIT_MISCELLANEOUS 100

typedef struct pgmfile
{
    int width;
    int height;
    int maxGray;
    int isbinarypgm;
    char *commentLine;
    unsigned char *imageData;
} PgmFile;

int read_and_check_pgm(PgmFile *pgm, char *filename);
int output_pgm(PgmFile *pgm, char *filename, int isoutputbinary);
int read_and_check_magic_number(FILE* pgmFile, int *isbinarypgm);
int read_and_check_comment(FILE* pgmFile, char *commentLine);
int read_and_check_pgm_info(FILE* pgmFile, unsigned int *width, unsigned int *height, unsigned int *maxGray);
int read_and_check_pgm_data(FILE* pgmFile, long nImageBytes, unsigned int maxGray, int isbinarypgm, unsigned char *imageData);

#endif 


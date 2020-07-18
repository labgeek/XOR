/*
 ##########################################################################################
 # Created on:  14 December 2010
 # File(s):     xor.c
 # Version:	    0.1 beta
 # Author:		JD Durick <labgeek@gmail.com>
 # Description: Simple program using XOR to encrypt files with a rolling key.
 ##########################################################################################
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <inttypes.h>
#include <time.h>
#include <limits.h>
#include <zlib.h>  /* will be used for gzip compression */

void usage(char *);
void xor(char *, char *, char *);
int getFileSize(char[]);

char *XORkey = NULL;
char *input, *output = NULL;

//TODO add compression before XORing

int main(int argc, char ** argv) {
	char *cmd = argv[0];
	char opt = 0;
	int encrypt, decrypt = 0;

	while (opt != -1) {
		opt = getopt(argc, argv, "hedi:o:k:");
		switch (opt) {
		case '?':
			usage(cmd);
			break;
		case 'i':
			input = optarg; /* input file*/
			break;
		case 'o':
			output = optarg; /* output file */
			break;
		case 'k':
			XORkey = optarg; /* key to be used during XOR operation */
			break;
		case 'h':
			usage(cmd); /* help command */
			break;
		case -1:
			break;
		default:
			usage(cmd);
			return (1);
		}
	}

	if (input && output && XORkey) {
		xor(input, output, XORkey);
	} else {
		usage(cmd);
		exit(0);
	}
	return 0;
}

/* Function name:  xor
 * Input:  input file, output file, and the key used to encrypt
 * or decrypt the file in question
 */
void xor(char *input_file, char *output_file, char *key) {
	FILE* input = fopen(input_file, "rb");
	FILE* output = fopen(output_file, "w");
	int keycount = 0;
	int encryptbyte;

	if (input == NULL) {
		printf("Input file %s cannot be read\n", input_file);
	}

	if (output == NULL) {
		printf("Output file %s cannot be read\n", output_file);
	}

	while ((encryptbyte = fgetc(input)) != EOF) {
		fputc(encryptbyte ^ key[keycount], output);
		keycount++;
		if (keycount == strlen(key)) {
			keycount = 0;
		}
	}
	fclose(input);
	fclose(output);
}

void usage(char *prog_name) {
	fprintf(stderr, "\nProgram: xor v0.1 beta\n");
	fprintf(stderr, "Author:  JD Durick <labgeek@gmail.com>\n");
	fprintf(
			stderr,
			"Description:  xor.c is a small program to encrypt/decrypt files via a rolling key\n");
	fprintf(stderr,
			"\nUsage: %s -i <input file to be encrypted or decrypted> -o <output file> -k <key used>\n"
				"\nOPTIONS:\n"
				"   -i File to be encrypted or decrypted via XOR key - REQUIRED\n"
				"   -o output [can be either an encrypted file or decrypted file]\n"
				"   -k key that is used to encrypt or decrypt the file(s)\n",
			prog_name);
}

/* Function to get the byte size of the file
 * Was used for testing purposes
 */
int getFileSize(char inFilename[]) {
	FILE *infile;
	int infilesize = 0;

	infile = fopen(inFilename, "rb");
	if (infile == NULL) {
		printf("Read Failed\n");
		exit(0);
	}
	fseek(infile, 0, SEEK_END);
	infilesize = ftell(infile);
	return (infilesize);
}

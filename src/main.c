/*
 	ACRG
	Generates cipher decoder rings for arbitrary alphabets in SVG.

    Copyright (C) 2016 Morgan Hill <morgan@pcwizzltd.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "acrg.h"

#define OUTER_R 400
#define MIDDLE_R 300
#define INNER_R 200

typedef struct config {
	double outer, middle, inner;
} Config;

void* wrap_malloc(size_t salloc);
char* get_input();
UTF8 process_input(char* input);

int main (int argc, char **argv) {
	
	char* raw_input = get_input();
	
	UTF8 utf8 = process_input(raw_input);

	free(raw_input);

	raw_input = NULL;

	DrawFull(utf8, OUTER_R, MIDDLE_R, INNER_R);

	free_utf8(utf8);	

	return 0;
}

//Deal with errors from malloc and clean the allocation
void* wrap_malloc(size_t salloc){

	void* alloc = malloc(salloc);

	if (alloc == NULL){
		fprintf(stderr, "Cannot allocate %d bytes memory at this timea\n", (unsigned int)salloc);
		exit(-1);
	}

	char *bitstring = (char*)alloc;
	
	for (int i = 0; i < salloc; i++)
		bitstring[i] = '\0';
	
	bitstring = NULL;

	return alloc;
}

char* get_input(){
	int c = 0;
	int nbytes = 0;
	char* raw_input = NULL;

	raw_input = (char*)wrap_malloc(nbytes + 1);

	for (;;) {

		if (nbytes == INT_MAX){//stop overflows
			fprintf(stderr, "Input to long\n");
			exit(-1);
		}

		c = getchar();

		if (c == EOF || c == '\n')
			break;

		nbytes++;

		char* tmp = (char*)realloc((void*)raw_input, nbytes+1);

		if (tmp == NULL){

			tmp = (char*)wrap_malloc(nbytes + 1);

			strcpy(tmp, raw_input);

			free(raw_input);

		} else {
			tmp[nbytes] = '\0';//clean the new end char
		}

		raw_input = tmp;
		tmp = NULL;
		raw_input[nbytes-1] = (char)c;

	}

	return raw_input;
}

//plits out codepoints into aray with onebyte of trailing \0 padding
UTF8 process_input(char* input){

	char** output = NULL;
	size_t ncpoints = 0;
	size_t pos = 0;

	char* next_codepoint = input;

	 while (*next_codepoint != '\0'){

	 	if (ncpoints == INT_MAX){
			fprintf(stderr, "Input too long; exiting to avoid overflow\n");
			exit(-1);
		}

	 	pos = ncpoints++;

	 	char** tmp = (char**)wrap_malloc((ncpoints) * sizeof(char*));

		if (output != NULL){

			memcpy((void*)tmp, (void*)output, (pos) * sizeof(char*));

			free(output);
		}

		output = tmp;

		tmp = NULL;

		//Find and copy the codepoints
		int codepoint_len = utf8_codepoint_len(*next_codepoint);
		output[pos] = (char*)wrap_malloc(codepoint_len +1);

		memcpy(output[pos], next_codepoint, codepoint_len);
		output[pos][codepoint_len] = '\0';

		next_codepoint += codepoint_len;

	}

	return (UTF8){str: output, len: ncpoints};
}

/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "writer.h"

FILE *outputStream;

int writeError(char *data, int lineNo, int colNo) {
	if(fprintf(outputStream, "%d-%d:%s\n", lineNo, colNo, data) <= 0)
	{
		return IO_ERROR;
	}
	return IO_SUCCESS;
}

int openOutputStreamForError(char *fileName, char *data, int lineNo, int colNo) {
	openOrCreateFile(fileName);

	if (outputStream == NULL){
		return IO_ERROR;
	}
	return writeError(data, lineNo, colNo);
}

int writeResult(char *data, int lineNo, int colNo, char *type) {
	if(strncmp(type,"TK",2) == 0){
		if(fprintf(outputStream, "%d-%d:%s(%s)\n", lineNo, colNo, type, data) <= 0){
			return IO_ERROR;
		}
		return IO_SUCCESS;
	}
	if(fprintf(outputStream, "%d-%d:%s%s\n", lineNo, colNo, type, data) <= 0){
		return IO_ERROR;
	}
	return IO_SUCCESS;
}

void openOrCreateFile(char *fileName){
	if( access( fileName, F_OK ) != -1 ) {
		// file exists
		outputStream = fopen(fileName, "a");
	} else {
		// file doesn't exist
		outputStream = fopen(fileName, "w");
	}
}

int openOutputStreamForResult(char *fileName, char *data, int lineNo, int colNo, char *type) {
	openOrCreateFile(fileName);

	if (outputStream == NULL){
		return IO_ERROR;
	}
	return writeResult(data, lineNo, colNo, type);
}

void closeOutputStream() {
	fclose(outputStream);
}
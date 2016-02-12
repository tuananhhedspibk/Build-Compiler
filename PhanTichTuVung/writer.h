/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __WRITER_H__
#define __WRITER_H__

#define IO_ERROR 0
#define IO_SUCCESS 1

void openOrCreateFile(char *fileName);
int writeError(char *data, int lineNo, int colNo);
int openOutputStreamForError(char *fileName, char *data, int lineNo, int colNo);
int writeResult(char *data, int lineNo, int colNo, char *type);
int openOutputStreamForResult(char *fileName, char *data, int lineNo, int colNo, char *type);
void closeOutputStream(void);

#endif
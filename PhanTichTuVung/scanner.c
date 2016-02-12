/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"

extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

char identifierTempor[100];

/***************************************************************/

void skipBlank() {										// --- Ham bo qua dau cach ---
	while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_SPACE)){
		readChar();
	}
}

void skipComment() {									// --- Ham bo qua comment ---
	if(charCodes[currentChar] == CHAR_TIMES){
		while(charCodes[currentChar] != CHAR_RPAR){
			readChar();
			if(currentChar == EOF){
				error(ERR_ENDOFCOMMENT, lineNo, colNo);
			}
		}
	}
	else{
		do{
			readChar();
		}while(currentChar != '\n');
	}
	readChar();
}

Token* readIdentKeyword(void) {							// --- Ham doc identifier ---
	Token *token;
	int i = 0;
	token = makeToken(TK_IDENT,lineNo,colNo);
	identifierTempor[0] = '\0';
	while(charCodes[currentChar] == CHAR_DIGIT || charCodes[currentChar] == CHAR_LETTER){
		if(i == 100){
			error(ERR_IDENTTOOLONG,lineNo,colNo);
			return NULL;
		}
		identifierTempor[i] = currentChar;
		token->string[i++] = currentChar;
		readChar();
	}
	identifierTempor[i] = '\0';
	token->string[i] = '\0';
	TokenType tokenType = checkKeyword(token->string);
	if(tokenType >= KW_PROGRAM && tokenType <= KW_UNTIL){
		token->tokenType = tokenType;
	}
	if(token->tokenType != TK_NONE && token->tokenType != TK_IDENT && token->tokenType != TK_NUMBER && token->tokenType != TK_CHAR && token->tokenType != TK_EOF && token->tokenType != TK_STRING){
		token->string[0] = '\0';
		identifierTempor[0] = '\0';
	}
	return token;
}

Token* readNumber(void) {
	Token *token;
	token = makeToken(TK_NUMBER, lineNo, colNo);
	token->value = 0;
	while(charCodes[currentChar] == CHAR_DIGIT){
		token->value = token->value * 10 + currentChar - '0';
		readChar();
	}
	return token;
}

Token* readConstChar() {
	Token *token;
	int i = 0, ln, cn;
	token = makeToken(TK_CHAR, lineNo, colNo);
	ln = lineNo; cn = colNo;
	readChar();
	token->string[i++] = currentChar;
	token->string[i] = '\0';
	if(charCodes[readChar()] != CHAR_SINGLEQUOTE){
		error(ERR_INVALIDCHARCONSTANT, ln, cn);
	}
	readChar();
	return token;
}

Token* readConstString(){
	Token *token;
	token = makeToken(TK_STRING, lineNo, colNo);
	int i = 0;
	while(charCodes[readChar()] != CHAR_DOUBLEQUOTE){
		token->string[i++] = currentChar;
	}
	token->string[i] = '\0';
	readChar();
	return token;
}

Token* getToken() {
	Token *token;
	int ln,cn;

	if (currentChar == EOF) {
		return makeToken(TK_EOF, lineNo, colNo);
	}

	switch (charCodes[currentChar]) {
		case CHAR_SPACE: { 
			skipBlank(); return getToken();
		}
		case CHAR_LETTER: { 
			return readIdentKeyword();
		}
		case CHAR_DIGIT: {
			return readNumber();
		}
		case CHAR_SEMICOLON: {
			token = makeToken(SB_SEMICOLON, lineNo, colNo);
			readChar();
			return token;
		}
		case CHAR_PLUS: {
			token = makeToken(SB_PLUS, lineNo, colNo);
			readChar(); 
			return token;
		}
		case CHAR_PERIOD: {
			token = makeToken(SB_PERIOD, lineNo, colNo);
			readChar(); 
			return token;
		}
		case CHAR_TIMES: {
			token = makeToken(SB_TIMES, lineNo, colNo);
			readChar();
			return token;
		}
		case CHAR_DOUBLEQUOTE:{
			if(strcmp(identifierTempor,"WRITEC") == 0){
				token = makeToken(TK_NONE, lineNo, colNo);
				error(ERR_INVALIDSYMBOL, lineNo, colNo);
				return token;
			}
			return readConstString();
		}
		case CHAR_MINUS: {
			token = makeToken(SB_MINUS, lineNo, colNo);
			readChar();
			return token;
		}
		case CHAR_RPAR: {
			token = makeToken(SB_RPAR, lineNo, colNo);
			readChar();
			return token;
		}
		case CHAR_LPAR: {
			token = makeToken(SB_LPAR, lineNo, colNo);
			if(charCodes[readChar()] == CHAR_TIMES){
				skipComment();
				free(token);
				return getToken();
			}
			return token;
		}
		case CHAR_EQ: {
			token = makeToken(SB_EQ, lineNo, colNo);
			readChar();
			return token;
		}
		case CHAR_EXCLAIMATION:{
			token = makeToken(TK_NONE, lineNo, colNo);
			ln = lineNo;
			cn = colNo;
			if(charCodes[readChar()] == CHAR_EQ){
				free(token);
				token = makeToken(SB_NEQ, ln, cn);
				readChar();
				return token;
			}
			error(ERR_INVALIDSYMBOL, ln, cn);
			return token;
		}
		case CHAR_SINGLEQUOTE:{
			return readConstChar();
		}
		case CHAR_SLASH:{
			token = makeToken(SB_SLASH, lineNo, colNo);
			if(charCodes[readChar()] == CHAR_SLASH){
				free(token);
				skipComment();
				return getToken();
			}
			else{
				return token;
			}
		}
		case CHAR_COMMA: {
			token = makeToken(SB_COMMA, lineNo, colNo);
			readChar();
			return token;
		}
		case CHAR_COLON: {
			token = makeToken(SB_COLON, lineNo, colNo);
			ln = lineNo;
			cn = colNo;
			if(charCodes[readChar()] == CHAR_EQ){
				free(token);
				token = makeToken(SB_ASSIGN, ln, cn);
				readChar();
			}
			return token;
		}
		default: {
			token = makeToken(TK_NONE, lineNo, colNo);
			error(ERR_INVALIDSYMBOL, lineNo, colNo);
			return token;
		}
	}
}


/******************************************************************/

void printToken(Token *token) {

	printf("%d-%d:", token->lineNo, token->colNo);

	switch (token->tokenType) {
		case TK_NONE: printf("TK_NONE\n"); break;
		case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
		case TK_NUMBER: printf("TK_NUMBER(%d)\n", token->value); break;
		case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
		case TK_STRING: printf("TK_STRING(\"%s\")\n",token->string); break;
		case TK_EOF: printf("TK_EOF\n"); break;

		case KW_REPEAT: printf("KW_REPEAT\n"); break;
		case KW_UNTIL: printf("KW_UNTIL\n"); break;
		case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
		case KW_STRING: printf("KW_STRING\n"); break;
		case KW_CONST: printf("KW_CONST\n"); break;
		case KW_TYPE: printf("KW_TYPE\n"); break;
		case KW_VAR: printf("KW_VAR\n"); break;
		case KW_INTEGER: printf("KW_INTEGER\n"); break;
		case KW_CHAR: printf("KW_CHAR\n"); break;
		case KW_ARRAY: printf("KW_ARRAY\n"); break;
		case KW_OF: printf("KW_OF\n"); break;
		case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
		case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
		case KW_BEGIN: printf("KW_BEGIN\n"); break;
		case KW_END: printf("KW_END\n"); break;
		case KW_CALL: printf("KW_CALL\n"); break;
		case KW_IF: printf("KW_IF\n"); break;
		case KW_THEN: printf("KW_THEN\n"); break;
		case KW_ELSE: printf("KW_ELSE\n"); break;
		case KW_WHILE: printf("KW_WHILE\n"); break;
		case KW_DO: printf("KW_DO\n"); break;
		case KW_FOR: printf("KW_FOR\n"); break;
		case KW_TO: printf("KW_TO\n"); break;

		case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
		case SB_COLON: printf("SB_COLON\n"); break;
		case SB_PERIOD: printf("SB_PERIOD\n"); break;
		case SB_COMMA: printf("SB_COMMA\n"); break;
		case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
		case SB_EQ: printf("SB_EQ\n"); break;
		case SB_NEQ: printf("SB_NEQ\n"); break;
		case SB_LT: printf("SB_LT\n"); break;
		case SB_LE: printf("SB_LE\n"); break;
		case SB_GT: printf("SB_GT\n"); break;
		case SB_GE: printf("SB_GE\n"); break;
		case SB_PLUS: printf("SB_PLUS\n"); break;
		case SB_MINUS: printf("SB_MINUS\n"); break;
		case SB_TIMES: printf("SB_TIMES\n"); break;
		case SB_SLASH: printf("SB_SLASH\n"); break;
		case SB_LPAR: printf("SB_LPAR\n"); break;
		case SB_RPAR: printf("SB_RPAR\n"); break;
		case SB_LSEL: printf("SB_LSEL\n"); break;
		case SB_RSEL: printf("SB_RSEL\n"); break;
	}
}

int scan(char *fileNameIn) {			// --- Ham xu li file dau vao ---
	Token *token;
	if (openInputStream(fileNameIn) == IO_ERROR){		// --- Mo luong doc file ---
		return IO_ERROR;								// --- Ket thuc neu co loi vao ro IO ---
	}

	token = getToken();
	while (token->tokenType != TK_EOF) {
		printToken(token);								// --- In ra Token lay duoc ---	
		free(token);											// --- Giai phong Token de tiet kiem bo nho ---
		token = getToken();										// --- Lay Token ke tiep ---
	}
	free(token);
	closeInputStream();
	return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		printf("scanner: no input file.\n");
		return -1;
	}

	if (scan(argv[1]) == IO_ERROR) {
		printf("Can\'t read input file!\n");
		return -1;
	}
	return 0;
}
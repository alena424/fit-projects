/**
* @file lex.h
* Implementation of IFJ17 imperative language compiler	
*
* @brief Lexical analysator for the compiler.
*
* @date 20.10.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#ifndef _LEX
#define _LEX

#include <stdio.h>
#include "token.h"

#define KEYWORDS_NUM 35 // Number of keywords and reserved words
#define CHUNK 16        // Value used to allocate chunk of memory

// Enum of all states used in finite state machine
typedef enum {
    s_init,
    s_line_comment,
    s_block_comment,
    s_num,
    s_real,
    s_exp,
    s_literal,
    s_identifier
} state_t;

// Function for copying pointer to source file to local variable used by lexical analysator
void file_init(FILE *fp);
// Function for editing insides of the token structure
token_t* edit_token(token_t *token, token_type_t type, char *data);
// Function for finding, recognizing and returning next token in source file.
token_t* get_token();

#endif

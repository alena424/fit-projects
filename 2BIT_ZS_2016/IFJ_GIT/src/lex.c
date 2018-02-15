/**
* @file lex.c
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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lex.h"
#include "token.h"

// Array of keywords and reserved words represented as string literals
char *keywords[] = {
    "as", "asc", "declare", "dim", "double", "do", "else", "end",
    "chr", "function", "if", "input", "integer", "length", "loop",
    "print", "return", "scope", "string", "substr", "then", "while",
    "and", "boolean", "continue", "elseif", "exit", "false", "for",
    "next", "not", "or", "shared", "static", "true"
};

// Array of keywords and reserved words represented as token types with positions coresponding
// to positions of the same keywords and reserved words in 'keywords' array
token_type_t keywords_tokens[] = {
    t_as, t_asc, t_declare, t_dim, t_double, t_do, t_else, t_end,
    t_chr, t_function, t_if, t_input, t_integer, t_length, t_loop,
    t_print, t_return, t_scope, t_string, t_substr, t_then, t_while,
    t_and, t_boolean, t_continue, t_elseif, t_exit, t_false, t_for,
    t_next, t_not, t_or, t_shared, t_static, t_true
};

FILE *fr_source = NULL;     // Variable used to store pointer to opened input file


/**
* @brief Function for copying pointer to source file to local variable used by lexical analysator.
* @param const FILE *fp: pointer to source file
*/
void file_init(FILE *fp) {
    fr_source = fp;
}

/**
* @brief Function for editing insides of the token structure.
* @param token_t *token: Pointer to token structure
* @param token_type_t type: Token type used as tokens type
* @param const char *data: Pointer to string (identifiers name, string or number literal, etc.)
* @return token_t *token: Pointer to token with changed data inside
*/
token_t* edit_token(token_t *token, token_type_t type, char *data) {
    token->type = type;
    token->data = data;
    return token;
}

/**
* @brief Function for finding, recognizing and returning next token in source file.
* @return token_t *token: Pointer to token with all necessary data inside
*/
token_t* get_token() {

    int c = '\0';           // Variable used to store one character loaded from the input file
    state_t state = s_init; // Variable used to store current state of the state machine

    token_t *token = NULL;  // Variable used to store pointer to 'token_t' structure that will be returned
    if ((token = malloc(sizeof(token_t))) == NULL) {
        return NULL;
    }
    edit_token(token, t_invalid, NULL);

    char *literal = NULL;   // Variable used to store data string of some tokens (string or number literals or name of an identifier)
    int index = 0;          // Variable used to store writing location in 'literal' variable
    int length = 0;         // Variable used to store number of characters in the 'literal' variable
    int allocated = 0;      // Variable used to store for how many characters is currently allocated memory
    int tail = 1;

    while(1) {

        switch(state) {

            /* ----- s_init -----
             * Skips all whitespaces, returns one character operators
             * and determines which state will be next. */
            case s_init:
                while(isspace(c = getc(fr_source)) && c != '\n')
                    ;
                c = tolower(c); // tolower becase it's case insensitive

                /* Return coresponding token. */
                if (c == EOF) {
                    return edit_token(token, t_eof, NULL);
                }

                /* Return coresponding token. */
                if (c == '\n') {
                    return edit_token(token, t_eol, NULL);
                }

                /* Return coresponding token. */
                if (c == '(') {
                    return edit_token(token, t_l_bracket, NULL);
                }

                /* Return coresponding token. */
                if (c == ')') {
                    return edit_token(token, t_r_bracket, NULL);
                }

                /* Return coresponding token. */
                if (c == ',') {
                    return edit_token(token, t_comma, NULL);
                }

                /* Return coresponding token. */
                if (c == ';') {
                    return edit_token(token, t_semicolon, NULL);
                }

                /* Return coresponding token. */
                if (c == '=') {
                    return edit_token(token, t_eq, NULL);
                }

                /* Checks if '<' is alone -> returns 't_lt' operator,
                 * if '>' is followed by '=' -> return 't_le' operator,
                 * if '>' is followed by '=' -> return 't_ne' operator. */
                if (c == '<') {
                    if ((c = getc(fr_source)) == '=' || c == '>')
                        return edit_token(token, (c == '=' ? t_le : t_ne), NULL);
                    else
                        ungetc(c, fr_source);   // Return unused character back to input stream
                    return edit_token(token, t_lt, NULL);
                }

                /* Checks if '>' is alone -> returns 't_gt' operator,
                 * if '>' is followed by '=' -> return 't_ge' operator. */
                if (c == '>') {
                    if ((c = getc(fr_source)) == '=')
                        return edit_token(token, t_ge, NULL);
                    else
                        ungetc(c, fr_source);   // Return unused character back to input stream
                    return edit_token(token, t_gt, NULL);
                }

                /* Return coresponding token. */
                if (c == '+') {
                    return edit_token(token, t_plus, NULL);
                }

                /* Return coresponding token. */
                if (c == '-') {
                    return edit_token(token, t_minus, NULL);
                }

                /* Return coresponding token. */
                if (c == '*') {
                    return edit_token(token, t_mul, NULL);
                }

                /* Return coresponding token. */
                if (c == '\\') {
                    return edit_token(token, t_int_div, NULL);
                }

                /* Checks if '/' is alone -> returns operator,
                 * if '/' is followed by '\'' -> go to s_block_comment state. */
                if (c == '/') {
                    if ((c = getc(fr_source)) == '\'') {
                        state = s_block_comment;
                        break;
                    }
                    else {
                        ungetc(c, fr_source);   // Return unused character back to input stream
                        return edit_token(token, t_div, NULL);
                    }
                }

                /* Go to s_line_comment state. */
                if (c == '\'') {
                    state = s_line_comment;
                    break;
                }

                /* Checks if '!' is alone -> returns invalid token,
                 * if '!' is followed by '\"' -> go to s_literal state. */
                if (c == '!') {
                    if ((c = getc(fr_source)) == '\"') {
                        state = s_literal;
                        break;
                    }
                    else {
                        ungetc(c, fr_source);
                        return edit_token(token, t_invalid, NULL);
                    }
                }

                /* Checks if c is a digit. */
                if (isdigit(c)) {
                    while(c == '0') {               // Skips all zeros
                        c = getc(fr_source);
                    }

                    ungetc(c, fr_source);       // First real number or other character back to stream
                    if (!isdigit(c))
                        ungetc('0', fr_source);     // If there were only zeros
                    state = s_num;
                    break;
                }

                /* Checks if c is valid first character of an identifier. */
                if (isalpha(c) || c == '_') {
                    ungetc(c, fr_source);       // First character of identifier back to stream
                    state = s_identifier;
                    break;
                }

                return edit_token(token, t_invalid, NULL);

            /* ----- s_line_comment -----
             * Skips all characters until end of line. */
            case s_line_comment:
                // Skip all characters until '\n'
                while((c = getc(fr_source)) != '\n' && c != EOF)
                    ;
                // Character '\n' is returned to input stream and can be used
                ungetc(c, fr_source);
                state = s_init;
                break;

            /* ----- s_block_comment -----
             * Skips all characters until "'/" sequence. */
            case s_block_comment:
                // Skip all characters until '\''
                while((c = getc(fr_source)) != '\'' && c != EOF)
                    ;
                // If was character '\'' followed by '/' it's valid end of the block comment 
                if ((c = getc(fr_source)) == '/') {
                    state = s_init;
                }
                // If block comment wasn't correctly ended it's not valid
                else if (c == EOF) {
                    return edit_token(token, t_invalid, NULL);
                }
                break;

            /* ----- s_literal -----
             * Reads string literal. */
            case s_literal:
                // Initial alloc
                if ((literal = realloc(literal, sizeof(char) * (allocated + CHUNK))) == NULL) {
                    free(token);
                    return NULL;
                }
                allocated += CHUNK;
                literal[0] = '\0';
                // Read characters as long as there is no EOF or '\n'
                while((c = getc(fr_source)) != EOF || c == '\n') {
                    // Variable used to store temporary string representing one escape sequence
                    char seq[5] = {'\0'};   // 5 because sequence has form "\\XXX\0"

                    // End when there is '\"' character
                    if (c == '\"') {
                        literal[index] = '\0';  // Final '\0' character is added to the end
                        return edit_token(token, t_literal, literal);
                    }

                    // If there is an probable escape sequence
                    if (c == '\\') {
                        c = getc(fr_source);    // Get second character after '\\'

                        // If the character is digit there should be a sequence "\NNN" where N are digits
                        if (isdigit(c)) {
                            char *end = NULL;
                            seq[0] = c;
                            seq[1] = getc(fr_source);   // Get the rest of the sequence
                            seq[2] = getc(fr_source);
                            seq[3] = '\0';

                            long res = strtol(seq, &end, 10);   // Variable used to save converted escape sequence
                            // If there wasn't a valid number (no characters but digits and within range) it's not valid
                            if (*end != '\0' || res < 1 || res > 255) {
                                ungetc(seq[0], fr_source);
                                ungetc(seq[1], fr_source);
                                ungetc(seq[2], fr_source);
                                free(literal);
                                return edit_token(token, t_invalid, NULL);
                            }

                            c = res; // Save converted escape sequence as char
                        } else {

                            // Other Supported escape sequences replaced by coresponding character
                            switch(c) {
                                case '\"':
                                    c = '\"';
                                    break;
                                case 'n':
                                    c = '\n';
                                    break;
                                case 't':
                                    c = '\t';
                                    break;
                                case '\\':
                                    c = '\\';
                                    break;
                                default:
                                    // Unsupported escape sequence is not valid
                                    ungetc(c, fr_source);
                                    free(literal);
                                    return edit_token(token, t_invalid, NULL);
                            }
                        }
                    }

                    tail = 1;
                    if (!isprint(c) || c > 127 || c == ' ' || c == '\"' || c == '\n' || c == '\t' || c == '\\') {
                        tail = 5;
                        sprintf(seq, "\\%03d", c);
                    }

                    // If there isn't enought space, allocate new CHUNK of memmory
                    if (allocated <= length + tail) {  // + 1 for '\0' character at the end
                        if ((literal = realloc(literal, sizeof(char) * (allocated + CHUNK))) == NULL) {
                            free(token);
                            return NULL;
                        }
                        allocated += CHUNK;
                    }

                    if (tail == 1) {
                        literal[index++] = c;   // Add character to string
                        literal[index] = '\0';
                        length++;
                    } else {
                        strcat(literal, seq);
                        index += 4;
                        length += 4;
                    }
                }

                // If literal wasn't correctly ended it's not valid literal
                ungetc(c, fr_source);
                free(literal);
                return edit_token(token, t_invalid, NULL);

            /* ----- s_num -----
             * Reads integer literal. */
            case s_num:
                // Read characters as long as they represent numbers
                while(isdigit(c = getc(fr_source))) {
                    // If there isn't enought space allocate new CHUNK of memmory
                    if (allocated <= length + 1) {  // + 1 for '\0' character at the end
                        if ((literal = realloc(literal, sizeof(char) * (allocated + CHUNK))) == NULL) {
                            free(token);
                            return NULL;
                        }
                        allocated += CHUNK;
                    }
                    literal[index++] = c;   // Add character to string
                    length++;
                }

                // If there is character '.' or 'e' change state to 's_real' or 's_exp'
                if (c == '.' || tolower(c) == 'e') {
                    state = c == '.' ? s_real : s_exp;
                    literal[index++] = tolower(c);  // Add character to string
                    length++;
                    break;
                // If the number is followed by anything but alpha character or '_' it's valid
                } else if (!isalnum(c) && c != '_') {
                    ungetc(c, fr_source);
                    literal[index] = '\0';  // Final '\0' character is added to the end
                    return edit_token(token, t_int, literal);
                // If the number is followed by alpha character or '_' it's  not valid
                } else {
                    ungetc(c, fr_source);
                    free(literal);
                    return edit_token(token, t_invalid, NULL);
                }

            /* ----- s_real -----
             * Reads decimal numbers after the '.'. */
            case s_real:
                // Read characters as long as they represent numbers
                while(isdigit(c = getc(fr_source))) {
                    // If there isn't enought space allocate new CHUNK of memmory
                    if (allocated <= length + 1) {  // + 1 for '\0' character at the end
                        if ((literal = realloc(literal, sizeof(char) * (allocated + CHUNK))) == NULL) {
                            free(token);
                            return NULL;
                        }
                        allocated += CHUNK;
                    }
                    literal[index++] = c;   // Add character to string
                    length++;
                }

                // Check if there is at least one digit
                if (index > 0 && !isdigit(literal[index - 1])) {
                    ungetc(c, fr_source);
                    free(literal);
                    return edit_token(token, t_invalid, NULL);
                // If there is character 'e' change state to 's_exp'
                } else if (tolower(c) == 'e') {
                    state = s_exp;
                    literal[index++] = tolower(c);  // Add character to string
                    length++;
                    break;
                // If the number is followed by anything but alpha character or '_' it's valid
                } else if (!isalnum(c) && c != '_') {
                    ungetc(c, fr_source);
                    literal[index] = '\0';  // Final '\0' character is added to the end

                    double res = 0;
                    sscanf(literal, "%lf", &res);   // Converting literal to double
                    sprintf(literal, "%g", res);    // Converting double back to literal in %g notation

                    return edit_token(token, t_real, literal);
                // If the number is followed by alpha character or '_' it's  not valid
                } else {
                    ungetc(c, fr_source);
                    free(literal);
                    return edit_token(token, t_invalid, NULL);
                }

            /* ----- s_exp -----
             * Reads exponent sign and numbers after it. */
            case s_exp:
                // Character 'e' must be followed by '+', '-' or character representing number
                if (isdigit(c = getc(fr_source)) || c == '+' || c == '-') {
                    // If there isn't enought space allocate new CHUNK of memmory
                    if (allocated <= length + 1) {  // + 1 for '\0' character at the end
                        if ((literal = realloc(literal, sizeof(char) * (allocated + CHUNK))) == NULL) {
                            free(token);
                            return NULL;
                        }
                        allocated += CHUNK;
                    }
                    literal[index++] = c;   // Add character to string
                    length++;

                    // Read characters as long as they represent numbers
                    while(isdigit(c = getc(fr_source))) {
                        // If there isn't enought space allocate new CHUNK of memmory
                        if (allocated <= length + 1) {  // + 1 for '\0' character at the end
                            if ((literal = realloc(literal, sizeof(char) * (allocated + CHUNK))) == NULL) {
                                free(token);
                                return NULL;
                            }
                            allocated += CHUNK;
                        }
                        literal[index++] = c;   // Add character to string
                        length++;
                    }
                }
                // Check if there is at least one digit
                if (index > 0 && !isdigit(literal[index - 1])) {
                    ungetc(c, fr_source);
                    free(literal);
                    return edit_token(token, t_invalid, NULL);
                // If the number is followed by anything but alpha character or '_' it's valid
                } else if (!isalnum(c) && c != '_') {
                    ungetc(c, fr_source);
                    literal[index] = '\0';  // Final '\0' character is added to the end

                    double res = 0;
                    sscanf(literal, "%lf", &res);   // Converting literal to double
                    sprintf(literal, "%g", res);    // Converting double back to literal in %g notation

                    return edit_token(token, t_real, literal);
                // If the number is followed by alpha character or '_' it's  not valid
                } else {
                    ungetc(c, fr_source);
                    free(literal);
                    return edit_token(token, t_invalid, NULL);
                }

            /* ----- s_identifier -----
             * Reads string of characters and determines if it's valid identifier,
             * or keyword. */
            case s_identifier:
                // Read characters as long as they represent numbers
                while(isalnum(c = getc(fr_source)) || c == '_') {
                    // If there isn't enought space allocate new CHUNK of memmory
                    if (allocated <= length + 1) {
                        if ((literal = realloc(literal, sizeof(char) * (allocated + CHUNK))) == NULL) {
                            free(token);
                            return NULL;
                        }
                        allocated += CHUNK;
                    }
                    literal[index++] = tolower(c);  // Add character to string
                    length++;
                }

                ungetc(c, fr_source);   // Return last unused character back to input stream
                literal[index] = '\0';  // Final '\0' character is added to the end

                // Check if loaded string coresponds to some keyword or reserved word
                for (int i = 0; i < KEYWORDS_NUM; i++) {
                    if (!strcmp(literal, keywords[i])) {
                        free(literal);
                        return edit_token(token, keywords_tokens[i], NULL);
                    }
                }

                return edit_token(token, t_identifier, literal);
        }
    }
}

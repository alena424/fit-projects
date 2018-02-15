
/**
* @file error.c
* Implementation of IFJ17 imperative language compiler
*
* @brief File contains error functions
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
#include "error.h"

/**
* @brief throws an error
* @param code error code
*/
void throwError ( errors code )
{
    int return_code_error = 1;

    switch ( code )
    {
    case error_scope:
        fprintf(stderr, "Expected 'scope'\n");
        return_code_error = 2;
        break;

    case error_identifier:
        fprintf(stderr, "Expected 'identifier'\n");
        return_code_error = 2;
        break;

    case error_while:
        fprintf(stderr, "Expected 'while'\n");
        return_code_error = 2;
        break;

    case error_then:
        fprintf(stderr, "Expected 'then'\n");
        return_code_error = 2;
        break;

    case error_else:
        fprintf(stderr, "Expected 'else'\n");
        return_code_error = 2;
        break;

    case error_if:
        fprintf(stderr, "Expected 'if'\n");
        return_code_error = 2;
        break;

    case error_expression:
        fprintf(stderr, "Error in expression\n");
        return_code_error = 2;
        break;

    case error_loop:
        fprintf(stderr, "Expected 'loop'\n");
        return_code_error = 2;
        break;

    case error_eof:
        fprintf(stderr, "Expected end of file\n");
        return_code_error = 2;
        break;

    case error_function:
        fprintf(stderr, "Expected 'function'\n");
        return_code_error = 2;
        break;

    case error_as:
        fprintf(stderr, "Expected 'as'\n");
        return_code_error = 2;
        break;

    case error_eol:
        fprintf(stderr, "Expected end of line\n");
        return_code_error = 2;
        break;
        
    case error_type:
	fprintf(stderr, "Error in data type\n");
        return_code_error = 2;
        break;

    case error_parenthesis_left:
        fprintf(stderr, "Expected '('\n");
        return_code_error = 2;
        break;

    case error_parenthesis_right:
        fprintf(stderr, "Expected ')'\n");
        return_code_error = 2;
        break;

    case error_end:
        fprintf(stderr, "Expected 'end'\n");
        return_code_error = 2;
        break;

    case error_return_scope:
        fprintf(stderr, "Unexpected return in scope\n");
        return_code_error = 2;
        break;

    case error_call_function:
        fprintf(stderr, "Error when calling function\n");
        return_code_error = 2;
        break;

    case error_comma:
        fprintf(stderr, "Expected ','\n");
        return_code_error = 2;
        break;

    case error_scanner:
        fprintf(stderr, "Invalid token\n");
        return_code_error = 1;
        break;

    case error_assignment:
        fprintf(stderr, "Error in assignment, expected '='\n");
        return_code_error = 2;
        break;

    case error_print:
        fprintf(stderr, "Print is in this format 'Print exp; exp; ...; exp;'\n");
        return_code_error = 2;
        break;

    case error_semicolon:
        fprintf(stderr, "Expected ';'\n");
        return_code_error = 2;
        break;

    case error_too_many_arg_def:
        fprintf(stderr, "Too many arguments in definition of function\n");
        return_code_error = 3;
        break;

    case error_arguments_missing_def:
        fprintf(stderr, "Missing arguments in definition of function\n");
        return_code_error = 3;
        break;

    case error_incompatible_type_def:
        fprintf(stderr, "Incompatible types in definition of function\n");
        return_code_error = 3;
        break;

    case error_wrong_arg:
        fprintf(stderr, "Wrong argument in definition of function\n");
        return_code_error = 3;
        break;

    case error_undeclared:
        fprintf(stderr, "Use of undeclared variable or function\n");
        return_code_error = 3;
        break;

    case error_redeclaration:
        fprintf(stderr, "Redeclaration\n");
        return_code_error = 3;
        break;

    case error_too_many_arg:
        fprintf(stderr, "Function has too many arguments\n");
        return_code_error = 4;
        break;

    case error_arguments_missing:
        fprintf(stderr, "Function needs more arguments\n");
        return_code_error = 4;
        break;

    case error_incompatible_type:
        fprintf(stderr, "Use of incompatible types\n");
        return_code_error = 4;
        break;
    
    case error_unknown_lex:
        fprintf(stderr, "Unknown error in lexical analysis\n");
        return_code_error = 2;
        break;

    case error_unknown_semantic:
        fprintf(stderr, "Unknown error in semantic analysis\n");
        return_code_error = 6;
        break;

    case error_internal:
        fprintf(stderr, "Unknown internal error\n");
        return_code_error = 99;
        break;

    default:
        fprintf(stderr, "Unknown syntax error\n");
        return_code_error = 2;
        break;

    }

    exit( return_code_error );

}


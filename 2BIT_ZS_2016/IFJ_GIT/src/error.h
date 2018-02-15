
/**
* @file error.h
* Implementation of IFJ17 imperative language compiler
*
* @brief File contains all possible errors that can happen
*
* @date 20.10.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#ifndef _ERROR
#define _ERROR

/**
* @brief enum type errros
* 1 - lex. analysis error
* 2 - syntax. analysis error
* 3 - semantic error: undefined/redefined variable
* 4 - semantic error: compatibility types, arguments in functions
* 6 - other semantic errors
* 99 - internal error (not influenced by compiler): allocation memory
*/

typedef enum
{
    error_scope, 				// 2, expected keyword scope
    error_identifier, 				// 2, expected identifier
    error_while, 				// 2, expected keyword while
    error_then,					// 2, expected keyword then
    error_else,					// 2, expected keyword else
    error_if,					// 2, expected keyword if
    error_expression, 				// 2, error in expression
    error_loop,					// 2, expected keyword loop
    error_eof,					// 2, expected end of file
    error_function,				// 2, expected keyword function
    error_as,					// 2, expected keyword as
    error_eol,					// 2, expected end of line
    error_type, 				// 2, error in data type, just String, Double, Integer
    error_parenthesis_left,			// 2, expected '('
    error_parenthesis_right,			// 2, expected '('
    error_end,					// 2, expected keyword end
    error_return_scope,				// 2, unexpected return in scope
    error_call_function,			// 2, error when calling function
    error_comma,				// 2, expected ','
    error_scanner,				// 1, token is invalid
    error_assignment,				// 2, error in assignment, expected '='
    error_print,				// 2, error in function print
    error_semicolon,				// 2, expected semicolon
    error_too_many_arg_def,			// 3, too many arguments in definition
    error_arguments_missing_def,		// 3, need more arguments in definiton
    error_incompatible_type_def,		// 3, incompatible type in definition
    error_wrong_arg,                            // 3, wrong argument
    error_redeclaration,			// 3, redeclaration of variable/function
    error_undeclared,				// 3, undeclared variable/function
    error_too_many_arg,				// 4, too many arguments
    error_arguments_missing,			// 4, need more arguments
    error_incompatible_type,			// 4, incompatible type,
    error_unknown_lex,				// 2, unknown error in lex. analysis
    error_unknown_semantic,			// 6, unknown error in semantic. analysis
    error_internal,				// 99, internal error (allocation, ...)
    error_unknown_syntax			// 2, unknown error in syntax analysis

} errors;

void throwError( errors code );

#endif

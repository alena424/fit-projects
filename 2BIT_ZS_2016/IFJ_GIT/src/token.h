/**
* @file token.h
* Implementation of IFJ17 imperative language compiler	
*
* @brief Decleration of all necessary structures and data types for tokens.
* @date 20.10.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#ifndef TOKEN
#define TOKEN

#define TOKEN_TYPES_NUM 57  // Number of all token types

// Enum of all token types
typedef enum {
    t_invalid,

    t_as, t_asc, t_declare, t_dim, t_double, t_do, t_else, t_end,
    t_chr, t_function, t_if, t_input, t_integer, t_length, t_loop,
    t_print, t_return, t_scope, t_string, t_substr, t_then, t_while,
    t_and, t_boolean, t_continue, t_elseif, t_exit, t_false, t_for,
    t_next, t_not, t_or, t_shared, t_static, t_true,

    t_plus, t_minus, t_mul, t_div, t_int_div,

    t_lt, t_gt, t_eq, t_le, t_ge, t_ne,

    t_eof, t_eol, t_l_bracket, t_r_bracket, t_comma, t_semicolon,

    t_int, t_real, t_literal, t_identifier
} token_type_t;

// Structure representing token that is returned by get_token() function
typedef struct {
    token_type_t type;
    char *data;
} token_t;

#endif

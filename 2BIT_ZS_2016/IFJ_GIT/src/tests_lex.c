/**
* @file tests_les.c
* Implementation of IFJ17 imperative language compiler
*
* @brief tests for lexical analyzator
*
* @date 20.10.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#include <stdlib.h>
#include "lex.h"
#include "token.h"

#define FALSE 1
#define TRUE 0

#define TEST_BASIC 8 //number of defined basic tests
#define TEST_FUNCTION 8 //number of defined functions tests
#define TEST_STATEMENT 7  //number of defined statement tests
#define TEST_EXPR 5
#define TEST_COND 4
#define TEST_LOOPS 1
#define TEST_STRINGS 1
#define TEST_ERR 2

//colors
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */

token_type_t token_types[] =
{
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
};

char *token_types_str[] =
{
    "t_invalid",

    "t_as", "t_asc", "t_declare", "t_dim", "t_double", "t_do", "t_else", "t_end",
    "t_chr", "t_function", "t_if", "t_input", "t_integer", "t_length", "t_loop",
    "t_print", "t_return", "t_scope", "t_string", "t_substr", "t_then", "t_while",
    "t_and", "t_boolean", "t_continue", "t_elseif", "t_exit", "t_false", "t_for",
    "t_next", "t_not", "t_or", "t_shared", "t_static", "t_true",

    "t_plus", "t_minus", "t_mul", "t_div", "t_int_div",

    "t_lt", "t_gt", "t_eq", "t_le", "t_ge", "t_ne",

    "t_eof", "t_eol", "t_l_bracket", "t_r_bracket", "t_comma", "t_semicolon",

    "t_int", "t_real", "t_literal", "t_identifier"
};


/**
@brief Function opens a file with a name file_name and expects array enumtypes (enum_count times)
@return true/false - depends on the test if the test was successfull
*/

int run_test ( char *file_name, token_type_t enumtypes[], int enum_count, int *correct, int *error )
{
    FILE *file;
    char c ;
    file = fopen( file_name, "r+");
    if ( file == NULL )
    {
        printf("Can not open file\n");
        return FALSE;
    }
    else
    {
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
               "File %s contains:\n"
               "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", file_name);
        // printf("___________________________________\n");
        while ( (c = fgetc(file) ) != EOF )
        {
            printf("%c", c);
        }
        printf("\n___________________________________\n\n");
    }

    rewind(file);
    //inicializuji
    file_init(file);
    //printf("%d times\n", enum_count);
    for ( int i = 0; i < enum_count; i++ )
    {
        token_t* token = get_token(); // v promenne token mam token
        if (token == NULL)
        {
            fclose(file);
            return FALSE;
        }

        int expected_index, returned_index;
        for (int j = 0; j < TOKEN_TYPES_NUM; j++)
        {
            if (enumtypes[i] == token_types[j])
                expected_index = j;
            if (token->type == token_types[j])
                returned_index = j;
        }

        if ( token->type != enumtypes[i] )
        {
            printf( RED "Expected type is %s\nReturned type is %s\n" RESET,
                    token_types_str[expected_index], token_types_str[returned_index]);
            (*error)++;
            fclose(file);
            if (token->data != NULL)
                free(token->data);
            free(token);
            return FALSE;
        }
        else
        {
            printf( GREEN "CORRECT" RESET CYAN" %s" RESET, token_types_str[expected_index]);
            if (token->type == t_identifier || token->type == t_literal ||
                    token->type == t_int || token->type == t_real)
                // pokud se jedna o identifikator, vypiseme hodnotu
                printf(": \'%s\'", token->data);
            printf("\n");
            (*correct)++;
        }

        if (token->data != NULL)
            free(token->data);
        free(token);
    }

    printf("\n");
    fclose(file);
    return TRUE;
}


int main ( /*int argc, char *argv[]*/ )
{

    /*********************** BASIC **********************/
    // counters
    int basic_correct = 0;
    int basic_error = 0;


    //from these files we take tests
    char *file_name[] =  { "tests/01_TEST_BASIC/01.code",
                           "tests/01_TEST_BASIC/02.code",
                           "tests/01_TEST_BASIC/03.code",
                           "tests/01_TEST_BASIC/04.code",
                           "tests/01_TEST_BASIC/05.code",
                           "tests/01_TEST_BASIC/06.code",
                           "tests/01_TEST_BASIC/07.code",
                           "tests/01_TEST_BASIC/08.code",

                           //scope extended - implemeted
                           "tests/01_TEST_BASIC/09.code",
                           "tests/01_TEST_BASIC/10.code",
                           
                         };


    printf("BASIC TESTS\n"
           "-----------------------------------\n\n");

    token_type_t first_test_enum[][100] =
    {

        {
            t_eol,
            t_scope, t_eol,
            t_end, t_scope, t_eof
        }, //01. - 6

        {
            t_eol,
            t_scope, t_eol,
            t_dim, t_identifier, t_as, t_string, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eol,
            t_dim, t_identifier, t_as, t_double, t_eol,
            t_end, t_scope, t_eof
        }, //02. - 21

        {
            t_eol,
            t_scope, t_eol,
            t_dim, t_identifier, t_as, t_string, t_eq, t_literal, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eq, t_int, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eq, t_int, t_eol,
            t_dim, t_identifier, t_as, t_double, t_eq, t_real, t_eol,
            t_dim, t_identifier, t_as, t_double, t_eq, t_real, t_eol,
            t_dim, t_identifier, t_as, t_double, t_eq, t_real, t_eol,
            t_eol,
            t_dim, t_identifier, t_as, t_double, t_eq, t_real, t_eol,
            t_end, t_scope, t_eof
        }, //03. - 56

        {
            t_eol,
            t_scope, t_eol,
            t_dim, t_identifier, t_as, t_string, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eol,
            t_dim, t_identifier, t_as, t_double, t_eol,
            t_dim, t_identifier, t_as, t_double,  t_eol,
            t_dim, t_identifier, t_as, t_double,  t_eol,
            t_dim, t_identifier, t_as, t_double,  t_eol,
            t_dim, t_identifier, t_as, t_double,  t_eol,
            t_end, t_scope, t_eof
        }, //04. - 41

        {
            t_eol,
            t_scope, t_eol,
            t_dim, t_identifier, t_as, t_string, t_eq, t_literal, t_eol,
            t_dim, t_identifier, t_as, t_string, t_eq, t_literal, t_eol,
            t_end, t_scope, t_eof
        }, //05. - 20

        {
            t_eol,
            t_scope, t_eol,
            t_eol,
            t_dim, t_identifier, t_as, t_string, t_eol,
            t_dim, t_identifier, t_as, t_string, t_eol,
            t_eol,
            t_eol,
            t_end, t_scope, t_eof
        }, //06. - 14

        {
            t_eol,
            t_eol,
            t_scope, t_eol,
            t_eol,
            t_dim, t_identifier, t_as, t_string, t_eq, t_literal, t_eol,
            t_eol,
            t_end, t_scope, t_eof
        }, //07. - 16

        {
            t_eol,
            t_scope, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eol,
            t_dim, t_identifier, t_as, t_double, t_eol,
            t_dim, t_identifier, t_as, t_string, t_eol,
            t_eol,
            t_print, t_identifier, t_semicolon, t_identifier, t_semicolon, t_identifier, t_semicolon, t_eol,
            t_end, t_scope, t_eof
        }, //08. - 28

        /** ZANOROVANI SCOPE - rozsireni**
                {
                    t_scope, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eq, t_integer, t_eol,
                t_scope, t_eol,
                    t_dim, t_identifier, t_as, t_integer, t_eq, t_integer, t_eol,
                    t_scope, t_eol,
                        t_dim, t_identifier, t_as, t_integer, t_eq, t_integer, t_eol,
                        t_scope, t_eol,
                            t_print, t_identifier,
                    t_end, t_scope,
                t_end, t_scope,
            t_end, t_scope,
        t_end, t_scope, t_eof
            }, //09. - 40

        {
            t_scope, t_eol,
                            t_dim, t_identifier, t_as, t_integer, t_eq, t_integer, t_eol,
                            t_scope, t_eol,
                                t_if, t_integer, t_lt, t_integer, t_then
                                    t_dim, t_identifier, t_as, t_integer, t_eq, t_integer, t_eol,
                                t_print, t_identifier,
                                t_end, i_if
                            t_end, t_scope,
                        t_end, t_scope, t_eof

        }, //10. - 32*/

    };

    int basic_counter[] = { 6, 21, 56, 41, 20, 14, 16, 28, /*40,32*/ }; // how many token we expect in each test

    for ( int i = 0; i < TEST_BASIC; i++)
    {
        run_test( file_name[i], first_test_enum[i], basic_counter[i], &basic_correct, &basic_error);
    }


    /*********************** FUNCTIONS **********************/

    // counters to zero
    int funct_correct = 0;
    int funct_error = 0;


    //from these files we take tests
    char *file_name_function[] =
    {
        "tests/02_TEST_FUNCTIONS/01.code",
        "tests/02_TEST_FUNCTIONS/02.code",
        "tests/02_TEST_FUNCTIONS/03.code",
        "tests/02_TEST_FUNCTIONS/04.code",
        "tests/02_TEST_FUNCTIONS/05.code",
        "tests/02_TEST_FUNCTIONS/06.code",
        "tests/02_TEST_FUNCTIONS/07.code",
        "tests/02_TEST_FUNCTIONS/08.code",

        /**extentions
        "tests/02_TEST_FUNCTIONS/09.code",
        "tests/02_TEST_FUNCTIONS/10.code",

        "tests/02_TEST_FUNCTIONS/11.code",
        "tests/02_TEST_FUNCTIONS/12.code",*/
    };


    printf("\n\nFUNCTIONS\n"
           "-----------------------------------\n\n");

    token_type_t second_test_enum[][100] =
    {

        {
            t_eol,
            t_eol,
            t_function, t_identifier, t_l_bracket, t_r_bracket, t_as, t_string,  t_eol,
            t_eol,
            t_end, t_function, t_eol,
            t_eol,
            t_scope, t_eol,
            t_end, t_scope, t_eof
        }, //01. - 19

        {
            t_eol,
            t_eol,
            t_declare, t_function, t_identifier, t_l_bracket, t_r_bracket, t_as, t_string, t_eol,
            t_eol,
            t_function, t_identifier, t_l_bracket, t_r_bracket, t_as, t_string, t_eol,
            t_end, t_function, t_eol,
            t_eol,
            t_scope, t_eol,
            t_end, t_scope, t_eof
        }, //02. - 27

        {
            t_eol,
            t_eol,
            t_function, t_identifier, t_l_bracket, t_r_bracket, t_as, t_string, t_eol,
            t_end, t_function, t_eol,
            t_eol,
            t_scope, t_eol,
            t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eol,
            t_eol,
            t_end, t_scope, t_eof
        }, //03. - 24

        {
            t_eol,
            t_eol,
            t_function, t_identifier,
            t_l_bracket,  t_identifier, t_as, t_string, t_comma, t_identifier, t_as, t_integer, t_r_bracket,
            t_as, t_string,  t_eol,
            t_end, t_function, t_eol,
            t_eol,
            t_scope, t_eol,
            t_end, t_scope, t_eof
        }, //04. - 25

        {
            t_eol,
            t_declare, t_function, t_identifier,
            t_l_bracket, t_identifier, t_as, t_string, t_comma, t_identifier, t_as, t_integer,
            t_r_bracket, t_as, t_string, t_eol,
            t_eol,
            t_function, t_identifier,
            t_l_bracket, t_identifier, t_as, t_string, t_comma, t_identifier, t_as, t_integer,
            t_r_bracket, t_as, t_string, t_eol,
            t_end, t_function, t_eol,
            t_eol,
            t_scope, t_eol,
            t_end, t_scope, t_eof
        }, //05. - 40

        {
            t_eol,
            t_declare, t_function, t_identifier,
            t_l_bracket, t_identifier, t_as, t_string, t_comma, t_identifier, t_as, t_string,
            t_r_bracket, t_as, t_string,  t_eol,
            t_eol,
            t_function, t_identifier,
            t_l_bracket, t_identifier, t_as, t_string, t_comma, t_identifier, t_as, t_integer,
            t_r_bracket, t_as, t_string, t_eol,
            t_end, t_function, t_eol,
            t_eol,
            t_scope, t_eol,
            t_end, t_scope, t_eof
        }, //06. - 40

        {
            t_eol,
            t_declare, t_function, t_identifier,
            t_l_bracket, t_identifier, t_as, t_string, t_comma, t_identifier, t_as, t_integer, t_comma, t_identifier, t_as, t_string,
            t_r_bracket, t_as, t_string, t_eol,
            t_eol,
            t_function, t_identifier,
            t_l_bracket,  t_identifier, t_as, t_string, t_comma, t_identifier, t_as, t_integer,
            t_r_bracket, t_as, t_string, t_eol,
            t_end, t_function, t_eol,
            t_eol,
            t_scope, t_eol,
            t_end, t_scope, t_eof
        }, //07. - 44

        {
            t_eol,
            t_eol,
            t_function, t_identifier, t_l_bracket, t_r_bracket, t_as, t_integer, t_eol,
            t_return, t_int, t_eol,
            t_end, t_function, t_eol,
            t_eol,
            t_scope, t_eol,
            t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eol,
            t_identifier, t_eq, t_identifier, t_l_bracket, t_r_bracket, t_eol,
            t_print, t_identifier, t_semicolon, t_eol,
            t_eol,
            t_end, t_scope, t_eof
        }, //08. - 38


    };

    int function_counter[] = { 19, 27, 25, 25, 40, 40, 44, 38 }; // how many token we expect in each test

    for ( int i = 0; i < TEST_FUNCTION; i++)
    {
        run_test( file_name_function[i], second_test_enum[i], function_counter[i], &funct_correct, &funct_error);
    }

    /*********************** STATEMENT **********************/

    // counters to zero
    int stat_correct = 0;
    int stat_error = 0;


    //from these files we take tests
    char *file_name_statement[] =
    {
        "tests/03_TEST_STATEMENTS/01.code",
        "tests/03_TEST_STATEMENTS/02.code",
        "tests/03_TEST_STATEMENTS/03.code",
        "tests/03_TEST_STATEMENTS/04.code",
        "tests/03_TEST_STATEMENTS/05.code",
        "tests/03_TEST_STATEMENTS/06.code",
        "tests/03_TEST_STATEMENTS/07.code",
        "tests/03_TEST_STATEMENTS/08.code",

    };


    printf("\n\nSTATEMENT\n"
           "-----------------------------------\n\n");

    token_type_t third_test_enum[][100] =
    {

        {
            t_eol,
            t_scope, t_eol,
            t_eol,
            t_dim, t_identifier, t_as, t_string, t_eol,
            t_identifier, t_eq, t_literal, t_eol,
            t_eol,

            t_dim, t_identifier, t_as, t_integer, t_eol,
            t_identifier, t_eq, t_int, t_eol,
            t_eol,

            t_dim, t_identifier, t_as, t_double, t_eol,
            t_identifier, t_eq, t_real, t_eol,
            t_eol,

            t_end, t_scope, t_eol, t_eof
        }, //01. - 38

        {
            t_eol,
            t_scope, t_eol,
            t_eol,

            t_dim, t_identifier, t_as, t_integer, t_eol,
            t_identifier, t_eq, t_real, t_eol,
            t_identifier, t_eq, t_real, t_eol,
            t_eol,
            t_dim, t_identifier, t_as, t_double, t_eol,
            t_identifier, t_eq, t_int, t_eol,
            t_eol,

            t_end, t_scope, t_eol, t_eof
        }, //02. - 32

        {
            t_eol,
            t_scope, t_eol,
            t_eol,

            t_dim, t_identifier, t_as, t_integer, t_eol,
            t_identifier, t_eq, t_literal, t_eol,
            t_eol,

            t_end, t_scope, t_eol, t_eof
        }, //03. - 18

        {
            t_eol,
            t_scope, t_eol,
            t_eol,

            t_dim, t_identifier, t_as, t_string, t_eol,
            t_identifier, t_eq, t_int, t_eol,
            t_eol,

            t_end, t_scope, t_eol, t_eof
        }, //04. - 18

        {
            t_eol,
            t_scope, t_eol,

            t_dim, t_identifier, t_as, t_integer, t_eol,
            t_input, t_identifier, t_eol,

            t_eol,
            t_dim, t_identifier, t_as, t_string, t_eol,
            t_input, t_identifier, t_eol,
            t_eol,
            t_dim, t_identifier, t_as, t_double, t_eol,
            t_input, t_identifier, t_eol,
            t_eol,
            t_input, t_identifier, t_eol,
            t_end, t_scope, t_eof
        }, //05. - 36

        {
            t_eol,
            t_scope, t_eol,
            t_print, t_int, t_semicolon, t_minus,t_int, t_semicolon,t_real, t_semicolon,t_eol,
            t_print, t_minus, t_real, t_semicolon, t_eol,
            t_print, t_real, t_semicolon, t_eol,
            t_end, t_scope, t_eof
        }, //06. - 23

        {
            t_eol,
            t_scope, t_eol,
            t_print, t_int, t_eol,
            t_end, t_scope, t_eof
        }, //07. - 9


    };

    int statement_counter[] = { 38, 32, 18, 18, 36, 23, 9,  }; // how many token we expect in each test

    for ( int i = 0; i < TEST_STATEMENT; i++)
    {
        run_test( file_name_statement[i], third_test_enum[i], statement_counter[i], &stat_correct, &stat_error);
    }

    /*********************** EXPRESSIONS **********************/

    // counters to zero
    int expr_correct = 0;
    int expr_error = 0;


    //from these files we take tests
    char *file_name_expr[] =
    {
        "tests/04_TEST_EXPRESSIONS/06.code",
        "tests/04_TEST_EXPRESSIONS/07.code",
        "tests/04_TEST_EXPRESSIONS/08.code",
        "tests/04_TEST_EXPRESSIONS/09.code",
        "tests/04_TEST_EXPRESSIONS/14.code",

    };


    printf("\nEXPRESSIONS\n"
           "-----------------------------------\n\n");

    token_type_t fourth_test_enum[][100] =
    {

        {
            t_eol,
            t_scope, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eq, t_int, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eq, t_int, t_eol,
            t_identifier, t_eq, t_identifier, t_plus, t_identifier, t_mul, t_identifier, t_plus, t_identifier, t_eol,
            t_print, t_identifier, t_semicolon, t_eol,

            t_end, t_scope, t_eol, t_eof
        }, //06. - 35

        {
            t_eol,
            t_scope, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eq, t_int, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eq, t_int, t_eol,
            t_identifier, t_eq, t_identifier, t_div, t_identifier, t_plus, t_identifier, t_minus, t_identifier, t_plus, t_int, t_eol,
            t_print, t_identifier, t_semicolon, t_eol,

            t_end, t_scope, t_eol, t_eof
        }, //07. - 37

        {
            t_eol,
            t_scope, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eq, t_int, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eq, t_int, t_eol,
            t_dim, t_identifier, t_as, t_double, t_eq, t_int, t_eol,
            t_dim, t_identifier, t_as, t_double, t_eq, t_int, t_eol,
            t_eol,
            t_print, t_identifier, t_div, t_identifier, t_semicolon, t_literal, t_semicolon, t_identifier, t_int_div,
            t_identifier, t_semicolon, t_eol,

            t_end, t_scope, t_eol, t_eof
        }, //08. - 48

        {
            t_eol,
            t_scope, t_eol,
            t_dim, t_identifier, t_as, t_string, t_eq, t_literal, t_eol,
            t_dim, t_identifier, t_as, t_string, t_eq, t_literal, t_eol,
            t_eol,
            t_print, t_identifier, t_semicolon, t_literal, t_semicolon, t_identifier, t_semicolon,t_eol,
            t_end, t_scope, t_eol, t_eof
        }, //09. - 30

        {
            t_eol,
            t_scope, t_eol,
            t_eol,
            t_dim, t_identifier, t_as, t_double, t_eol,
            t_identifier, t_eq, t_real, t_eol,
            t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eol,
            t_identifier, t_eq, t_int, t_eol,
            t_eol,
            t_dim, t_identifier, t_as, t_double, t_eol,
            t_identifier, t_eq, t_l_bracket, t_l_bracket, t_l_bracket, t_identifier, t_mul, t_identifier, t_r_bracket,
            t_div, t_identifier, t_r_bracket, t_mul, t_identifier, t_r_bracket, t_div, t_identifier, t_eol,
            t_eol,
            t_print, t_identifier, t_semicolon, t_eol,
            t_eol,
            t_end, t_scope, t_eof
        }, //14. - 56


    };

    int expr_counter[] = { 35, 37, 48, 30, 56 }; // how many token we expect in each test

    for ( int i = 0; i < TEST_EXPR; i++)
    {
        run_test( file_name_expr[i], fourth_test_enum[i], expr_counter[i], &expr_correct, &expr_error);
    }


    /*********************** CONDITIONS **********************/

    // counters to zero
    int cond_correct = 0;
    int cond_error = 0;


    //from these files we take tests
    char *file_name_cond[] =
    {
        "tests/05_TEST_CONDITIONS/08.code",
        "tests/05_TEST_CONDITIONS/09.code",
        "tests/05_TEST_CONDITIONS/10.code",
        "tests/05_TEST_CONDITIONS/11.code",

    };


    printf("\nCONDITIONS\n"
           "-----------------------------------\n\n");

    token_type_t fifth_test_enum[][100] =
    {

        {
            t_eol,
            t_eol,
            t_scope, t_eol,
            t_if, t_l_bracket, t_int, t_le, t_int, t_r_bracket, t_then, t_eol,
            t_print, t_int, t_semicolon, t_eol,
            t_else, t_eol,
            t_print, t_int, t_semicolon, t_eol,
            t_end, t_if, t_eol,
            t_end, t_scope, t_eof
        }, //08. - 28

        {
            t_eol,
            t_scope, t_eol,
            t_if, t_l_bracket, t_l_bracket, t_int, t_mul, t_l_bracket, t_real, t_div, t_int,t_r_bracket,
            t_plus, t_real, t_minus, t_real, t_r_bracket, t_gt,
            t_l_bracket, t_int, t_mul, t_l_bracket, t_real, t_div, t_int,t_r_bracket,
            t_plus, t_real, t_plus, t_real, t_r_bracket,
            t_r_bracket, t_then, t_eol,
            t_print, t_int, t_semicolon, t_eol,
            t_else, t_eol,
            t_print, t_int, t_semicolon, t_eol,
            t_end, t_if, t_eol,
            t_end, t_scope, t_eof
        }, //09. - 51

        {
            t_eol,
            t_eol,
            t_function, t_identifier, t_l_bracket, t_r_bracket, t_as, t_integer, t_eol,
            t_eol,
            t_return, t_int, t_plus, t_int, t_eol,
            t_eol,
            t_end, t_function, t_eol,
            t_eol,

            t_scope, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eol,
            t_identifier, t_eq, t_int, t_eol,
            t_eol,
            t_if, t_l_bracket, t_identifier, t_l_bracket, t_r_bracket, t_gt, t_identifier, t_r_bracket, t_then, t_eol,
            t_print, t_int, t_semicolon, t_eol,
            t_else, t_eol,
            t_print, t_int, t_semicolon, t_eol,
            t_end, t_if, t_eol,
            t_end, t_scope, t_eof

        }, //10. - 58

        {
            t_eol,
            t_scope, t_eol,
            t_if, t_identifier, t_l_bracket, t_r_bracket, t_gt, t_int, t_then, t_eol,
            t_print, t_int, t_semicolon, t_eol,
            t_else, t_eol,
            t_print, t_int, t_semicolon, t_eol,
            t_end, t_if, t_eol,
            t_end, t_scope, t_eof
        }, //11. - 27


    };

    int cond_counter[] = { 28, 51, 58, 27 }; // how many token we expect in each test

    for ( int i = 0; i < TEST_COND; i++)
    {
        run_test( file_name_cond[i], fifth_test_enum[i], cond_counter[i], &cond_correct, &cond_error);
    }

    /*********************** LOOPS **********************/

    // counters to zero
    int loops_correct = 0;
    int loops_error = 0;


    //from these files we take tests
    char *file_name_loops[] =
    {
        "tests/06_TEST_LOOPS/04.code",

    };


    printf("LOOPS\n -----------------------------------\n\n");

    token_type_t sixth_test_enum[][100] =
    {

        {
            t_eol,
            t_scope, t_eol,
            t_dim, t_identifier, t_as, t_double, t_eol,
            t_dim, t_identifier, t_as, t_double, t_eol,
            t_identifier, t_eq, t_int, t_minus, t_real, t_eol,
            t_do, t_while, t_identifier, t_gt, t_l_bracket, t_int, t_minus, t_real, t_r_bracket, t_eol,

            t_identifier, t_eq, t_int, t_eol,
            //t_print, t_identifier, t_semicolon, t_eol,
            t_do, t_while, t_identifier, t_gt, t_real, t_eol,
            t_print, t_int, t_semicolon, t_eol,
            t_identifier, t_eq, t_identifier, t_minus, t_real, t_eol,
            t_loop, t_eol,

            t_identifier, t_eq, t_identifier, t_plus,  t_identifier, t_mul, t_int, t_eol,
            t_loop, t_eol,

            t_identifier, t_eq, t_int, t_eol,
            t_do, t_while, t_identifier, t_gt, t_int, t_eol,
            t_print, t_int, t_semicolon, t_eol,
            t_identifier, t_eq, t_identifier, t_minus, t_real, t_eol,
            t_loop, t_eol,

            t_end, t_scope, t_eof
        } //04. - 86

    };

    int loops_counter[] = { 86}; // how many token we expect in each test

    for ( int i = 0; i < TEST_LOOPS; i++)
    {
        run_test( file_name_loops[i], sixth_test_enum[i], loops_counter[i], &loops_correct, &loops_error);
    }

    /*********************** STRINGS **********************/

    // counters to zero
    int strings_correct = 0;
    int strings_error = 0;


    //from these files we take tests
    char *file_name_strings[] =
    {
        "tests/07_TEST_STRINGS/10.code",

    };


    printf("\nSTRINGS\n"
           "-----------------------------------\n\n");

    token_type_t seventh_test_enum[][120] =
    {

        {
            t_eol,
            t_eol,
            t_scope, t_eol,
            t_dim, t_identifier, t_as, t_string, t_eol,
            t_dim, t_identifier, t_as, t_string, t_eol,
            t_dim, t_identifier, t_as, t_integer, t_eol,
            t_eol,
            t_eol,
            t_eol,
            t_identifier, t_eq, t_literal, t_eol,
            t_identifier, t_eq, t_identifier, t_plus, t_literal, t_eol,
            t_print, t_identifier, t_semicolon, t_literal,  t_semicolon, t_identifier, t_semicolon,
            t_literal,t_semicolon, t_eol,

            t_identifier, t_eq, t_length, t_l_bracket, t_identifier, t_r_bracket, t_eol,
            t_identifier, t_eq, t_identifier, t_minus, t_int, t_plus, t_int, t_eol,

            t_identifier, t_eq, t_substr, t_l_bracket, t_identifier, t_comma, t_identifier, t_comma, t_int,
            t_r_bracket, t_eol,
            t_print, t_literal,  t_semicolon, t_identifier, t_semicolon, t_literal,
            t_semicolon, t_identifier, t_semicolon, t_literal, t_semicolon, t_identifier,t_semicolon, t_literal,
            t_semicolon, t_eol,
            t_eol,
            t_print, t_literal, t_semicolon, t_eol,
            t_print, t_literal, t_semicolon, t_eol,
            t_eol,
            t_input, t_identifier, t_eol,
            t_do, t_while, t_l_bracket, t_identifier, t_ne, t_literal, t_r_bracket, t_eol,
            t_print, t_literal, t_semicolon,t_eol,
            t_input, t_identifier, t_eol,

            t_loop, t_eol,

            t_end, t_scope, t_eof
        }, //10. - 116


    };

    int strings_counter[] = { 116 }; // how many token we expect in each test

    for ( int i = 0; i < TEST_STRINGS; i++)
    {
        run_test( file_name_strings[i], seventh_test_enum[i], strings_counter[i], &strings_correct, &strings_error);
    }

    /*********************** ERRORS **********************/

    // counters to zero
    int err_correct = 0;
    int err_error = 0;


    //from these files we take tests
    char *file_name_err[] =
    {
        "tests/08_TEST_ERRORS/01.code",
        "tests/08_TEST_ERRORS/14.code",
        /*"tests/08_TEST_ERRORS/03.code",
        "tests/08_TEST_ERRORS/04.code",
        "tests/08_TEST_ERRORS/05.code",*/
    };


    printf("\nERRORS\n -----------------------------------\n\n");

    token_type_t eight_test_enum[][50] =
    {

        {
            t_eol,
            t_invalid

        }, //01. - 2
        {
            t_eol,
            t_eol,
            t_scope, t_eol,
            t_input, t_identifier, t_eol,
            t_end, t_scope, t_invalid

        }, //02. - 10


    };

    int err_counter[] = { 2, 10 }; // how many token we expect in each test

    for ( int i = 0; i < TEST_ERR; i++)
    {
        run_test( file_name_err[i], eight_test_enum[i], err_counter[i], &err_correct, &err_error);
    }



    printf("\n=========== SUMMARIZE BASIC ===========\n");
    printf( RED "ERRORS: %d \n" RESET, basic_error);
    printf( GREEN "OK: %d \n" RESET, basic_correct);

    printf("\n========= SUMMARIZE FUNCTIONS =========\n");
    printf( RED "ERRORS: %d \n" RESET, funct_error);
    printf( GREEN "OK: %d \n" RESET, funct_correct);

    printf("\n========= SUMMARIZE STATEMENT =========\n");
    printf( RED "ERRORS: %d \n" RESET, stat_error);
    printf( GREEN "OK: %d \n" RESET, stat_correct);


    printf("\n========= SUMMARIZE EXPRESSIONS =========\n");
    printf( RED "ERRORS: %d \n" RESET, expr_error);
    printf( GREEN "OK: %d \n" RESET, expr_correct);

    printf("\n========= SUMMARIZE CONDITIONS =========\n");
    printf( RED "ERRORS: %d \n" RESET, cond_error);
    printf( GREEN "OK: %d \n" RESET, cond_correct);

    printf("\n========= SUMMARIZE LOOPS =========\n");
    printf( RED "ERRORS: %d \n" RESET, loops_error);
    printf( GREEN "OK: %d \n" RESET, loops_correct);

    printf("\n========= SUMMARIZE STRINGS =========\n");
    printf( RED "ERRORS: %d \n" RESET, strings_error);
    printf( GREEN "OK: %d \n" RESET, strings_correct);

    printf("\n========= SUMMARIZE ERRORS =========\n");
    printf( RED "ERRORS: %d \n" RESET, strings_error);
    printf( GREEN "OK: %d \n" RESET, strings_correct);

    return 0;
}

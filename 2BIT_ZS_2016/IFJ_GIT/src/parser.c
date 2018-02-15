/**
 * @file parser.c
 * Implementation of IFJ17 imperative language compiler
 *
 * @brief recursive descent parser
 *
 * @date 30.10.2017
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
#include "parser.h"
#include "generator.h"
#include "gen_stack.h"

// Global instruction list
//instrList globalInstrList;

// Current token
token_t *token = NULL;

TSymtable *localTable;

// While / ifuses counter
int numWhile,
    numIf;

// Global pointers to stacks for nested whiles and ifs
genStack *whileStack,
         *ifStack;

// Pointers used for generator arguments preparation
char *addr1,
     *addr2,
     *addr3;

/**
 * @brief Function which checks whether we got expected token type.
 *
 * @param type token_type_t   expected token type
 * @param error   errors      error type
 */
void check(token_type_t type, errors error) {
   // Unexpected token type
   if (token->type != type) {
      freeAndError( error );
   }
}

/**
 * @brief Function frees the old token, loads a new one and checks whether
 *        operation is successful.
 */
void next() {

   // Frees token data
   if (token != NULL && token->data != NULL) {
      //free(token->data);
   }

   // Frees token itself
   if (token != NULL) {
      //free(token);
   }

   // Gets new token from lex
   token = get_token();

   // Internal error from lex
   if (token == NULL) {
      symtabFree(globalTable);
      throwError(error_internal);
   }

   // Invalid token -> lexical error
   if (token->type == t_invalid) {
      freeAndError(error_scanner);
   }

}

/**
 * @brief Function loads new token and check its type.
 *        Uses functions next() and check().
 *
 * @param type token_type_t   expected token type
 * @param error   errors      error type
 */
void getAndCheck(token_type_t type, errors error) {

   // Calls for new token
   next();

   // Checks token type
   check(type, error);

}


/**
 * @brief Function frees a token and calls error handling function.
 *
 * @param error   errors   error type
 */
void freeAndError(errors error) {

   // Frees token data
   if (token != NULL && token->data != NULL) {
      //free(token->data);
   }

   // Frees actual token
   if (token != NULL) {
      //free(token);
   }

   // Frees global table
   symtabFree(globalTable);

   // Handles error
   throwError(error);
}


/**
 * @brief Function decodes token types to data types used in hash table.
 *        Used for checking data types. 
 */
Type_data decodeType() {
   
   // Checks token type
   switch (token->type) {
      case t_integer:
         return TInteger;

      case t_double:
         return TDouble;

      case t_string:
         return TString;

      default:
         return Unknown;
   }

   return Unknown;
}


/**
 * @brief Functions represents LL rules:
 *    1. <program> -> <function> <program>
 *    2. <program> -> <scope> EOF
 *    
 */
void program() {

   // Calls for new token
   next();

   // Skips empty lines
   while (token->type == t_eol) {
      next();
   }

   // Use different rules - dependent on type of a token
   switch (token->type) {

      // <program> -> <function> <program>
      case t_declare:
      case t_function:

         // Analysis of function
         function();

         // Recursive analysis of the rest of the program
         program();

         return;


      // <program> -> <scope> EOF
      case t_scope:
         
         // Checks whether all functions are defined
         if (isDefined(globalTable) == false) {
            freeAndError(error_undeclared);
         }

         // Analysis of scope
         scope();

         // Loads next token
         next();

         // Skips 
         while (token->type == t_eol) {
            next();
         }

         // Checks for EOF - otherwise throws an error
         check(t_eof, error_eof);

         return;

      // Error
      default:

         // Error handling function
         freeAndError(error_unknown_syntax);
   }
}

/**
 * @brief Function represents two rules for global parts of program:
 *    1) Declaration of of a function: <function> -> Declare Function idf ( <parameters> ) As <type> EOL
 *    2) Header of a function:         <function> -> Function idf ( <parameters> ) As <type> EOL <statementList> End Function
 */
void function() {

   // Token is ready from function "program()"
   // We'll prepare pointer to hash table item
   htab_listitem *fce;

   // 3 modes:
   //    0 - exiting declaration
   //    1 - exiting definition
   //    2 - exiting definition, which is also a declaration
   int mode = 0;

   // Use different rules - dependent on type of a token
   switch(token->type) {

      // <function> -> Declare Function idf ( <parameters> ) As <type> EOL
      case t_declare:      

         // Checks whether next token is "Function"
         getAndCheck(t_function, error_function);

         // Checks whether next token is identifier
         getAndCheck(t_identifier, error_identifier);

         // Search in globalTable whether identifier is already declared.
         fce = symtabFind(globalTable, token->data);

         // If it is declared -> error
         if (fce != NULL) {
            freeAndError(error_redeclaration);
         } else {
            // Otherwise new record in the table for this function
            if ((fce = symtabLookAdd(globalTable, token->data)) == NULL) {
               freeAndError(error_internal);
            }
         }

         // Checks whether next is a left bracket
         getAndCheck(t_l_bracket, error_parenthesis_left);

         // Calls for a new token
         next();

         // Calls function for handling parameters
         parameters(mode, fce);

         // Checks whether our token is a right bracket
         check(t_r_bracket, error_parenthesis_right);

         // Checks whether next token is "As"
         getAndCheck(t_as, error_as);

         // Calls for a new token
         next();

         // Checks whether next token is a valid return type
         if (decodeType() == Unknown) {
            freeAndError(error_type);
         }

         // Adds return type to function record in globalTable.
         if (symtabAddType(globalTable, fce->name, decodeType()) == NULL) {
            freeAndError(error_internal);
         }

         // Checks whether next token is EOL
         getAndCheck(t_eol, error_eol);

         return;


      // <function> -> Function idf ( <parameters> ) As <type> EOL <statementList> End Function
      case t_function:  

         // Checks whether next token is an identifier
         getAndCheck(t_identifier, error_identifier);
            
         // Checks global whether function is already declared
         fce = symtabFind(globalTable, token->data);

         if ( fce != NULL ) { 
            // Function is already declared -> mode = 1
            mode = 1;

            // Checks whether function is already defined
            if (fce->is_defined == true){
               freeAndError(error_undeclared);
            }
         } else {
            // Function is not declared, we'll add new record for this function -> mode = 2
            if ((fce = symtabLookAdd(globalTable, token->data)) == NULL) {
               freeAndError(error_internal);
            }
            mode = 2;
         }

         // Allocates new local table for local variables
         fce->local_table = symtabInit(TABLE_SIZE);

         // Malloc fail -> internal error
         if (fce->local_table == NULL) {
            freeAndError(error_internal);
         }

         // + Generating

         // Label
         instrListAppendInstrLabel(&globalInstrList, "LABEL", fce->name, -1);

         // Pushframe
         instrListAppend(&globalInstrList, "PUSHFRAME", NULL, NULL, NULL);

         // Temporary variables for converting types on stack
         instrListAppend(&globalInstrList, "DEFVAR", "LF@%tmp1", NULL, NULL);
         instrListAppend(&globalInstrList, "DEFVAR", "LF@%tmp2", NULL, NULL);
         instrListAppend(&globalInstrList, "DEFVAR", "LF@%tmp3", NULL, NULL);

         // - Generating

         // Temporary pointer for local_table
         localTable = fce->local_table;

         // Checks whether next token is left bracket
         getAndCheck(t_l_bracket, error_parenthesis_left);

         // Calls for a new token
         next();

         // Calls function for parameters handling
         parameters(mode, fce);

         // Checks whether next token is a right bracket
         check(t_r_bracket, error_parenthesis_right);

         // Checks whether next token is "As"
         getAndCheck(t_as, error_as);

         // Calls for a new token
         next();

         // Checks whether token is a valid return type
         if (decodeType() == Unknown) {
            freeAndError(error_type);
         }

         // If it is both definition of declaration of a function
         if (mode == 2) {
            // Adds return type into the function record in globalTable 
            if ( symtabAddType( globalTable, fce->name, decodeType() ) == NULL ) {
               freeAndError( error_internal );
            }
         } else {
            // If it is already declared in past
            // Checks whether we have right return type - if current token is 
            if ( fce->data_type != decodeType() ) {
               freeAndError( error_incompatible_type_def );
            }
         }

         // Adds "variable" return into the local table (for checking return type)
         if (symtabLookAdd(localTable, "return") == NULL ||
             symtabAddType(localTable, "return", decodeType()) == NULL) {

            freeAndError( error_internal );
         }

         // Checks whether next token is EOL
         getAndCheck(t_eol, error_eol);

         // + Generating

         // Return value
         instrListAppend(&globalInstrList, "DEFVAR", "LF@%retval", NULL, NULL);

         // Null initialization
         if (fce->data_type == TInteger) 
            instrListAppend(&globalInstrList, "MOVE", "LF@%retval", "int@0", NULL);
         if (fce->data_type == TDouble)  
            instrListAppend(&globalInstrList, "MOVE", "LF@%retval", "float@0.0", NULL);
         if (fce->data_type == TString)  
            instrListAppend(&globalInstrList, "MOVE", "LF@%retval", "string@", NULL);

         // - Generating
         
         // Calls for a new token
         next();

         // We're in statement list and NOT in scope
         statementList(false);

         // Checks whether our token is "End"
         check(t_end, error_end);

         // Checks whether next token is "Function"
         getAndCheck(t_function, error_function);

         // Function was defined
         fce->is_defined = true;

         // + Generating
         
         instrListAppend(&globalInstrList, "POPFRAME", NULL, NULL, NULL);
         instrListAppend(&globalInstrList, "RETURN", NULL, NULL, NULL);

         // - Generating

         return;


      // Something else -> error
      default: 
         freeAndError(error_unknown_syntax);
   }
}


/**
 * @brief Function represents rules for parameters handling of a function
 *    1. <parameters> -> id As <type> <parameters2>
 *
 * @param mode int            describes current function mode
 * @param fce  htab_listitem  pointer to hash table record
 */
void parameters(int mode, htab_listitem *fce) {

   // Pointer to first parameter of a function - NULL if currently declaring
   TParam *par = fce->params;

   // Use different rules - dependent on type of a token
   switch (token->type) {

      // <parameters> -> id As <type> <parameters2>
      case t_identifier:

         // If we are in definition and there are more parameters then in declaration
         if (mode == 1 && par == NULL) {
            freeAndError(error_too_many_arg_def);
         }

         // Checks whether param name is unique
         if (symtabFind(globalTable, token->data)){
            freeAndError(error_redeclaration);
         }

         // Allocating memory for of parameter
         char *name = malloc(sizeof(char) * (strlen(token->data) + 1));

         if (name == NULL) {
            freeAndError(error_internal);
         }
         
         // Copies param name from token
         strcpy(name, token->data);

         // Calls for a new token
         next();

         // Checks whether our token is "As"
         if (token->type != t_as) {
            //free(name);
            freeAndError( error_as );
         }

         // Calls for a new token
         next();

         // Checks whether token is valid return type
         if (decodeType() == Unknown) {
            //free(name);
            freeAndError(error_type);
         }

         // We're in function definition and declaration was earlier (mode 1)
         if (mode == 1) {

            // Checks for types match in declaration and definition
            if (par->data_type != decodeType()) {
               //free(name);
               freeAndError(error_incompatible_type_def);
            }

            // Actualizes param name
            if (strcmp(par->name, name)) {
               strcpy(par->name, name);
            }

            // Adds parameter into local table of variables
            if (symtabLookAdd(localTable, name) == NULL ||
                symtabAddType(localTable, name, decodeType()) == NULL) {

               //free(name);
               freeAndError(error_internal);
            }

         } else {

            // We're in mode 0 or 2 (both creating new table records)
            if (symtabAddParam(globalTable, fce->name, decodeType(), name) == NULL) {
               //free(name);
               freeAndError(error_internal);
            }

            // Sets temp pointer to parameters
            par = fce->params;

            // If we're currently both defining and declaring
            // Adds parameters into local table of variables
            if (mode == 2) {
               if (symtabLookAdd(localTable, name) == NULL ||
                   symtabAddType(localTable, name, decodeType()) == NULL) {

                   //free(name);
                   freeAndError(error_internal);
               }
            }
         }

         // Frees temp name variable
         //free(name);

         // Calls for a new token
         next();

         // Check other pamareters
         parameters2(mode, fce, par);

         return;

      // No parameters or something bad
      default:

         // Number of definition parameters differ from declaration.
         if (mode == 1 && par != NULL) {
            freeAndError( error_arguments_missing_def );
         }

         return;
   }
}


/**
 * @brief Function represents second part of rule for parameters handling.
 *    1. <parameters2> -> , id As Type <parameters2>
 *
 * @param mode int            describes current function mode
 * @param fce  htab_listitem  pointer to hash table record
 * @param par  TParam         pointer to last checked parameter
 */
void parameters2( int mode, htab_listitem *fce, TParam *par ) {

   // Temporary variable for storing parameters
   TParam *tmp;
   
   // Use different rules - dependent on type of a token
   switch (token->type) {

         // <parameters2> -> , id As Type <parameters2>
         case t_comma:

            // Declared function has less parameters
            if (mode == 1 && par->next == NULL) {
               freeAndError( error_too_many_arg_def );
            }

            // Checks whether next token is an identifier
            getAndCheck(t_identifier, error_identifier);

            // Checks parameter naming collisions
            if (symtabFind(globalTable, token->data)){
               freeAndError(error_redeclaration);
            }

            tmp = fce->params;

            while (1) {
               if (!strcmp(tmp->name, token->data)) {
                  freeAndError(error_redeclaration);
               }
               // End of loop while we got last checked parameter
               if (tmp == par) {
                  break;
               }
               tmp = tmp->next;
            }

            // Allocates memory for name of paramater
            char *name = malloc(sizeof(char) * (strlen(token->data) + 1));       
            if (name == NULL) {
               freeAndError( error_internal );
            }
            
            // Saves paramater name
            strcpy(name, token->data);

            // Calls for a new token
            next();

            // Checks whether our token is "As"
            if (token->type != t_as) {
               //free(name);
               freeAndError(error_as);       
            }
                           
            // Calls for a new token
            next();

            // Checks whether token is a valid return type
            if (decodeType() == Unknown) {
               //free(name);
               freeAndError(error_type);
            }
            
            if (mode == 1) {

               // Checks whether parameter type is same as in declaration
               if (par->next->data_type != decodeType()) {
                  //free(name);
                  freeAndError( error_incompatible_type_def );
               }

               // Actualizes param name
               if (strcmp(par->next->name, name)) {
                  strcpy(par->next->name, name);
               }

               // Adds parameter into local table
               if (symtabLookAdd( localTable, name) == NULL ||
                   symtabAddType( localTable, name, decodeType()) == NULL ) {
                      //free( name );
                      freeAndError( error_internal );
               } 

            } else { 

               // Adds record to global table
               if (symtabAddParam(globalTable, fce->name, decodeType(), name) == NULL) {
                  //free(name);
                  freeAndError(error_internal);
               }

               // If it is both definition and declaration, we'll add to local table
               if (mode == 2) {
                  if (symtabLookAdd(localTable, name) == NULL ||
                      symtabAddType(localTable, name, decodeType()) == NULL) {

                     //free(name);
                     freeAndError(error_internal);
                  }
               }
            }

            //free(name);

            // Calls for a next token
            next();

            // Recursively calls function for handling other parameters
            parameters2(mode, fce, par->next);

            return;

         // No parameters or something bad
         default:

            // If number of parameters in definition and declaration doesn't match
            if ( mode == 1 && par->next != NULL ) {
               freeAndError( error_arguments_missing_def );
            }

            return;
      }
}

/**
 * @brief Function represents rules for main part of program
 *    1. <scope> -> SCOPE EOL <statementList> End Scope
 */
void scope() {

   // Pointer to global table record
   htab_listitem *fce;
   
   // Use different rules - dependent on type of a token
   switch (token->type) {

      // <scope> -> SCOPE EOL <statementList> End Scope
      case t_scope:
         
         // Checks whether next token is EOL
         getAndCheck(t_eol, error_eol);
         
         // Adds record for scope to global table (same as for function)
         if ((fce = symtabLookAdd( globalTable, "scope" )) == NULL) {
            freeAndError( error_internal );
         }

         // Allocates local table
         fce->local_table = symtabInit(TABLE_SIZE);

         if (fce->local_table == NULL) {
            freeAndError( error_internal );
         }        

         // Temp variable for localtable
         localTable = fce->local_table;         

         // + Generating
         instrListAppend(&globalInstrList, "LABEL", "$scope", NULL, NULL);

         instrListAppend(&globalInstrList, "DEFVAR", "GF@%tmp1", NULL, NULL);
         instrListAppend(&globalInstrList, "DEFVAR", "GF@%tmp2", NULL, NULL);
         instrListAppend(&globalInstrList, "DEFVAR", "GF@%tmp3", NULL, NULL);

         // - Generating

         // Calls for next token
         next();

         // We're in statement list comming from scope
         statementList(true);

         // Checks whether our token is "End"
         check(t_end, error_end);

         // Checks whether next token is "Scope"
         getAndCheck(t_scope, error_scope);

         return;

      // Something else -> error
      default:
         freeAndError(error_unknown_syntax);
   }

}

/**
 * @brief Function represents rules for creating statements:
 *    1. <statementList> -> <statement> EOL <statementList>
 *    2. <statementList> -> epsilon
 *
 * @param scope   bool  1/0 whether we are in scope or in a function 
 */
void statementList(bool scope) {
   // Token is already loaded
   
   // Skip EOLs
   while (token->type == t_eol) {
      next();
   }

   // Use different rules - dependent on type of a token
   switch (token->type) {

      // <statementList> -> <statement> <statementList>
      case t_if:
      case t_do:
      case t_return:
      case t_input:
      case t_print:
      case t_dim:
      case t_identifier:

         // Evaluations of statements
         statement(scope);

         // Each statement is ended w/ EOL
         check(t_eol, error_eol);

         // Calls for a new token
         next(); 

         // Recursive call
         statementList(scope);

         return;
      
      // Evaluation of token after return in a function which called statementList()
      default:
         return;
   }
}


/**
 * @brief Funkce represents rules for these statements:
 * 1. <statement> -> If <expression> Then EOL <statementList> Else EOL <statementList> End If
 * 2. <statement> -> Do While <expression> Then EOL <statementList> LOOP
 * 3. <statement> -> Return <expression>
 * 4. <statement> -> Input id
 * 5. <statement> -> Print <print>
 * 6. <statememt> -> Dim id As Type <inicialization>
 * 7. <statement> -> id = <assignment>
 *
 * @param scope   bool  1/0 whether we are in a scope
 */
void statement(bool scope) {
   
   // Variable for storing expression type
   exprType typeExpr;

   // Pointers - var for local table item and fce global table item
   htab_listitem *var, *fce;

   // Variable data type
   Type_data typeId;

   // Use different rules - dependent on type of a token
   switch (token->type) {

      // <statement> -> If <expression> Then EOL <statementList> Else EOL <statementList> End If
      case t_if:

         // Calls for a new token
         next();

         // Calls function for handling expression and check valid type in condition
         if (expression(token, localTable) != ex_bool) {
            freeAndError(error_incompatible_type);
         }

         // + Generating

         numIf++;
         genStackPush(ifStack, numIf);

         // Hard push bool for ifneq jump
         instrListAppend(&globalInstrList, "PUSHS", "bool@true", NULL, NULL);
         instrListAppendInstrLabel(&globalInstrList, "JUMPIFNEQS", "else", numIf);

         // - Generating
         
         // Checkes whether our token is "then"
         check(t_then, error_then);

         // Checkes whether next token is EOL
         getAndCheck(t_eol, error_eol);
         
         // Calls for a new token
         next();

         // Evaluetaes statement
         statementList(scope);

         // In case of using bonus
/*       elsif( scope );
         next();
         return;*/

         // Checkes whether our token is "Else"
         check(t_else, error_else);

         // Checkes whether next token is EOL
         getAndCheck(t_eol, error_eol);

         // + Generating

         instrListAppendInstrLabel(&globalInstrList, "JUMP", "endif", genStackTop(ifStack));
         instrListAppendInstrLabel(&globalInstrList, "LABEL", "else", genStackTop(ifStack));

         // - Generating

         // Calls for a new token
         next();
         
         // Evaluetes statement
         statementList(scope);

         // Checkes whether our token is "End"
         check(t_end, error_end);

         // Checkes whether next token is "If"
         getAndCheck(t_if, error_if);

         // + Generating

         instrListAppendInstrLabel(&globalInstrList, "LABEL", "endif", genStackTop(ifStack));

         genStackPop(ifStack);

         // - Generating

         // Calls for a new token
         next();

         return;

      // <statement> -> Do While <expression> Then EOL <statementList> LOOP
      case t_do:

         // Checks whether next token is "While"
         getAndCheck(t_while, error_while);

         // + Generating

         numWhile++;

         genStackPush(whileStack, numWhile);

         instrListAppendInstrLabel(&globalInstrList, "LABEL", "while", numWhile);

         // - Generating
         
         // Calls for a new token
         next();

         // Calls function for handling expression and check valid type in condition
         if (expression(token, localTable) != ex_bool) {
            freeAndError(error_incompatible_type);
         }

         // + Generating

         // Hardpush true and check whether while should end
         instrListAppend(&globalInstrList, "PUSHS", "bool@true", NULL, NULL);
         instrListAppendInstrLabel(&globalInstrList, "JUMPIFNEQS", "endwhile", numWhile);

         // - Generating

         // Checks whether our token is EOL
         check(t_eol, error_eol);

         // Calls for a new token
         next();

         // Evaluates statement (we're in scope)
         statementList(scope);

         // Checks whether our token is "Loop"
         check(t_loop, error_loop);

         // + Generating

         instrListAppendInstrLabel(&globalInstrList, "JUMP", "while", genStackTop(whileStack));
         instrListAppendInstrLabel(&globalInstrList, "LABEL", "endwhile", genStackTop(whileStack));

         genStackPop(whileStack);


         // - Generating

         // Calls for a next token
         next();

         return;


      // <statement> -> Return <expression>
      case t_return:

         // If we're in scope, return means error
         if (scope) {
            freeAndError(error_return_scope);
         }

         // Calls for a next token
         next();

         // Expression handling
         typeExpr = expression(token, localTable);

         // We search for return type of function
         if ((var = symtabFind(localTable, "return")) == NULL ) {
            freeAndError(error_return_scope);
         }
         
         // Checks return rypes
         if ((typeExpr == ex_integer || typeExpr == ex_double) && (var->data_type == TInteger || var->data_type == TDouble) ) {
            // It is ok.
         } else if (typeExpr == ex_string && var->data_type == TString) {
            // It is ok.
         } else {
            // Other types are errors
            freeAndError(error_incompatible_type);
         }

         // + Generating

         if (typeExpr == ex_integer && var->data_type == TDouble) {
            instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
         } else if (typeExpr == ex_double && var->data_type == TInteger) {
            instrListAppend(&globalInstrList, "FLOAT2INTS", NULL, NULL, NULL);
         }

         instrListAppend(&globalInstrList, "POPS", "LF@%retval", NULL, NULL);
         instrListAppend(&globalInstrList, "POPFRAME", NULL, NULL, NULL);
         instrListAppend(&globalInstrList, "RETURN", NULL, NULL, NULL);

         // - Generating
         
         return;


      // <statement> -> Input id
      case t_input:

         // Checks whether next token is identifier
         getAndCheck(t_identifier, error_identifier);

         // Checks whether local variable to store input is declared
         if (symtabFind(localTable, token->data) == NULL ) {
            freeAndError(error_undeclared);
         }

         // + Generating

         // Preparing first argument
         addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(token->data) + 1));

         if (scope) {
            strcpy(addr1, "GF@");
         } else {
            strcpy(addr1, "LF@");
         }

         strcat(addr1, token->data);

         // Preparing second argument

         // "double" or "string" are two longest
         addr2 = malloc(sizeof(char) * (strlen("double") + 1));


         // Decide which datatype to input
         switch (symtabFind(localTable, token->data)->data_type) {
            case TInteger:
               strcpy(addr2, "int");
               break;

            case TDouble:
               strcpy(addr2, "float");
               break;

            case TString:
               strcpy(addr2, "string");
               break;

            default:
               break;
         }

         instrListAppend(&globalInstrList, "READ", addr1, addr2, NULL);

         //free(addr1);
         //free(addr2);

         // - Generating

         // Calls for a next token
         next();

         return;


      // <statement> -> Print <print>
      case t_print:

         // Calls for a next token
         next();

         // Print handling
         print(scope);

         return;

      // <statememt> -> Dim id As Type <inicialization>
      case t_dim:
         
         // Checks whether next token is an identifier
         getAndCheck(t_identifier, error_identifier);

         // Checks if already declared
         if (symtabFind(localTable, token->data) != NULL || symtabFind(globalTable, token->data) != NULL) {
            freeAndError(error_redeclaration);
         }

         // Adding into local table
         if ((var = symtabLookAdd(localTable, token->data)) == NULL) {
            freeAndError(error_internal);
         }

         // + Generating
         
         // Preparing first argument
         addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(token->data) + 1));

         if (scope) {
            strcpy(addr1, "GF@");
         } else {
            strcpy(addr1, "LF@");
         }

         strcat(addr1, token->data);
         instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);

         //free(addr1);

         // - Generating

         // Checks whether next token is "As"
         getAndCheck(t_as, error_as);

         // Calls for a next token
         next();

         // Checks return type
         typeId = decodeType();
         if (typeId == Unknown) {
            freeAndError( error_type );
         }

         // Add type into localtable
         if (symtabAddType(localTable, var->name, typeId) == NULL) {
            freeAndError(error_internal);
         }

         // Calls for a next token
         next();

         // Inicialization handler
         inicialization(scope, var, typeId);

         return;


      // <statement> -> id = <assignment>
      case t_identifier:

         // Checks whether our variable is declared
         var = symtabFind(localTable, token->data);
         if (var == NULL) {
            freeAndError(error_undeclared);
         }
         
         // Checks whether next token is "="
         getAndCheck(t_eq, error_assignment);
         
         // Calls for a next token -> we'll know whether we assign function or expression
         next();

         // Checks if we got bultin funciton
         if (token->type == t_length || token->type == t_substr || token->type == t_asc || token->type == t_chr) {
            switch (token->type) {
               case t_length:
                  fce = symtabFind(globalTable, "length");
                  break;
               case t_substr:
                  fce = symtabFind(globalTable, "substr");
                  break;
               case t_asc:
                  fce = symtabFind(globalTable, "asc");
                  break;
               default:
                  fce = symtabFind(globalTable, "chr");
            }

            if (fce == NULL) {
               freeAndError(error_internal);
            }

            // Check return type matches variable
            if ((fce->data_type == TString && var->data_type == TString)
                  || ((fce->data_type == TInteger || fce->data_type == TDouble)
                     && (var->data_type == TInteger || var->data_type == TDouble))) {
               // It is ok...
            } else {
               freeAndError(error_incompatible_type);
            }

            // Checks whether next token is left bracket
            getAndCheck(t_l_bracket, error_parenthesis_left);

            // + Generating

            instrListAppend(&globalInstrList, "CREATEFRAME", NULL, NULL, NULL);

            // - Generating

            // Arguments handler
            arguments(scope, fce);

            // + Generating

            instrListAppendInstrLabel(&globalInstrList, "CALL", fce->name, -1);

            // Assigning
            addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(var->name) + 1));

            if (scope) {
               strcpy(addr1, "GF@"); 
            } else {
               strcpy(addr1, "LF@"); 
            }

            strcat(addr1, var->name);

            if (fce->data_type == TInteger && var->data_type == TDouble) {
               instrListAppend(&globalInstrList, "INT2FLOAT", addr1, "TF@%retval", NULL);
            } else if (fce->data_type == TDouble && var->data_type == TInteger) {
               instrListAppend(&globalInstrList, "FLOAT2R2EINT", addr1, "TF@%retval", NULL);
            } else {
               instrListAppend(&globalInstrList, "MOVE", addr1, "TF@%retval", NULL);
            }

            //free(addr1);

            // - Generating

            // Calls for a next token
            next();

            return;
         }

         // <asisignment> -> id ( <arguments> ) ..
         if (token->type == t_identifier && (fce = symtabFind(globalTable, token->data)) != NULL) { // je to funkce

            // Checks whether next token is left bracket
            getAndCheck(t_l_bracket, error_parenthesis_left);
            
            // Check return type matches variable 
            if ((fce->data_type == TString && var->data_type == TString)
                  || ((fce->data_type == TInteger || fce->data_type == TDouble)
                     && (var->data_type == TInteger || var->data_type == TDouble))) {
               // It is ok...
            } else {
               freeAndError(error_incompatible_type);
            }

            // + Generating

            instrListAppend(&globalInstrList, "CREATEFRAME", NULL, NULL, NULL);

            // - Generating

            // Arguments handler
            arguments(scope, fce);

            // + Generating

            instrListAppendInstrLabel(&globalInstrList, "CALL", fce->name, -1);

            // Assigning 
            addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(var->name) + 1));
            
            if (scope) {
               strcpy(addr1, "GF@");
            } else {
               strcpy(addr1, "LF@");
            }

            strcat(addr1, var->name);


            // TODO - zde to Dane prosim zkontroluj, driv tu bylo jen to v else, ale doplnil jsem jeste ty dve konverze
            // TODO - mozna by neco podobneho melo byt i u vestavenych funkci
            if (fce->data_type == TInteger && var->data_type == TDouble) {
               instrListAppend(&globalInstrList, "INT2FLOAT", addr1, "TF@%retval", NULL);
            } else if (fce->data_type == TDouble && var->data_type == TInteger) {
               instrListAppend(&globalInstrList, "FLOAT2R2EINT", addr1, "TF@%retval", NULL);
            } else {
               instrListAppend(&globalInstrList, "MOVE", addr1, "TF@%retval", NULL);
            }

            //free(addr1);

            // - Generating

            // Calls for a next token
            next();

            return;
         } else { // <assignment> -> <expression>

            // Expression handler
            typeExpr = expression(token, localTable);
            
            // Variable type checking
            if ((typeExpr == ex_integer || typeExpr == ex_double) && (var->data_type == TInteger || var->data_type == TDouble)) {

               if ((typeExpr == ex_integer && var->data_type == TInteger) ||
                   (typeExpr == ex_double && var->data_type == TDouble)) {
                  // No need to convert

                  // + Generating

                  // Preparing for storing value
                  
                  addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(var->name) + 1));

                  if (scope) {
                     strcpy(addr1, "GF@"); 
                  } else {
                     strcpy(addr1, "LF@");
                  }

                  strcat(addr1, var->name);

                  instrListAppend(&globalInstrList, "POPS", addr1, NULL, NULL);

                  //free(addr1);

                  // - Generating
               
               } else if (typeExpr == ex_integer && var->data_type == TDouble) {
                  
                  // Need to convert stack value from int to double

                  // + Generating

                  // Convert on stack

                  instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);

                  // Prepare for storing value

                  addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(var->name) + 1));

                  if (scope) {
                     strcpy(addr1, "GF@");
                  } else {
                     strcpy(addr1, "LF@"); 
                  }

                  strcat(addr1, var->name);

                  instrListAppend(&globalInstrList, "POPS", addr1, NULL, NULL);

                  // free(addr1);

                  // - Generating

               
               } else if (typeExpr == ex_double && var->data_type == TInteger) {

                  // Need to convert stackvalue from double to int

                  // + Generating

                  // Convert on stack

                  instrListAppend(&globalInstrList, "FLOAT2R2EINTS", NULL, NULL, NULL);

                  // Prepare for storing value

                  addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(var->name) + 1));

                  if (scope) {
                     strcpy(addr1, "GF@"); 
                  } else {
                     strcpy(addr1, "LF@"); 
                  }

                  strcat(addr1, var->name);

                  instrListAppend(&globalInstrList, "POPS", addr1, NULL, NULL);

                  // free(addr1);

                  // - Generating
                   
               }
               

               return;

            } else if (typeExpr == ex_string && var->data_type == TString) {

               // No need to convert

               // + Generating

               // Preparing for storing value

               addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(var->name) + 1));

               if (scope) {
                  strcpy(addr1, "GF@"); 
               } else {
                  strcpy(addr1, "LF@"); 
               }

               strcat(addr1, var->name);
               
               instrListAppend(&globalInstrList, "POPS", addr1, NULL, NULL);

               //free(addr1);

               // - Generating

               return;
            } else {
               // Other combinations are incorrect
               freeAndError(error_incompatible_type);
            }
         }
         break;

      default:
         freeAndError(error_unknown_syntax);
   }
}

/**
 * @brief Function represents first part for rule of printing
 *      1. <print> -> <expression>; <print2>
 */
void print(bool scope) {

   // Expression handler
   exprType typeExpr = expression(token, localTable);
   
   // Can't be relation operator
   if (typeExpr == ex_bool) {
      freeAndError(error_incompatible_type);
   }

   // + Generating

   // Get value from stack and print
   if (scope) {
      instrListAppend(&globalInstrList, "POPS", "GF@%tmp3", NULL, NULL);
      instrListAppend(&globalInstrList, "WRITE", "GF@%tmp3", NULL, NULL);
   } else {
      instrListAppend(&globalInstrList, "POPS", "LF@%tmp3", NULL, NULL); 
      instrListAppend(&globalInstrList, "WRITE", "LF@%tmp3", NULL, NULL);
   }

   // - Generating

   // Checks whether next token is a semicolon
   check(t_semicolon, error_semicolon);

   // Calls for a new token
   next();

   // Calls the second part of rule
   print2(scope);

   return;
}

/**
 * @brief Function represents second part for rule of printting
 *      1. <print2> -> <print>
 *      2. <print2> -> epsilon
 */
void print2(bool scope) {

   // Use different rules - dependent on type of a token
   switch (token->type) {

      // <print2> -> epsilon ... dosli jsme na konce radku
      case t_eol:
         return;

      // <print2> -> <print>
      default:
         print(scope);
         return;
      }
}


/**
 * @brief Function represents firs part of rule for calling function.
 *      1. <arguments> -> TERM <arguments2>
 *      2. <arguments> -> epsilon
 *
 * @param scope        bool   Says whether function is called from scope or not
 * @param fce htab_listitem   Pointer to record of function
 */
void arguments(bool scope, htab_listitem *fce) {

   // Temporary pointer of record
   htab_listitem *var;

   // Calls for a new token
   next();

   // Checks whether we got different term (and if we wanted that term)
   if (token->type == t_identifier || token->type == t_int || token->type == t_real || token->type == t_literal) {

      if (fce->params == NULL) { // funkce nema parametry
         freeAndError(error_too_many_arg);
      }

      if (token->type == t_identifier) {

         // Searches if variable is declared in local table
         var = symtabFind(localTable, token->data);

         // Variable is undeclared
         if (var == NULL) {
            freeAndError( error_undeclared );

         } else if (var->data_type == fce->params->data_type) {

            // They're same data type

            // + Generating

            // Prepare first argument
            addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(fce->params->name) + 1));
            strcpy(addr1, "TF@");
            strcat(addr1, fce->params->name);

            // Prepare second argument
            addr2 = malloc(sizeof(char) * (strlen("XF@") + strlen(token->data) + 1));
            if (scope) {
               strcpy(addr2, "GF@"); 
            } else {
               strcpy(addr2, "LF@"); 
            }
            strcat(addr2, token->data);

            // Declaration and setting of first argument
            instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
            instrListAppend(&globalInstrList, "MOVE", addr1, addr2, NULL);

            //free(addr1)
            //free(addr2)

            // - Generating

            // Arguments handling
            arguments2(scope, fce, fce->params->next );


         } else if ((var->data_type == TInteger && fce->params->data_type == TDouble) ||
                    ( var->data_type == TDouble && fce->params->data_type == TInteger)) { 

            // They're same data type after conversion -> ok

            // Need to convert var to double
            if (var->data_type == TInteger && fce->params->data_type == TDouble) {

               // + Generating
               
               // Prepare arguments for conversion
               addr2 = malloc(sizeof(char) * (strlen("XF@") + strlen(token->data) + 1));

               if (scope) {

                  strcpy(addr2, "GF@");
                  strcat(addr2, token->data);
                  instrListAppend(&globalInstrList, "INT2FLOAT", "GF@%tmp2", addr2, NULL);

               } else {
                  
                  strcpy(addr2, "LF@");
                  strcat(addr2, token->data);
                  instrListAppend(&globalInstrList, "INT2FLOAT", "LF@%tmp2", addr2, NULL);

               }

               //free(addr2);

               // Prepare first argument for defining and initializing argument
               addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(fce->params->name) + 1));
               strcpy(addr1, "TF@");
               strcat(addr1, fce->params->name);

               // Declaration and setting first argument
               if (scope) {

                  instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
                  instrListAppend(&globalInstrList, "MOVE", addr1, "GF%tmp2", NULL);

               } else {

                  instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
                  instrListAppend(&globalInstrList, "MOVE", addr1, "LF%tmp2", NULL);

               }

               //free(addr1);

               // - Generating

            }

            // Need to convert var to integer
            if (var->data_type == TDouble && fce->params->data_type == TInteger) {

               // + Generating
               
               // Prepare argument for conversion
               addr2 = malloc(sizeof(char) * (strlen("XF@") + strlen(token->data) + 1));

               if (scope) {

                  strcpy(addr2, "GF@");
                  strcat(addr2, token->data);
                  // TODO opravdu na sudou?
                  instrListAppend(&globalInstrList, "FLOAT2R2EINT", "GF@%tmp2", addr2, NULL);

               } else {

                  strcpy(addr2, "LF@");
                  strcat(addr2, token->data);
                  // TODO opravdu na sudou?
                  instrListAppend(&globalInstrList, "FLOAT2R2EINT", "LF@%tmp2", addr2, NULL);

               }

               //free(addr2);

               // Prepare first argument for defining and initializing argument
               addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(fce->params->name) + 1));
               strcpy(addr1, "TF@");
               strcat(addr1, fce->params->name);

               // Declaration and setting first argument
               if (scope) {

                  instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
                  instrListAppend(&globalInstrList, "MOVE", addr1, "GF@%tmp2", NULL);

               } else {

                  instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
                  instrListAppend(&globalInstrList, "MOVE", addr1, "LF@%tmp2", NULL);

               }

               //free(addr1);

               // - Generating
            
            }

            // Arguments handling
            arguments2(scope, fce, fce->params->next);

         } else {


            // Otherwise wrong type
            freeAndError(error_incompatible_type);

         }


      } else { // Otherwise constant

         if (token->type == t_int && fce->params->data_type == TInteger) {

            // + Generating

            // Prepare first argument
            addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(fce->params->name) + 1));
            strcpy(addr1, "TF@");
            strcat(addr1, fce->params->name);

            // Prepare second argument
            addr2 = malloc(sizeof(char) * (strlen("int@") + strlen(token->data) + 1));
            strcpy(addr2, "int@");
            strcat(addr2, token->data);

            // Declaration and setting of first argument
            instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
            instrListAppend(&globalInstrList, "MOVE", addr1, addr2, NULL);

            //free(addr1)
            //free(addr2)
            
            // - Generating

            arguments2(scope, fce, fce->params->next);

         } else if (token->type == t_real && fce->params->data_type == TDouble) {

            // + Generating

            // Prepare first argument
            addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(fce->params->name) + 1));
            strcpy(addr1, "TF@");
            strcat(addr1, fce->params->name);

            // Prepare second argument
            addr2 = malloc(sizeof(char) * (strlen("float@") + strlen(token->data) + 1));
            strcpy(addr2, "float@");
            strcat(addr2, token->data);

            // Declaration and setting of first argument
            instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
            instrListAppend(&globalInstrList, "MOVE", addr1, addr2, NULL);

            //free(addr1)
            //free(addr2)

            // - Generating

            arguments2(scope, fce, fce->params->next);

         } else if (token->type == t_int && fce->params->data_type == TDouble) {

            // Need to convert types - int to double

            // + Generating

            // Prepare arguments for converting
            addr2 = malloc(sizeof(char) * (strlen("int@") + strlen(token->data) + 1));
            strcpy(addr2, "int@");
            strcat(addr2, token->data);

            // Convert
            if (scope) {
               instrListAppend(&globalInstrList, "INT2FLOAT", "GF@%tmp2", addr2, NULL);
            } else {
               instrListAppend(&globalInstrList, "INT2FLOAT", "LF@%tmp2", addr2, NULL);
            }

            //free(addr2);

            // Prepare first argument for next action
            addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(fce->params->name) + 1));
            strcpy(addr1, "TF@");
            strcat(addr1, fce->params->name);

            // Declare and move
            if (scope) {
               instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
               instrListAppend(&globalInstrList, "MOVE", addr1, "GF@%tmp2", NULL);
            } else {
               instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
               instrListAppend(&globalInstrList, "MOVE", addr1, "LF@%tmp2", NULL);
            }

            //free(addr1);

            // - Generating

            arguments2(scope, fce, fce->params->next);
         
         } else if (token->type == t_real && fce->params->data_type == TInteger) {

            // Need to convert types - double to int

            // + Generating

            // Prepare arguments for converting 
            addr2 = malloc(sizeof(char) * (strlen("float@") + strlen(token->data) + 1));
            strcpy(addr2, "float@");
            strcat(addr2, token->data);

            // Convert
            if (scope) {
               instrListAppend(&globalInstrList, "FLOAT2R2EINT", "GF@%tmp2", addr2, NULL);
            } else {
               instrListAppend(&globalInstrList, "FLOAT2R2EINT", "LF@%tmp2", addr2, NULL);
            }

            //free(addr2);
            
            // Prepare first argument for next action
            addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(fce->params->name) + 1));
            strcpy(addr1, "TF@");
            strcat(addr1, fce->params->name);

            // Decare and move
            if (scope) {
               instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
               instrListAppend(&globalInstrList, "MOVE", addr1, "GF@%tmp2", NULL);
            } else {
               instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
               instrListAppend(&globalInstrList, "MOVE", addr1, "LF@%tmp2", NULL);
            }

            //free(addr1);

            // - Generating

            arguments2(scope, fce, fce->params->next);
         
         } else if (token->type == t_literal && fce->params->data_type == TString) {

            // + Generating

            // Prepare first argument
            addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(fce->params->name) + 1));
            strcpy(addr1, "TF@");
            strcat(addr1, fce->params->name);

            // Prepare second argument
            addr2 = malloc(sizeof(char) * (strlen("string@") + strlen(token->data) + 1));
            strcpy(addr2, "string@");
            strcat(addr2, token->data);

            // Declaration and setting of first argument
            instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
            instrListAppend(&globalInstrList, "MOVE", addr1, addr2, NULL);

            //free(addr1)
            //free(addr2)

            // - Generating

            arguments2(scope, fce, fce->params->next);

         } else {
            // Otherwise wrong parameter type
            freeAndError(error_incompatible_type);
         }
      }
      return;
   }

   // We got something else
   // TODO Jake budeme hazet chyby?
   if (token->type == t_r_bracket) {

      // Close bracket, but more declared parameters
      if (fce->params != NULL) {
         freeAndError(error_arguments_missing);
      } else {
         return;
      }

   } else {
      freeAndError(error_call_function);
   }

}

/*
 * @brief Function represents second part of rule for calling functions:
 *      1. <arguments2> -> , TERM <arguments2>
 *      2. <arguments2> -> epsilon
 *
 * @param scope         bool  says whether function is called from scope or not
 * @param fce  htab_listitem  pointer to record of function
 * @param par  TParam         pointer to currently handling parameter
 */
void arguments2(bool scope, htab_listitem *fce, TParam *par ) {

   // Temporary pointer for function record
   htab_listitem *var;

   // Call for a next token
   next();

   // Right bracket -> check if declared more parameters
   if (token->type == t_r_bracket) {
      if (par != NULL) {
         freeAndError(error_arguments_missing);
      } else {
         return;
      }
   }

   // Expecting commaa
   check(t_comma, error_comma);

   // Call for a next token
   next();
   
   // zjistime, jestli prisel term a jestli jsme ho chteli
   if (token->type == t_identifier || token->type == t_int || token->type == t_real || token->type == t_literal) {
      
      if ( par == NULL ) { // nebyly uz deklarovane dalsi parametry
         freeAndError( error_too_many_arg );
      }

      // Checks identifiers declaration and type
      if (token->type == t_identifier) {

         // Searches in local variable table
         var = symtabFind(localTable, token->data);

         // Undeclared variable
         if (var == NULL) {
            freeAndError(error_undeclared);

         } else if (var->data_type == par->data_type) {
            // Same data types

            // + Generating

            // Prepare first argument
            addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(par->name) + 1));
            strcpy(addr1, "TF@");
            strcat(addr1, par->name);

            // Prepare second argument
            addr2 = malloc(sizeof(char) * (strlen("XF@") + strlen(token->data) + 1));
            if (scope) {
               strcpy(addr2, "GF@"); 
            } else {
               strcpy(addr2, "LF@"); 
            }
            strcat(addr2, token->data);

            // Declaration and setting of first argument
            instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
            instrListAppend(&globalInstrList, "MOVE", addr1, addr2, NULL);

            //free(addr1)
            //free(addr2)

            // - Generating

            arguments2(scope, fce, par->next);

         } else if ((var->data_type == TInteger && par->data_type == TDouble) ||
                    (var->data_type == TDouble && par->data_type == TInteger)) { 

            // Same types after conversion

            // Need to convert var to double
            if (var->data_type == TInteger && par->data_type == TDouble) {

               // + Generating
               
               // Prepare arguments for conversion
               addr2 = malloc(sizeof(char) * (strlen("XF@") + strlen(token->data) + 1));

               if (scope) {

                  strcpy(addr2, "GF@");
                  strcat(addr2, token->data);
                  instrListAppend(&globalInstrList, "INT2FLOAT", "GF@%tmp2", addr2, NULL);

               } else {
                  
                  strcpy(addr2, "LF@");
                  strcat(addr2, token->data);
                  instrListAppend(&globalInstrList, "INT2FLOAT", "LF@%tmp2", addr2, NULL);

               }

               //free(addr2);

               // Prepare first argument for defining and initializing argument
               addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(par->name) + 1));
               strcpy(addr1, "TF@");
               strcat(addr1, par->name);

               // Declaration and setting first argument
               if (scope) {

                  instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
                  instrListAppend(&globalInstrList, "MOVE", addr1, "GF%tmp2", NULL);

               } else {

                  instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
                  instrListAppend(&globalInstrList, "MOVE", addr1, "LF%tmp2", NULL);

               }

               //free(addr1);

               // - Generating

            }

            // Need to convert var to integer
            if (var->data_type == TDouble && par->data_type == TInteger) {

               // + Generating
               
               // Prepare argument for conversion
               addr2 = malloc(sizeof(char) * (strlen("XF@") + strlen(token->data) + 1));

               if (scope) {

                  strcpy(addr2, "GF@");
                  strcat(addr2, token->data);
                  // TODO opravdu na sudou?
                  instrListAppend(&globalInstrList, "FLOAT2R2EINT", "GF@%tmp2", addr2, NULL);

               } else {

                  strcpy(addr2, "LF@");
                  strcat(addr2, token->data);
                  // TODO opravdu na sudou?
                  instrListAppend(&globalInstrList, "FLOAT2R2EINT", "LF@%tmp2", addr2, NULL);

               }

               //free(addr2);

               // Prepare first argument for defining and initializing argument
               addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(par->name) + 1));
               strcpy(addr1, "TF@");
               strcat(addr1, par->name);

               // Declaration and setting first argument
               if (scope) {

                  instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
                  instrListAppend(&globalInstrList, "MOVE", addr1, "GF@%tmp2", NULL);

               } else {

                  instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
                  instrListAppend(&globalInstrList, "MOVE", addr1, "LF@%tmp2", NULL);

               }

               //free(addr1);

               // - Generating
            
            }


            arguments2(scope, fce, par->next);

         } else {

            // Wrong argument type
            freeAndError(error_incompatible_type);
         }

      } else { // Otherwise constant

         if (token->type == t_int && par->data_type == TInteger) {

            // + Generating

            // Prepare first argument
            addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(par->name) + 1));
            strcpy(addr1, "TF@");
            strcat(addr1, par->name);

            // Prepare second argument
            addr2 = malloc(sizeof(char) * (strlen("int@") + strlen(token->data) + 1));
            strcpy(addr2, "int@");
            strcat(addr2, token->data);

            // Declaration and setting of first argument
            instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
            instrListAppend(&globalInstrList, "MOVE", addr1, addr2, NULL);

            //free(addr1)
            //free(addr2)
            
            // - Generating

            arguments2(scope, fce, par->next);

         } else if (token->type == t_real && par->data_type == TDouble) {

            // + Generating

            // Prepare first argument
            addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(par->name) + 1));
            strcpy(addr1, "TF@");
            strcat(addr1, par->name);

            // Prepare second argument
            addr2 = malloc(sizeof(char) * (strlen("float@") + strlen(token->data) + 1));
            strcpy(addr2, "float@");
            strcat(addr2, token->data);

            // Declaration and setting of first argument
            instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
            instrListAppend(&globalInstrList, "MOVE", addr1, addr2, NULL);

            //free(addr1)
            //free(addr2)

            // - Generating

            arguments2(scope, fce, par->next);

         } else if (token->type == t_int && par->data_type == TDouble) {

            // Need to convert types - int to double

            // + Generating

            // Prepare arguments for converting
            addr2 = malloc(sizeof(char) * (strlen("int@") + strlen(token->data) + 1));
            strcpy(addr2, "int@");
            strcat(addr2, token->data);

            // Convert
            if (scope) {
               instrListAppend(&globalInstrList, "INT2FLOAT", "GF@%tmp2", addr2, NULL);
            } else {
               instrListAppend(&globalInstrList, "INT2FLOAT", "LF@%tmp2", addr2, NULL);
            }

            //free(addr2);

            // Prepare first argument for next action
            addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(par->name) + 1));
            strcpy(addr1, "TF@");
            strcat(addr1, par->name);

            // Declare and move
            if (scope) {
               instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
               instrListAppend(&globalInstrList, "MOVE", addr1, "GF@%tmp2", NULL);
            } else {
               instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
               instrListAppend(&globalInstrList, "MOVE", addr1, "LF@%tmp2", NULL);
            }

            //free(addr1);

            // - Generating

            arguments2(scope, fce, par->next);
         
         } else if (token->type == t_real && par->data_type == TInteger) {

            // Need to convert types - double to int

            // + Generating

            // Prepare arguments for converting 
            addr2 = malloc(sizeof(char) * (strlen("float@") + strlen(token->data) + 1));
            strcpy(addr2, "float@");
            strcat(addr2, token->data);

            // Convert
            if (scope) {
               instrListAppend(&globalInstrList, "FLOAT2R2EINT", "GF@%tmp2", addr2, NULL);
            } else {
               instrListAppend(&globalInstrList, "FLOAT2R2EINT", "LF@%tmp2", addr2, NULL);
            }

            //free(addr2);
            
            // Prepare first argument for next action
            addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(par->name) + 1));
            strcpy(addr1, "TF@");
            strcat(addr1, par->name);

            // Decare and move
            if (scope) {
               instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
               instrListAppend(&globalInstrList, "MOVE", addr1, "GF@%tmp2", NULL);
            } else {
               instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
               instrListAppend(&globalInstrList, "MOVE", addr1, "LF@%tmp2", NULL);
            }

            //free(addr1);

            // - Generating

            arguments2(scope, fce, par->next);
         
         } else if (token->type == t_literal && par->data_type == TString) {

            // + Generating

            // Prepare first argument
            addr1 = malloc(sizeof(char) * (strlen("TF@") + strlen(par->name) + 1));
            strcpy(addr1, "TF@");
            strcat(addr1, par->name);

            // Prepare second argument
            addr2 = malloc(sizeof(char) * (strlen("string@") + strlen(token->data) + 1));
            strcpy(addr2, "string@");
            strcat(addr2, token->data);

            // Declaration and setting of first argument
            instrListAppend(&globalInstrList, "DEFVAR", addr1, NULL, NULL);
            instrListAppend(&globalInstrList, "MOVE", addr1, addr2, NULL);

            //free(addr1)
            //free(addr2)

            // - Generating

            arguments2(scope, fce, par->next);

         } else {
            // Otherwise wrong parameter type
            freeAndError(error_incompatible_type);
         }


         /* OLD
         if (( token->type == t_int || token->type == t_real) && (par->data_type == TInteger || par->data_type == TDouble )) {
            arguments2(scope, fce, par->next);

         } else if (token->type == t_literal && par->data_type == TString) {

            arguments2(scope, fce, par->next);

         } else {
            // Wrong parameter type
            freeAndError(error_incompatible_type);
         }
         */
      }
      return;
   }

   // Syntax error
   freeAndError(error_call_function);

}

/*
 * @brief Function represents rules for initialization of values during declaration of variable.
 *    1. <inicialization> -> = <expression>
 *    2. <inicialization> -> epsilon
 *
 * @param typeId  Type_data   data type of variable
 */
void inicialization(bool scope, htab_listitem* var, Type_data typeId) {
   
   // Expression type
   exprType typeExpr;

   // Use different rules - dependent on type of a token
   switch (token->type) {

      // <inicialization> -> = <expression>
      case t_eq:

         // Calls for a next token
         next();

         // Expression handler
         typeExpr = expression(token, localTable);
         
         // Check initialized value and type
         if (typeId == TString && typeExpr == ex_string) {

            // + Generating

            // preparing for storing value

            addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(var->name) + 1));

            if (scope) {
               strcpy(addr1, "GF@"); 
            } else {
               strcpy(addr1, "LF@"); 
            }

            strcat(addr1, var->name);

            instrListAppend(&globalInstrList, "POPS", addr1, NULL, NULL);

            //free(addr1);

            // - Generating

            return;

         } else if ((typeId == TInteger || typeId == TDouble) && (typeExpr == ex_integer || typeExpr == ex_double)) {

            if ((typeId == TInteger && typeExpr == ex_integer) || (typeId == TDouble && typeExpr == ex_double)) {

               // No need to convert

               // + Generating
               
               // Preparing for storing value

               addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(var->name) + 1));
               
               if (scope) {
                  strcpy(addr1, "GF@"); 
               } else {
                  strcpy(addr1, "LF@"); 
               }

               strcat(addr1, var->name);

               instrListAppend(&globalInstrList, "POPS", addr1, NULL, NULL);

               //free(addr1);

               // - Generating
            
            } else if (typeId == TInteger && typeExpr == ex_double) {

               // Need to convert double on stack to integer

               // + Generating

               // Convert

               instrListAppend(&globalInstrList, "FLOAT2R2EINTS", NULL, NULL, NULL);

               // Preparing for storing value

               addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(var->name) + 1));
               
               if (scope) {
                  strcpy(addr1, "GF@"); 
               } else {
                  strcpy(addr1, "LF@"); 
               }

               strcat(addr1, var->name);

               instrListAppend(&globalInstrList, "POPS", addr1, NULL, NULL);

               //free(addr1);

               // - Generating
            
            } else if (typeId == TDouble && typeExpr == ex_integer) {

               // Need to convert int on stack to double

               // + Generating

               // Convert

               instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);

               // Preparing for storing value

               addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(var->name) + 1));
               
               if (scope) {
                  strcpy(addr1, "GF@"); 
               } else {
                  strcpy(addr1, "LF@"); 
               }

               strcat(addr1, var->name);

               instrListAppend(&globalInstrList, "POPS", addr1, NULL, NULL);

               //free(addr1);

               // - Generating
            
            }

            return;

         } else {
            freeAndError(error_incompatible_type);
         }


         return;

      // <inicialization> -> epsilon ...  evalueated in callers function
      default:

         // + Generating

         // Preaparing for default initialization

         addr1 = malloc(sizeof(char) * (strlen("XF@") + strlen(var->name) + 1));

         if (scope) {
            strcpy(addr1, "GF@");
         } else {
            strcpy(addr1, "LF@"); 
         }

         strcat(addr1, var->name);

         if (var->data_type == TInteger) {
            instrListAppend(&globalInstrList, "MOVE", addr1, "int@0", NULL); 
         } else if (var->data_type == TDouble) {
            instrListAppend(&globalInstrList, "MOVE", addr1, "float@0.0", NULL); 
         } else if (var->data_type == TString) {
            instrListAppend(&globalInstrList, "MOVE", addr1, "string@", NULL); 
         }

         //free(addr1);

         // - Generating

         return;
   }  
}

/*
 * @brief Function represents rules for else:
 *    1. <elsif> -> end if
 *    2. <elsif> -> elsif <expression> then EOL <statementList> <elsif>
 *    3. <elsif> -> else EOL STL end if
 * @brief Funkce reprezentujici pravidla pro vetev else u podminky:
 *    1. <elsif> -> end if
 *    2. <elsif> -> elsif <expression> then EOL <statementList> <elsif>
 *    3. <elsif> -> else EOL STL end if
 */
void elsif(bool scope) {

   // Use different rules - dependent on type of a token
   switch (token->type) {

      // <elsif> -> end if
      case t_end:
         
         // Checks whether next token is "If"
         getAndCheck(t_if, error_if);

         return;

      // <elsif> -> elsif <expression> then EOL <statementList> <elsif>
      case t_elseif:

         // Call for a new token
         next();

         // TODO - tady mozna bude potreba kontrolovat navratovou hodnotu
         expression(token, localTable);

         // Checks whether our token is "Then"
         check(t_then, error_then);

         // Checks whether next token is EOL
         getAndCheck(t_eol, error_eol);

         // Calls for a new token
         next();
         
         // Statement handler
         statementList(scope);
         
         // Recursive clal to elsif
         elsif(scope);

         return;


      // <elsif> -> else EOL STL end if
      case t_else:

         // Checks whether next token is EOL
         getAndCheck(t_eol, error_eol);

         // Calls for a new token
         next();

         // Statement handler
         statementList(scope);

         // Checks whether our token is "End"
         check(t_end, error_end);

         // Checks whether next token is "If"
         getAndCheck(t_if, error_if);
         return;

      default:
         // Syntax error
         freeAndError(error_unknown_syntax);
   }

}


/*
 * @brief Main function of syntactical analysis.
 *        Inicializes global table and fills in builtin functions.
 *        Than it calls function for recursive descent
 */
void parser() {

   // Allocates global table for function storing
   globalTable = symtabInit(TABLE_SIZE);

   // Initialization and allocation of stacks
   whileStack = genStackInit();
   ifStack = genStackInit();

   // Malloc error
   if (globalTable == NULL) {
      throwError(error_internal);
   }

   // Global instruction list initialization
   instrListInit(&globalInstrList);

   // Temporary pointer to function record in the global table
   htab_listitem *fce;  

   // Adding builtin functions into the table

   // Length( s As String ) as Integer
   if ((fce = symtabLookAdd( globalTable, "length")) == NULL       ||
       symtabAddParam(globalTable, "length", TString, "s") == NULL ||
       symtabAddType(globalTable, "length", TInteger) == NULL) {

      freeAndError(error_internal);
   }

   fce->is_defined = true;

   // SubStr( s As String, i As Integer, n As Integer ) as String
   if ((fce = symtabLookAdd( globalTable, "substr")) == NULL          ||
        symtabAddParam(globalTable, "substr", TString, "s") == NULL   ||
        symtabAddParam(globalTable, "substr", TInteger, "i") == NULL  ||
        symtabAddParam(globalTable, "substr", TInteger, "n") == NULL  ||
        symtabAddType(globalTable, "substr", TString) == NULL) {

      freeAndError( error_internal );
   }

   fce->is_defined = true;

   // Asc( s As String, i As Integer ) as Integer
   if ((fce = symtabLookAdd(globalTable, "asc")) == NULL              ||
        symtabAddParam(globalTable, "asc", TString, "s") == NULL      ||
        symtabAddParam(globalTable, "asc", TInteger, "i") == NULL     ||
        symtabAddType(globalTable, "asc", TInteger) == NULL) { 

      freeAndError( error_internal );
   }

   fce->is_defined = true;

   // Chr( i As Integer ) As String
   if ((fce = symtabLookAdd( globalTable, "chr")) == NULL             ||
        symtabAddParam(globalTable, "chr", TInteger, "i") == NULL     ||
        symtabAddType(globalTable, "chr", TString) == NULL) {

      freeAndError( error_internal );
   }

   fce->is_defined = true;

   // + Generating

   instrListAppend(&globalInstrList, ".IFJcode17", NULL, NULL, NULL);
   instrListAppendInstrLabel(&globalInstrList, "JUMP", "scope", -1);

   // Built-in functions

   // Length(s as string) as integer
   instrListAppendInstrLabel(&globalInstrList, "LABEL", "length", -1);
   instrListAppend(&globalInstrList, "PUSHFRAME", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "DEFVAR", "LF@%retval", NULL, NULL);
   instrListAppend(&globalInstrList, "MOVE", "LF@%retval", "int@0", NULL);
   instrListAppend(&globalInstrList, "STRLEN", "LF@%retval", "LF@s", NULL);
   instrListAppend(&globalInstrList, "POPFRAME", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "RETURN", NULL, NULL, NULL);

   // SubStr(s as string, i as integer, n as integer) as string
   instrListAppendInstrLabel(&globalInstrList, "LABEL", "substr", -1);
   instrListAppend(&globalInstrList, "PUSHFRAME", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "DEFVAR", "LF@%retval", NULL, NULL);
   instrListAppend(&globalInstrList, "MOVE", "LF@%retval", "string@", NULL);

   // if (i < 1) return ""
   instrListAppend(&globalInstrList, "PUSHS", "LF@i", NULL, NULL);
   instrListAppend(&globalInstrList, "PUSHS", "int@1", NULL, NULL);
   instrListAppend(&globalInstrList, "LTS", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "PUSHS", "bool@true", NULL, NULL);
   instrListAppendInstrLabel(&globalInstrList, "JUMPIFNEQS", "$substrok1", -1);
   instrListAppend(&globalInstrList, "POPFRAME", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "RETURN", NULL, NULL, NULL);
   instrListAppendInstrLabel(&globalInstrList, "LABEL", "$substrok1", -1);

   // if s = "" return ""
   instrListAppend(&globalInstrList, "PUSHS", "LF@s", NULL, NULL);
   instrListAppend(&globalInstrList, "PUSHS", "string@", NULL, NULL);
   instrListAppendInstrLabel(&globalInstrList, "JUMPIFNEQS", "$substrok2", -1);
   instrListAppend(&globalInstrList, "POPFRAME", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "RETURN", NULL, NULL, NULL);
   instrListAppendInstrLabel(&globalInstrList, "LABEL", "$substrok2", -1);

   // retval = n chars
   instrListAppend(&globalInstrList, "DEFVAR", "LF@spaces", NULL, NULL);
   instrListAppend(&globalInstrList, "MOVE", "LF@spaces", "LF@n", NULL);
   instrListAppendInstrLabel(&globalInstrList, "LABEL", "$substrwhile", 1);
   instrListAppend(&globalInstrList, "JUMPIFEQ", "$$substrendwhile1", "LF@spaces", "int@0");

   instrListAppend(&globalInstrList, "CONCAT", "LF@%retval", "LF@%retval", "string@.");
   instrListAppend(&globalInstrList, "SUB", "LF@spaces", "LF@spaces", "int@1");

   instrListAppendInstrLabel(&globalInstrList, "JUMP", "$substrwhile", 1);
   instrListAppendInstrLabel(&globalInstrList, "LABEL", "$substrendwhile", 1);

   // i--
   // iter = 0
   // string char
   // while (n != 0) 
   //   char = s[i]
   //   retval[iter] = char
   //   n--
   //   i++
   //   iter++

   instrListAppend(&globalInstrList, "SUB", "LF@i", "LF@i", "int@1");
   instrListAppend(&globalInstrList, "DEFVAR", "LF@iter", NULL, NULL);
   instrListAppend(&globalInstrList, "MOVE", "LF@iter", "int@0", NULL);
   instrListAppend(&globalInstrList, "DEFVAR", "LF@char", NULL, NULL);

   instrListAppendInstrLabel(&globalInstrList, "LABEL", "$substrwhile", 2);

   instrListAppend(&globalInstrList, "JUMPIFEQ", "$$substrendwhile2", "LF@n", "int@0");

   instrListAppend(&globalInstrList, "GETCHAR", "LF@char", "LF@s", "LF@iter");
   instrListAppend(&globalInstrList, "SETCHAR", "LF@%retval", "LF@iter", "LF@char");

   instrListAppend(&globalInstrList, "SUB", "LF@n", "LF@n", "int@1");
   instrListAppend(&globalInstrList, "ADD", "LF@i", "LF@i", "int@1");
   instrListAppend(&globalInstrList, "ADD", "LF@iter", "LF@iter", "int@1");
   
   instrListAppendInstrLabel(&globalInstrList, "JUMP", "$substrwhile", 2);

   instrListAppendInstrLabel(&globalInstrList, "LABEL", "$substrendwhile", 2);

   instrListAppend(&globalInstrList, "POPFRAME", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "RETURN", NULL, NULL, NULL);

   // Asc(s as string, i as integer) as integer
   instrListAppendInstrLabel(&globalInstrList, "LABEL", "asc", -1);
   instrListAppend(&globalInstrList, "PUSHFRAME", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "DEFVAR", "LF@%tmp1", NULL, NULL);
   instrListAppend(&globalInstrList, "DEFVAR", "LF@%retval", NULL, NULL);
   instrListAppend(&globalInstrList, "MOVE", "LF@%retval", "int@0", NULL);

   // if (length(s) < i && i < 1) return 0;
   instrListAppend(&globalInstrList, "STRLEN", "LF@%tmp1", "LF@s", NULL);
   instrListAppend(&globalInstrList, "PUSHS", "LF@%tmp1", NULL, NULL);
   instrListAppend(&globalInstrList, "PUSHS", "LF@i", NULL, NULL);
   instrListAppend(&globalInstrList, "LTS", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "PUSHS", "bool@true", NULL, NULL);
   instrListAppendInstrLabel(&globalInstrList, "JUMPIFNEQS", "$ascok1", -1);
   instrListAppend(&globalInstrList, "POPFRAME", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "RETURN", NULL, NULL, NULL);
   instrListAppendInstrLabel(&globalInstrList, "LABEL", "$ascok1", -1);
   instrListAppend(&globalInstrList, "PUSHS", "LF@i", NULL, NULL);
   instrListAppend(&globalInstrList, "PUSHS", "int@1", NULL, NULL);
   instrListAppend(&globalInstrList, "LTS", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "PUSHS", "bool@true", NULL, NULL);
   instrListAppendInstrLabel(&globalInstrList, "JUMPIFNEQS", "$ascok2", -1);
   instrListAppend(&globalInstrList, "POPFRAME", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "RETURN", NULL, NULL, NULL);

   instrListAppendInstrLabel(&globalInstrList, "LABEL", "$ascok2", -1);
   // Change indexes from 1 to 0 and calculate
   instrListAppend(&globalInstrList, "SUB", "LF@i", "LF@i", "int@1");
   instrListAppend(&globalInstrList, "STRI2INT", "LF@%retval", "LF@s", "LF@i");

   instrListAppend(&globalInstrList, "POPFRAME", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "RETURN", NULL, NULL, NULL);

   // Chr(i as integer) as string
   instrListAppendInstrLabel(&globalInstrList, "LABEL", "chr", -1);
   instrListAppend(&globalInstrList, "PUSHFRAME", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "DEFVAR", "LF@%retval", NULL, NULL);
   instrListAppend(&globalInstrList, "MOVE", "LF@%retval", "string@", NULL);
   instrListAppend(&globalInstrList, "INT2CHAR", "LF@%retval", "LF@i", NULL);
   instrListAppend(&globalInstrList, "POPFRAME", NULL, NULL, NULL);
   instrListAppend(&globalInstrList, "RETURN", NULL, NULL, NULL);

   // - Generating

   // Start recursive descent
   program();

   instrListPrintAll(&globalInstrList);

   instrListClear(&globalInstrList);

   genStackDispose(whileStack);
   genStackDispose(ifStack);
}

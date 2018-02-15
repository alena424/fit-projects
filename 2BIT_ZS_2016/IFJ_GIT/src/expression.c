/**
 * @file expression.c
 * Implementation of IFJ17 imperative language compiler
 *
 * @brief syntax analysis for expressions
 *
 * @date 11.11.2017
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
#include "expression.h"
#include "generator.h"

#define size 16

const char precedenceTable[ size ][ size ] = {
   // +    -    *    /    \    (    )    =   <>    <    >   <=   >=   id  un -  $
   { '>', '>', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>' }, // +
   { '>', '>', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>' }, // -
   { '>', '>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>' }, // *
   { '>', '>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>' }, // /
   { '>', '>', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>' }, // '\'
   { '<', '<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '-' }, // (
   { '>', '>', '>', '>', '>', '-', '>', '>', '>', '>', '>', '>', '>', '-', '-', '>' }, // )
   { '<', '<', '<', '<', '<', '<', '>', '-', '-', '-', '-', '-', '-', '<', '<', '>' }, // =
   { '<', '<', '<', '<', '<', '<', '>', '-', '-', '-', '-', '-', '-', '<', '<', '>' }, // <>
   { '<', '<', '<', '<', '<', '<', '>', '-', '-', '-', '-', '-', '-', '<', '<', '>' }, // <
   { '<', '<', '<', '<', '<', '<', '>', '-', '-', '-', '-', '-', '-', '<', '<', '>' }, // >
   { '<', '<', '<', '<', '<', '<', '>', '-', '-', '-', '-', '-', '-', '<', '<', '>' }, // <=
   { '<', '<', '<', '<', '<', '<', '>', '-', '-', '-', '-', '-', '-', '<', '<', '>' }, // >=
   { '>', '>', '>', '>', '>', '-', '>', '>', '>', '>', '>', '>', '>', '-', '-', '>' }, // id
   { '>', '>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>' }, // unary -
   { '<', '<', '<', '<', '<', '<', '-', '<', '<', '<', '<', '<', '<', '<', '<', '#' } // $
};

/**
 * @brief Main function for evaluating expression - called from parser.c
 *
 * @param token      token_t*     pointer to token
 * @param table      TSymtable*   pointer to local table
 * @return exprType  data type of result
 */
exprType expression(token_t *token, TSymtable *table) {

   // Pointers for storing parameters for generator
   char *laddr1;

   int totalLength = 0;

   // Variable for storing current character from precedenceTable
   char precedence;

   // Pointers to the top elements in stack
   tExprItem *firstItem, *secondItem, *thirdItem;

   // Pointer to item which is meant to be pushed into the stack
   tExprItem *pushItem;

   // Pointer to local table
   htab_listitem *var;

   // Variable for storing datatype of expression
   exprType typeExpr;

   // Variable for storing return type
   exprType resultType = ex_void;

   // Variable for storing info about interal error
   bool error = false;

   // Inicialization of stack
   tStack* stack = stackInit(MAX_STACK);

   // Allocation error
   if (stack == NULL) {
      if (token->data != NULL) {
         free(token->data);
      }
      free(token);
      symtabFree(globalTable);
      throwError(error_internal);
   }

   // Error if not even first token is part of expression
   if (decodeToken(token, stack) == e_dollar) {
      callError(token, stack, error_expression);       
   }

   // Pushing dollar into the stack
   if ((pushItem = newExprItem(e_dollar, ex_void)) == NULL ||
        stackPush(stack, pushItem) == false) {

      callError(token, stack, error_internal);
   }

   do {
      // Default type for pushed in item is void
      typeExpr = ex_void;

      // Checkes which term is at the top of the stack
      tStackElement term = stack->top;
      while (term->data->symbol != e_dollar) {
         // Skips E, until we got term
         if (term->data->symbol != e_expression) {
            break;
         }
         term = term->nextPtr;
      }

      // Look into the precedence table to get needed action
      precedence = precedenceTable[term->data->symbol][decodeToken(token, stack)];

      // Decides action 
      switch (precedence) {

         // Can happen only in one case
         case '=':

            // Pushing onto the stack
            if ((pushItem = newExprItem(decodeToken(token, stack), typeExpr)) == NULL ||
                 stackPush( stack, pushItem) == false) {

               callError(token, stack, error_internal);
            }

            // Frees the old one
            if (token->data != NULL) {
               free(token->data);
            }           
            free(token);

            // Get new token
            token = get_token();
            if (token == NULL) {
               stackDispose(stack);
               symtabFree(globalTable);
               throwError(error_internal);
            }

            if (token->type == t_invalid) {
               callError( token, stack, error_scanner );
            }

            break;

         case '<':
            // We'll push incoming item onto the stack

            // Checks whether it's an identifier and if it is declared
            if (token->type == t_identifier) {
               var = symtabFind(table, token->data);
               if (var == NULL) {
                  callError(token, stack, error_undeclared);
               }
               // Decoding data type of identifier
               switch (var->data_type) {
                  case TInteger:
                     typeExpr = ex_integer;
                     break;
                  case TDouble:
                     typeExpr = ex_double;
                     break;
                  case TString:
                     typeExpr = ex_string;
                     break;
                  default:
                     callError(token, stack, error_expression);
               }
            }

            // Checkes whether is is constant or literal and we'll add its datatype
            if (token->type == t_int) {
               typeExpr = ex_integer;
            } else if (token->type == t_real) {
               typeExpr = ex_double;
            } else if (token->type == t_literal) {
               typeExpr = ex_string;
            }

            // Pushes item onto the stack
            if ((pushItem = newExprItem(decodeToken(token, stack), typeExpr)) == NULL ||
                 stackPush(stack, pushItem) == false) {

               callError( token, stack, error_internal );
            }

            // + Generating

            if (token->data != NULL) {

               if (token->type == t_int) {
                  
                  totalLength = strlen("int@") + strlen(token->data);
                  laddr1 = malloc(sizeof(char) * (totalLength) + 1);
                  strcpy(laddr1, "int@");
                  strcat(laddr1, token->data);
                  laddr1[totalLength] = '\0';
                  instrListAppend(&globalInstrList, "PUSHS", laddr1, NULL, NULL);

               } else if (token->type == t_real) {

                  totalLength = strlen("float@") + strlen(token->data);
                  laddr1 = malloc(sizeof(char) * (totalLength) + 1);
                  strcpy(laddr1, "float@");
                  strcat(laddr1, token->data);
                  laddr1[totalLength] = '\0';
                  instrListAppend(&globalInstrList, "PUSHS", laddr1, NULL, NULL);

               } else if (token->type == t_literal) {
                  
                  totalLength = strlen("string@") + strlen(token->data);
                  laddr1 = malloc(sizeof(char) * (totalLength) + 1);
                  strcpy(laddr1, "string@");
                  strcat(laddr1, token->data);
                  laddr1[totalLength] = '\0';
                  instrListAppend(&globalInstrList, "PUSHS", laddr1, NULL, NULL);

               } else if (token->type == t_identifier) {

                  totalLength = strlen("XF@") + strlen(token->data);
                  laddr1 = malloc(sizeof(char) * (totalLength + 1));

                  // No return var -> we're in scope
                  if (symtabFind(table, "return") == NULL) {
                     strcpy(laddr1, "GF@");
                  } else {
                     // We're in function 
                     strcpy(laddr1, "LF@");
                  }
                  
                  strcat(laddr1, token->data);

                  instrListAppend(&globalInstrList, "PUSHS", laddr1, NULL, NULL);

               }
               //free(laddr1);
            }

            // - Generating

            // Frees the old one
            if (token->data != NULL) {
               free(token->data);
            }
            free(token);

            // Getting new token
            token = get_token();
            if (token == NULL) {
               stackDispose(stack);
               symtabFree(globalTable);
               throwError(error_internal);
            }
            if (token->type == t_invalid) {
               callError(token, stack, error_scanner);
            }

            break;


         // We're searching for rule to use and we will change data on stack
         case '>':

            // Pops top element of stack
            firstItem = stackPop(stack);
            if (firstItem == NULL) {
               callError(token, stack, error_internal);
            }

            // Call an error if stack is empty
            if (firstItem->symbol == e_dollar) {
               free(firstItem);
               callError(token, stack, error_expression);
            }

            // rule E -> i ... rename identifiers to expressions
            if (firstItem->symbol == e_identifier) {
               // Pushes intead of E
               if ((pushItem = newExprItem(e_expression, firstItem->type)) == NULL ||
                    stackPush(stack, pushItem) == false) {

                  free(firstItem);
                  callError(token, stack, error_internal);
               }
               free(firstItem);
               break;
            }

            // For checking outher rules we need more items
            secondItem = stackPop(stack);
            if (secondItem == NULL) {
               free(firstItem);
               callError(token, stack, error_internal);
            }

            // Rule E = -E ... unary minus
            if (firstItem->symbol == e_expression && secondItem->symbol == e_unary_minus) {

               // Unary minus works only for expressions of type integer or bool
               if (firstItem->type == ex_integer || firstItem->type == ex_double) {

                  if ((pushItem = newExprItem(e_expression, firstItem->type)) == NULL ||
                       stackPush(stack, pushItem) == false) {

                     free(firstItem);
                     free(secondItem);
                     callError(token, stack, error_internal);
                  }

                  // + Generating

                  if (firstItem->type == ex_integer) {
                     instrListAppend(&globalInstrList, "PUSHS", "int@-1", NULL, NULL);
                     instrListAppend(&globalInstrList, "MULS", NULL, NULL, NULL); 
                  } else if (firstItem->type == ex_double) {
                     instrListAppend(&globalInstrList, "PUSHS", "float@-1.0", NULL, NULL);
                     instrListAppend(&globalInstrList, "MULS", NULL, NULL, NULL); 
                  }

                  // - Generating

               } else {
                  free(firstItem);
                  free(secondItem);
                  callError(token, stack, error_unknown_syntax);
               }
               free(firstItem);
               free(secondItem);
               break;
            }

            thirdItem = stackPop(stack);
            if (thirdItem == NULL) {
               free(firstItem);
               free(secondItem);
               callError(token, stack, error_internal);
            }

            // Rule E -> (E) ... rules for getting rid of brackets
            if (firstItem->symbol == e_r_bracket && secondItem->symbol == e_expression
                && thirdItem->symbol == e_l_bracket) {

               // Pushes instead of that E
               if ((pushItem = newExprItem(e_expression, secondItem->type)) == NULL ||
                    stackPush(stack, pushItem) == false) {

                  free(firstItem);
                  free(secondItem);
                  free(thirdItem);
                  callError(token, stack, error_internal);
               }
               free(firstItem);
               free(secondItem);
               free(thirdItem);
               break;
            }

            if (firstItem->symbol == e_expression && thirdItem->symbol == e_expression) {

               // Rule E -> E operator E
               if (isOperator(secondItem->symbol)) {
                  // It is needed to check types and final type push onto the stack
                  // int \ int = int

                  if ((firstItem->type == ex_integer || firstItem->type == ex_double)
                        && (thirdItem->type == ex_integer || thirdItem->type == ex_double)
                        && secondItem->symbol == e_int_div) {


                     if ((pushItem = newExprItem(e_expression, ex_integer)) == NULL ||
                          stackPush( stack, pushItem) == false ) {

                        error = true;
                     }

                     // + Generating

                     // Need to convert both to int

                     if (firstItem->type == ex_double) {
                        instrListAppend(&globalInstrList, "FLOAT2R2EINTS", NULL, NULL, NULL);
                     }

                     if (thirdItem->type == ex_double) {
                        if (symtabFind(table, "return") == NULL) {
                           instrListAppend(&globalInstrList, "POPS", "GF@%tmp1", NULL, NULL);
                           instrListAppend(&globalInstrList, "FLOAT2R2EINTS", NULL, NULL, NULL);
                           instrListAppend(&globalInstrList, "PUSHS", "GF@%tmp1", NULL, NULL);
                        } else {
                           instrListAppend(&globalInstrList, "POPS", "LF@%tmp1", NULL, NULL);
                           instrListAppend(&globalInstrList, "FLOAT2R2EINTS", NULL, NULL, NULL);
                           instrListAppend(&globalInstrList, "PUSHS", "LF@%tmp1", NULL, NULL);
                        }
                     }

                     // Need to convert both to float, then divs, then cut to int

                     if (symtabFind(table, "return") == NULL) {
                        instrListAppend(&globalInstrList, "POPS", "GF@%tmp1", NULL, NULL); 
                        instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
                        instrListAppend(&globalInstrList, "PUSHS", "GF@%tmp1", NULL, NULL);
                        instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
                     } else {
                        instrListAppend(&globalInstrList, "POPS", "LF@%tmp1", NULL, NULL); 
                        instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
                        instrListAppend(&globalInstrList, "PUSHS", "LF@%tmp1", NULL, NULL);
                        instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
                     }

                     instrListAppend(&globalInstrList, "DIVS", NULL, NULL, NULL);

                     instrListAppend(&globalInstrList, "FLOAT2INTS", NULL, NULL, NULL);

                     // - Generating

                  } else if (secondItem->symbol == e_div
                        && (firstItem->type == ex_integer || firstItem->type == ex_double)
                        && (thirdItem->type == ex_integer || thirdItem->type == ex_double)) {
                     // int or double / int or double = double

                     if ((pushItem = newExprItem( e_expression, ex_double)) == NULL ||
                          stackPush(stack, pushItem) == false) {

                        error = true;
                     }

                     // + Generating

                     // First is double, second is double - no need to convert types
                     if (firstItem->type == ex_double && thirdItem->type == ex_double) {

                        instrListAppend(&globalInstrList, "DIVS", NULL, NULL, NULL);

                     } else if (firstItem->type == ex_integer && thirdItem->type == ex_double) {
                        // First

                        // Converting to float
                        instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);

                        // Division
                        instrListAppend(&globalInstrList, "DIVS", NULL, NULL, NULL);

                     } else if (firstItem->type == ex_double && thirdItem->type == ex_integer) {
                        // First

                        // Pop, convert and push
                        // we're in scope
                        if (symtabFind(table, "return") == NULL) {
                           instrListAppend(&globalInstrList, "POPS", "GF@%tmp1", NULL, NULL);
                           instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
                           instrListAppend(&globalInstrList, "PUSHS", "GF@%tmp1", NULL, NULL);
                        } else {
                           instrListAppend(&globalInstrList, "POPS", "LF@%tmp1", NULL, NULL);
                           instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
                           instrListAppend(&globalInstrList, "PUSHS", "LF@%tmp1", NULL, NULL);
                        }

                        instrListAppend(&globalInstrList, "DIVS", NULL, NULL, NULL);

                     } else {
                        // First

                        // Pop, convert and push
                        // we're in scope
                        if (symtabFind(table, "return") == NULL) {
                           instrListAppend(&globalInstrList, "POPS", "GF@%tmp1", NULL, NULL);
                           instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
                           instrListAppend(&globalInstrList, "PUSHS", "GF@%tmp1", NULL, NULL);
                        } else {
                           instrListAppend(&globalInstrList, "POPS", "LF@%tmp1", NULL, NULL);
                           instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
                           instrListAppend(&globalInstrList, "PUSHS", "LF@%tmp1", NULL, NULL);
                        }

                        instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);

                        instrListAppend(&globalInstrList, "DIVS", NULL, NULL, NULL);

                     } 

                     // - Generating

                  } else if (secondItem->symbol == e_plus && firstItem->type == ex_string
                        && thirdItem->type == ex_string) { 
                     // string + string = string

                     // TODO - tady budes muset konkatenovat dva retezce
                     // NEFUNGUJE - chyba interpretu?

                     // + Generating

                     if (symtabFind(table, "return") == NULL) {
                        instrListAppend(&globalInstrList, "POPS", "GF@%tmp2", NULL, NULL);
                        instrListAppend(&globalInstrList, "POPS", "GF@%tmp3", NULL, NULL);
                        instrListAppend(&globalInstrList, "CONCAT", "GF@%tmp1", "GF@%tmp2", "GF%tmp3");
                        instrListAppend(&globalInstrList, "PUSHS", "GF@%tmp1", NULL, NULL);
                     } else {
                        instrListAppend(&globalInstrList, "POPS", "LF@%tmp2", NULL, NULL);
                        instrListAppend(&globalInstrList, "POPS", "LF@%tmp3", NULL, NULL);
                        instrListAppend(&globalInstrList, "CONCAT", "LF@%tmp1", "LF@%tmp2", "LF%tmp3");
                        instrListAppend(&globalInstrList, "PUSHS", "LF@%tmp1", NULL, NULL);
                     }

                     // - Generating


                     if ((pushItem = newExprItem(e_expression, ex_string)) == NULL ||
                          stackPush(stack, pushItem) == false) {

                        error = true;
                     }

                  } else if (secondItem->symbol == e_plus || secondItem->symbol == e_minus ||
                             secondItem->symbol == e_mul) {

                     // int +-* int = int
                     if (firstItem->type == ex_integer && thirdItem->type == ex_integer) {

                        // + Generating

                        if (secondItem->symbol == e_plus) {
                           instrListAppend(&globalInstrList, "ADDS", NULL, NULL, NULL); 
                        } else if (secondItem->symbol == e_minus) {
                           instrListAppend(&globalInstrList, "SUBS", NULL, NULL, NULL);
                        } else if (secondItem->symbol == e_mul) {
                           instrListAppend(&globalInstrList, "MULS", NULL, NULL, NULL); 
                        }

                        // - Generating

                        if ((pushItem = newExprItem(e_expression, ex_integer)) == NULL ||
                              stackPush(stack, pushItem) == false) {

                           error = true;
                        }

                     } else if (firstItem->type == ex_integer && thirdItem->type == ex_double) {
                        // int +-* double = double

                        // + Generating

                        // Convert top of the stack int to float

                        instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);

                        if (secondItem->symbol == e_plus) {
                           instrListAppend(&globalInstrList, "ADDS", NULL, NULL, NULL); 
                        } else if (secondItem->symbol == e_minus) {
                           instrListAppend(&globalInstrList, "SUBS", NULL, NULL, NULL);
                        } else if (secondItem->symbol == e_mul) {
                           instrListAppend(&globalInstrList, "MULS", NULL, NULL, NULL); 
                        }

                        // - Generating

                        if ((pushItem = newExprItem( e_expression, ex_double)) == NULL ||
                             stackPush( stack, pushItem ) == false ) {

                           error = true;
                        }                    

                     } else if (firstItem->type == ex_double && thirdItem->type == ex_integer) {
                        // double +-* int = double

                        // + Generating

                        // Convert second of the stack int to float

                        if (symtabFind(table, "return") == NULL) {
                           instrListAppend(&globalInstrList, "POPS", "GF@%tmp1", NULL, NULL);
                           instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
                           instrListAppend(&globalInstrList, "PUSHS", "GF@%tmp1", NULL, NULL);
                        } else {
                           instrListAppend(&globalInstrList, "POPS", "LF@%tmp1", NULL, NULL);
                           instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
                           instrListAppend(&globalInstrList, "PUSHS", "LF@%tmp1", NULL, NULL);
                        }

                        if (secondItem->symbol == e_plus) {
                           instrListAppend(&globalInstrList, "ADDS", NULL, NULL, NULL); 
                        } else if (secondItem->symbol == e_minus) {
                           instrListAppend(&globalInstrList, "SUBS", NULL, NULL, NULL);
                        } else if (secondItem->symbol == e_mul) {
                           instrListAppend(&globalInstrList, "MULS", NULL, NULL, NULL); 
                        }

                        // - Generating

                        if ((pushItem = newExprItem( e_expression, ex_double)) == NULL ||
                             stackPush(stack, pushItem) == false) {

                           error = true;
                        }

                     } else if (firstItem->type == ex_double && thirdItem->type == ex_double) {
                        // double +-* double = double

                        // + Generating

                        if (secondItem->symbol == e_plus) {
                           instrListAppend(&globalInstrList, "ADDS", NULL, NULL, NULL); 
                        } else if (secondItem->symbol == e_minus) {
                           instrListAppend(&globalInstrList, "SUBS", NULL, NULL, NULL);
                        } else if (secondItem->symbol == e_mul) {
                           instrListAppend(&globalInstrList, "MULS", NULL, NULL, NULL); 
                        }

                        // - Generating

                        if ((pushItem = newExprItem( e_expression, ex_double)) == NULL ||
                             stackPush(stack, pushItem) == false) {

                           error = true;
                        }                                                        
                     } else {
                        free(firstItem);
                        free(secondItem);
                        free(thirdItem);
                        callError(token, stack, error_incompatible_type);
                     }
                  } else {
                     // Wrong combination
                     free(firstItem);
                     free(secondItem);
                     free(thirdItem);
                     callError(token, stack, error_incompatible_type);
                  }

                  free(firstItem);
                  free(secondItem);
                  free(thirdItem);

                  if (error) {
                     callError(token, stack, error_internal);
                  }

                  break;
               }

               // rule E -> E relational E
               if (isRelational(secondItem->symbol)) {

                  // Need to check types and final bool, push it onto the stack
                  if ((firstItem->type == ex_string && thirdItem->type != ex_string) ||
                      (firstItem->type != ex_string && thirdItem->type == ex_string) ||
                      (firstItem->type == ex_bool || thirdItem->type == ex_bool)) {
                     // If only one of them is string or one of them is bool

                     free(firstItem);
                     free(secondItem);
                     free(thirdItem);
                     if (firstItem->type == ex_bool || thirdItem->type == ex_bool) {
                        callError(token, stack, error_unknown_syntax);
                     }
                     callError(token, stack, error_incompatible_type);
                  }

                  // + Generating

                  if (firstItem->type == ex_integer && thirdItem->type == ex_double) {

                     instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL); 

                  } else if (firstItem->type == ex_double && thirdItem->type == ex_integer) {

                     if (symtabFind(table, "return") == NULL) {
                        instrListAppend(&globalInstrList, "POPS", "GF@%tmp1", NULL, NULL);
                        instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
                        instrListAppend(&globalInstrList, "PUSHS", "GF@%tmp1", NULL, NULL);
                     } else {
                        instrListAppend(&globalInstrList, "POPS", "LF@%tmp1", NULL, NULL);
                        instrListAppend(&globalInstrList, "INT2FLOATS", NULL, NULL, NULL);
                        instrListAppend(&globalInstrList, "PUSHS", "LF@%tmp1", NULL, NULL);
                     }

                  }


                  switch (secondItem->symbol) {
                     // =
                     case e_eq:
                        instrListAppend(&globalInstrList, "EQS", NULL, NULL, NULL);
                        break;

                     // <
                     case e_lt:
                        instrListAppend(&globalInstrList, "LTS", NULL, NULL, NULL);
                        break;

                     // >

                     case e_gt:
                        instrListAppend(&globalInstrList, "GTS", NULL, NULL, NULL);
                        break;

                     // <>
                     case e_ne:
                        instrListAppend(&globalInstrList, "EQS", NULL, NULL, NULL);
                        instrListAppend(&globalInstrList, "NOTS", NULL, NULL, NULL);
                        break;

                     // <=
                     case e_le:
                        instrListAppend(&globalInstrList, "GTS", NULL, NULL, NULL);
                        instrListAppend(&globalInstrList, "NOTS", NULL, NULL, NULL);
                        break;

                     // >=
                     case e_ge:
                        instrListAppend(&globalInstrList, "LTS", NULL, NULL, NULL);
                        instrListAppend(&globalInstrList, "NOTS", NULL, NULL, NULL);
                        break;

                     default:
                        break;
                  }
                  // - Generating


                  // Otherwise everything ok
                  if ((pushItem = newExprItem( e_expression, ex_bool)) == NULL ||
                       stackPush( stack, pushItem) == false) {

                     error = true;
                  }

                  free(firstItem);
                  free(secondItem);
                  free(thirdItem);

                  if (error) {
                     callError(token, stack, error_internal);
                  }

                  break;
               }
            }
            // Not any rule used by now -> error
            callError(token, stack, error_expression);
            break;

         // Result is on stack, need to return it
         case '#':

            // Check it
            firstItem = stackPop(stack);

            if (firstItem == NULL) {
               callError(token, stack, error_internal);
            }

            secondItem = stackPop(stack);
            if (secondItem == NULL) {
               free(firstItem);
               callError(token, stack, error_internal);
            }

            if (firstItem->symbol == e_expression && secondItem->symbol == e_dollar) {

               resultType = firstItem->type;
               free(firstItem);
               free(secondItem);
               break;
            }
            // Otherwise error
            free(firstItem);
            free(secondItem);
            callError(token, stack, error_expression);
            break;

         // Otherwise wrong combination
         default:
            callError(token, stack, error_expression);
      }

   } while (resultType == ex_void);

   // Clear stack
   stackDispose(stack);

   return resultType;
}

/**
 * @brief Function which tells whether symbol is an operator.
 *
 * @param symbol exprSymbol  considered symbol
 * @return int
 */
int isOperator(exprSymbol symbol) {
   return (symbol == e_plus || symbol == e_minus || symbol == e_mul || symbol == e_div || symbol == e_int_div);
}


/**
 * @brief Function which tells whether symbol is relation operator.
 *
 * @param symbol exprSymbol  considered symbol
 * @return int
 */
int isRelational(exprSymbol symbol) {
   return (symbol == e_eq || symbol == e_ne || symbol == e_lt || symbol == e_gt || symbol == e_le || symbol == e_ge);
}

/**
 * @brief Function which decodes token type so we're able to read it from precedence table
 *
 * @param token token_t*   pointer to decoded token
 * @param stack   tStack   pointer to expression stack
 * @return exprSymbol 
 */
exprSymbol decodeToken(token_t *token, tStack *stack) {

   tExprItem *top;

   switch(token->type) {    

      case t_plus:
         return e_plus;

      // Unary or binary minus
      case t_minus:

         // If stack empty - it is unary
         if (stackEmpty(stack)) {
            return e_unary_minus;
         }

         // Cases when its minus:
         //    on top of the stack is id or expression or right bracket
         top = stackTop(stack);
         if (top->symbol == e_expression || top->symbol == e_r_bracket || top->symbol == e_identifier) {
            return e_minus;
         } else {
            return e_unary_minus;
         }
      case t_mul:
         return e_mul;
      case t_div:
         return e_div;
      case t_int_div:
         return e_int_div;
      case t_l_bracket:
         return e_l_bracket;
      case t_r_bracket:
         return e_r_bracket;
      case t_eq:
         return e_eq;
      case t_ne:
         return e_ne;
      case t_lt:
         return e_lt;
      case t_gt:
         return e_gt;
      case t_le:
         return e_le;
      case t_ge:
         return e_ge;
      case t_identifier:
      case t_int:
      case t_real:
      case t_literal:
         return e_identifier;
      default:
         return e_dollar;
   }
}

/**
 * @brief Function creates new item tExprItem.
 *
 * @param symbol  exprSymbol  item's symbol which we push onto the stack
 * @param type    exprType    type of item
 * @return tExprItem pointer to newly created item
 */
tExprItem* newExprItem (exprSymbol symbol, exprType type) {
   tExprItem *item = malloc(sizeof(tExprItem));
   if (item == NULL) {
      return NULL;
   }
   item->symbol = symbol;
   item->type = type;

   return item;
}

/**
 * @brief Function frees token and a stack then handles errors
 *
 * @param token token_t*  current token
 * @param stack tStack    expression stack
 * @param errro errors    error type
 */

void callError ( token_t *token, tStack *stack, errors error ) {

   if (token->data != NULL) {
      free(token->data);
   }
   free(token);
   stackDispose(stack);
   symtabFree(globalTable);
   throwError(error);

}

/**
* @file symtable.h
* Implementation of IFJ17 imperative language compiler
*
* @brief hash table operations
*
* @date 12.11.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/



#ifndef _SYMTABLE
#define _SYMTABLE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// dont forget to include Stack

#define TABLE_SIZE 769  // max table size - prime

//data type of variables/functions
typedef enum {
    TInteger, TDouble, TString, Unknown// unknow nastavuji pro inicializaci
} Type_data;

/**
* @brief Structure of hash params
*/
typedef struct TParam
{
    char *name; //name of variable
    Type_data data_type; // type of variable
    struct TParam *next;

} TParam;



typedef struct TSymtable TSymtable;

typedef struct htab_listitem {
    bool is_defined; //was function or variable declared?
    char *name; //name of function/variable - our key
    Type_data data_type; // return type for function/type for variable
    unsigned int counter_params; //how many params do we have
    TSymtable *local_table;
    struct htab_listitem *nextPtr;
    TParam *params; //list of params

} htab_listitem;

/**
* @brief Structure of hash table
*/
struct TSymtable
{
    bool is_global; // is global?
    unsigned int size_sym; //maximum size
    unsigned int actual_number; //actual number of records in hash table
    struct htab_listitem *symtab_item[]; //elements in hash table
};


/**
* @param size size of hash table
* @brief Inicialization of hash table
* @return hash table
*/
TSymtable *symtabInit(unsigned int size_symtable);

/**
* @brief hash function
* @param str string, from which we create a key for indexing, it's a name of variable or function
*/
unsigned int hash_function(const char *str);

/**
* @brief deletes a list of item
* @param list
*/
void symtabRemoveList( htab_listitem *list );


/**
* @brief delete all items, table stays empty
* @param table hash table
*/
void symtabClear( TSymtable *table );

/**
* @brief release hash table
* @param table hash table
*/
void symtabFree(TSymtable *table);


/**
* @brief add a new item with token_data
* @param symtable hash table
* @param token_data name of variable
* @return pointer to found record or it adds key to hash table and returns pointer to new record
*/
struct htab_listitem *symtabLookAdd(TSymtable *symtable, char *token_data);

/**
* @brief add a new item with token_data
* @param symtable hash table
* @param token_data name of variable
* @param type_param type of param
* @param name_param name of param
* @return pointer to found record or NULL
*/
struct htab_listitem *symtabAddParam(TSymtable *symtable, char *token_data, Type_data type_param, char *name_param);


/**
* @brief add a new item with token_data
* @param symtable hash table
* @param token_data name of variable
* @param type_variable type of variable or return function type
* @return pointer to found record or NULL
*/
struct htab_listitem *symtabAddType(TSymtable *symtable, char *token_data, Type_data type_variable);



/**
* @brief same as htab_lookup_add, but without allocation
* @return NULL if key does not exist
*/
struct htab_listitem *symtabFind(TSymtable *table, const char *key);


/**
* @brief go throw table and verify if all the functions are defined
* @return true all functions defined, false otherwise
*/
bool isDefined (TSymtable *table);

#endif // _SYMTABLE

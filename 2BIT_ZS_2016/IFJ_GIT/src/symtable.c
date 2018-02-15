/**
* @file symtable.c
* Implementation of IFJ17 imperative language compiler
*
* @brief hash table operations
*
* @date 31.10.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#include "symtable.h"

/**
* @param size size of hash table
* @brief Inicialization of hash table
* @return hash table
*/
TSymtable *symtabInit(unsigned int size_symtable)
{
    TSymtable *table = malloc( sizeof( struct htab_listitem)*size_symtable + sizeof( TSymtable ) );
    if ( table == NULL )
    {
        return NULL;
    }
    if ( size_symtable <= 0 )
    {
        //size must be greater than zero
        return NULL;
    }
    //max size of hash table
    table->size_sym = size_symtable;

    //we have no records
    table->actual_number = 0;

    for(unsigned int i = 0; i < size_symtable; i++)
    {
        table->symtab_item[i] = NULL;
    }
    return table;
}

/**
* @brief hash function
* @param str string, from which we create a key for indexing, it's a name of variable or function
*/
unsigned int hash_function(const char *str)
{
    unsigned int h=0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}


/**
* @brief add a new item with token_data
* @param symtable hash table
* @param token_data name of variable
* @return pointer to found record or it adds key to hash table and returns pointer to new record
*/
struct htab_listitem *symtabLookAdd(TSymtable *symtable, char *token_data)
{
    // table must be inicialized
    if ( symtable != NULL && symtable->size_sym )
    {
        // finding index
        unsigned int index = hash_function(token_data) % symtable->size_sym;
        struct htab_listitem *zaznam =  malloc(sizeof( struct htab_listitem ) );
        if ( zaznam == NULL )
        {
            return NULL;
        }
        struct htab_listitem *prevlist = symtable->symtab_item[index];
        struct htab_listitem *list = symtable->symtab_item[index];
        char * klicpom = (char *) malloc( ( strlen(token_data) +1) );

        //inicializace promennych
        strcpy(klicpom, token_data);
        zaznam->name = klicpom;
        zaznam->counter_params = 0;
        zaznam->nextPtr = NULL;
        zaznam->local_table = NULL;
        zaznam->data_type = Unknown;
        zaznam->is_defined = 0;
        zaznam->params = NULL;

        //going through list and finding record
        //if list is empty, we create new record
        if ( list == NULL )
        {
            symtable->symtab_item[index] = zaznam;
            return symtable->symtab_item[index];
        }
        //list is not empty, we return record
        while( list != NULL)
        {
            if ( strcmp(list->name, token_data ) == 0 )
            {
                //nasli jsme, vratime, kde se nachazi
                return list;
            }
            prevlist = list;
            list = list->nextPtr;
        }
        //item was not found, we add record to the end of list
        prevlist->nextPtr = zaznam;
        list = prevlist;
        //symtable->symtab_item[index] = list;
        return zaznam;

    }
    return NULL;

}


/**
* @brief deletes a list of item
* @param list list
*/
void symtabRemoveList( htab_listitem *list )
{

    while( list != NULL )
    {

        // do we have any params?
        // deleting all params
        while( list->params != NULL )
        {
            TParam *deleteItem = list->params;
             list->params = list->params->next;
            free(deleteItem->name); //free name
            deleteItem->name = NULL;

            free(deleteItem); // uvolnime polozku
            deleteItem = NULL;

        }
        free((char*)list->name);
        list->name = NULL;

        // do we have an existing local table?
        if ( list->local_table != NULL )
        {
            symtabClear( list->local_table );
        }
        struct htab_listitem *pomlist = list;
        list = list->nextPtr;
        free(pomlist);
        pomlist = NULL;
    }
    free(list);
}
/**
* @brief delete all items, table stays empty
* @param table hash table
*/
void symtabClear( TSymtable *table )
{
    if ( table != NULL )
    {
        for (unsigned int i = 0; i < table->size_sym; i++)
        {
            struct htab_listitem *list = table->symtab_item[i];
            if ( table->symtab_item[i] != NULL)
            {
                symtabRemoveList( list );
            }
            table->symtab_item[i] = NULL;
        }
    }
    symtabFree( table );


}

/**
* @brief release hash table
* @param table hash table
*/
void symtabFree(TSymtable *table)
{
    if ( table != NULL )
    {
        free(table);
        table = NULL;
    }

}

/**
* @brief same as htab_lookup_add, but without allocation
* @return NULL if key does not exist
*/

struct htab_listitem *symtabFind (TSymtable *table, const char *name)
{
    // check if we have a table
    if ( table != NULL && table->size_sym)
    {
        unsigned int index = hash_function(name) % table->size_sym;
        struct htab_listitem *list = table->symtab_item[index];
        while( list != NULL )
        {
            if ( strcmp( list->name, name ) == 0 )
            {
                //found record
                return list;
            }
            list = list->nextPtr;
        }

    }

    return NULL;
}

/**
* @brief add a new item with token_data
* @param symtable hash table
* @param token_data name of variable
* @param type type of param
* @param name name of param
* @return pointer to found record or NULL
*/
struct htab_listitem *symtabAddParam(TSymtable *symtable, char *token_data, Type_data type_param, char *name_param)
{
    if ( symtable != NULL )
    {
        htab_listitem *record = symtabFind( symtable, token_data ); //search according to token_data
        if ( record != NULL )
        {
            record->counter_params++; //add params
            TParam *params = record->params;
            TParam *prevparams = record->params; // remember previous record of list

            TParam *newItem = malloc( sizeof(TParam ) );
            if ( newItem == NULL )
            {
                //error in malloc
                return NULL;
            }
            newItem->data_type = type_param;

            // we need a pointer to allocate space for name_param
            char * klicpom_par = (char *) malloc( ( strlen(name_param) +1) );
            if ( klicpom_par == NULL )
            {
                return NULL;
            }

            strcpy(klicpom_par, name_param);
            newItem->name = klicpom_par;
            newItem->next = NULL;

            // if it is a first param to add to list
            if ( params == NULL )
            {
                record->params = newItem;
                return record;
            }
            // go through list and get to the end
            while( params != NULL)
            {
                prevparams = params;
                params = params->next;
            }
            //add params to the end of list
            prevparams->next = newItem;
            params = prevparams;
            return record;

        }
        else
        {
            return NULL;
        }

    }
    return NULL;

}

/**
* @brief add a new item with token_data
* @param symtable hash table
* @param token_data name of variable
* @param type_variable type of variable or return function type
* @return pointer to found record or NULL
*/
struct htab_listitem *symtabAddType(TSymtable *symtable, char *token_data, Type_data type_variable)
{
    if ( symtable != NULL )
    {
        htab_listitem *record = symtabFind( symtable, token_data ); //vyhledavam podle token_data
        if ( record != NULL )
        {
            record->data_type = type_variable;
            return record;
        }
        else
        {
            return NULL;
        }

    }
    return NULL;
}

/**
* @brief go throw all items in list and verify if all the functions are declared
* @return true all functions declared, false otherwise
*/
bool _isListDefined (htab_listitem *list)
{
    while( list != NULL )
    {
        if ( ! list->is_defined )
        {
            return false;
        }
        else
        {
            list = list->nextPtr;
        }
    }
    //returns true even if list is empty
    return true;

}

/**
* @brief go throw table and verify if all the functions are declared
* @return true all functions declared, false otherwise
*/
bool isDefined (TSymtable *table)
{
    if ( table != NULL )
    {
        for (unsigned int i = 0; i < table->size_sym; i++)
        {
            //seznam
            struct htab_listitem *list = table->symtab_item[i];
            if ( table->symtab_item[i] != NULL)
            {
                if (  _isListDefined(list) == false )
                {
                    //at least one item was not defined in list
                    return false;
                }
            }
        }
        // we went through hash table
        return true;

    }
    return false;
}

/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, říjen 2014
**                              Radek Hranický, listopad 2015
**                              Radek Hranický, říjen 2016
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  htInsert ..... vložení prvku
**  htSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  htRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.free(deletedItem);
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key )
{
    int retval = 1;
    int keylen = strlen(key);
    for ( int i=0; i<keylen; i++ )
        retval += key[i];
    return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht )
{
    for ( int i = 0; i < MAX_HTSIZE ; i++ )
    {
        (*ptrht)[i] = NULL; //nastavime ukazatele na NULL
    }

}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key )
{
    int hash_index = hashCode( key ); // dostaneme index
    tHTItem *list = (*ptrht)[ hash_index ]; //dostanu seznam, kde se nachazi klic

    while ( list )
    {
        if ( strcmp(list->key, key) == 0 )
        {
            // rovnaji se klice
            return list;
        }
        // prejdu na dlasi prvek
        list = list->ptrnext;
    }
    return NULL;

}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data )
{
    tHTItem *list  = htSearch(ptrht, key); // dostanu list, kde se nachazi prvek, ktery chci vkladat
    // pokud jsme nasli
    if ( list != NULL )
    {
        // nasli jsme hodnotu, tzn. ze se tam dana hodnota nachazi, aktualizujeme jeji cast
        list->data = data;

    }
    else
    {
        // kdyz ne, tak vlozim na zacatek seznamu
        // napred musim najit index
        int index = hashCode(key);
        tHTItem *list = (*ptrht)[ index ]; // seznam, kam budu vkladat

        // mallokuju vkladany prvek
        tHTItem *item = malloc(sizeof( tHTItem ) );
        item->data = data;
        item->key = key;
        item->ptrnext = NULL;

        if ( list == NULL )
        {
            //pokud v nem nic neni, vkladam na zacatek
            (*ptrht)[ index ] = item;

        }
        else
        {
            // je v nem neco, provazeme to
            item->ptrnext = list;
            (*ptrht)[ index ] = item; // na zacatek seznamu
        }

    }
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci htSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key )
{
    if ( *ptrht != NULL )
    {
        tHTItem *list  = htSearch(ptrht, key);
        if ( list )
        {
            // pokud jsme neco nasli, vracime ukazatel
            return &(list->data) ;
        }
    }
    return NULL;
    // solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci htSearch.
*/

void htDelete ( tHTable* ptrht, tKey key )
{
    if ( *ptrht != NULL )
    {
        int hash_index = hashCode( key ); // dostaneme index
        tHTItem *list = (*ptrht)[ hash_index ]; //dostanu senznam, kde se nachazi klic
        tHTItem *predlist = NULL;

        while ( list != NULL)
        {
            if ( strcmp(list->key, key) == 0 )
            {
                // rovnaji se klice
                tHTItem *deletedItem = list;
                if ( list == (*ptrht)[ hash_index ] )
                {
                    // pokud mazu prvni
                    (*ptrht)[ hash_index ] = list->ptrnext;
                    free(deletedItem);

                }
                else
                {
                    // mazeme posledni nebo uprostred prvek seznamu
                    predlist->ptrnext = deletedItem->ptrnext;
                    free(deletedItem);

                }

                return;
            }
            // error corrected - predlist
            predlist = list;
            list = list->ptrnext;

        }
    }
    //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/


void htClearAll ( tHTable* ptrht )
{
    for ( int i = 0; i <  MAX_HTSIZE; i++ ){
        // projdeme celou tabulku
        if ( (*ptrht)[i] != NULL ) {
            // seznam neni prazdny


            while( (*ptrht)[ i ] != NULL ){
                    // neni prazdny, mazu
                tHTItem *deleteItem = (*ptrht)[ i ];
                (*ptrht)[ i ] = (*ptrht)[ i ]->ptrnext;
                free(deleteItem);
            }

        }
    }
}

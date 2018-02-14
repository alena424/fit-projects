/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, říjen 2017
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError()
{
    /*
    ** Vytiskne upozornění na to, že došlo k chybě.
    ** Tato funkce bude volána z některých dále implementovaných operací.
    **/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L)
{
    /*
    ** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
    ** z následujících funkcí nebude volána nad neinicializovaným seznamem).
    ** Tato inicializace se nikdy nebude provádět nad již inicializovaným
    ** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
    ** že neinicializované proměnné mají nedefinovanou hodnotu.
    **/
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;

}

void DLDisposeList (tDLList *L)
{
    /*
    ** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
    ** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
    ** uvolněny voláním operace free.
    **/
    tDLElemPtr newItem;

    // postupne uvolnovani, kdyz bude prazdny, nastavi se aktivita a aktivni hned na null
    while (L->First != NULL)
    {
        newItem = L->First->rptr;
        free(L->First);
        L->First = newItem;
    }
    // aktualni, posledni a prvni bude null

    L->Last = NULL;
    L->Act = NULL;


}

void DLInsertFirst (tDLList *L, int val)
{
    /*
    ** Vloží nový prvek na začátek seznamu L.
    ** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
    ** volá funkci DLError().
    **/
    tDLElemPtr newItem ;
    if ( ( newItem = malloc(sizeof( struct tDLElem)) ) == NULL)
    {
        DLError(); // nepodarilo se alokovat novy prvek
        return;
    }
    newItem->data = val;
    newItem->rptr = L->First;
    newItem->lptr = NULL;
    if ( L->First != NULL )
    {
        L->First->lptr = newItem; // prvni bude doleva ukazovat na novy prvek
    }
    else
    {
        L->Last = newItem;
    }
    L->First = newItem; // ukazatel zacatku

}

void DLInsertLast(tDLList *L, int val)
{
    /*
    ** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
    ** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
    ** volá funkci DLError().
    **/

    tDLElemPtr newItem ;
    if ( ( newItem = malloc(sizeof( struct tDLElem)) ) == NULL)
    {
        DLError(); // nepodarilo se alokovat novy prvek
    }
    newItem->data = val;
    newItem->rptr = NULL;
    newItem->lptr = L->Last;
    if ( L->Last != NULL )
    {
        L->Last->rptr = newItem; // prvni bude doleva ukazovat na novy prvek
    }
    else
    {
        L->First = newItem;
    }
    L->Last = newItem; // ukazatel zacatku

}

void DLFirst (tDLList *L)
{
    /*
    ** Nastaví aktivitu na první prvek seznamu L.
    ** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
    ** aniž byste testovali, zda je seznam L prázdný.
    **/
    L->Act = L->First;
}

void DLLast (tDLList *L)
{
    /*
    ** Nastaví aktivitu na poslední prvek seznamu L.
    ** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
    ** aniž byste testovali, zda je seznam L prázdný.
    **/
    L->Act = L->Last;

}

void DLCopyFirst (tDLList *L, int *val)
{
    /*
    ** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
    ** Pokud je seznam L prázdný, volá funkci DLError().
    **/
    if ( L->First == NULL )
    {
        DLError();
        return;
    }
    else
    {
        *val = L->First->data;
    }
}

void DLCopyLast (tDLList *L, int *val)
{
    /*
    ** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
    ** Pokud je seznam L prázdný, volá funkci DLError().
    **/
    if ( L->Last == NULL )
    {
        DLError();
        return;
    }
    else
    {
        *val = L->Last->data;
    }

}

void DLDeleteFirst (tDLList *L)
{
    /*
    ** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
    ** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
    **/
    tDLElemPtr deleteItem; // sem budu ukladat odstranovany prvek, ktery pak free()

    if (L->First != NULL)
    {
        deleteItem = L->First;
        if ( L->Act == L->First )
        {
            // aktualni je ten odstranovany
            L->Act = NULL; //aktivita se ztrati

        }

        if ( L->First == L->Last )
        {
            // seznam ma jeden prvek
            L->First = NULL;
            L->Last = NULL;
            L->Act = NULL;
        }
        else
        {
            L->First = L->First->rptr; // nova prvni polozka v seznamu
            L->First->lptr = NULL;

        }
        free(deleteItem);
    }
}

void DLDeleteLast (tDLList *L)
{
    /*
    ** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
    ** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
    **/

    tDLElemPtr deleteItem; // sem budu ukladat odstranovany prvek, ktery pak free()

    if (L->Last != NULL)
    {
        deleteItem = L->Last;
        if ( L->Act == L->Last )
        {
            // aktualni je ten odstranovany
            L->Act = NULL; //aktivita se ztrati
        }

        if ( L->First == L->Last )
        {
            // seznam ma jeden prvek
            L->First = NULL;
            L->Last = NULL;
            L->Act = NULL;
        }
        else
        {
            L->Last = L->Last->lptr; // nova prvni polozka v seznamu
            L->Last->rptr = NULL;

        }
        free(deleteItem);
    }                 /* V případě řešení, smažte tento řádek! */
}

void DLPostDelete (tDLList *L)
{
    /*
    ** Zruší prvek seznamu L za aktivním prvkem.
    ** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
    ** posledním prvkem seznamu, nic se neděje.
    **/
    // mame vubec aktivni prvek?
    if ( L->Act != NULL )
    {
        if ( L->Act->rptr != NULL )
        {
            //je co rusit?
            tDLElemPtr deleteItem;
            deleteItem = L->Act->rptr;
            L->Act->rptr = deleteItem->rptr; // vynechani mazaneho prvku

            if ( deleteItem == L->Last )
            {
                L->Last = L->Act;
            }
            else
            {
                deleteItem->rptr->lptr = L->Act;
            }
            free(deleteItem);
        }
    }
}

void DLPreDelete (tDLList *L)
{
    /*
    ** Zruší prvek před aktivním prvkem seznamu L .
    ** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
    ** prvním prvkem seznamu, nic se neděje.
    **/

    // mame vubec aktivni prvek?
    if ( L->Act != NULL )
    {
        if ( L->Act->lptr != NULL )
        {
            //je co rusit?
            tDLElemPtr deleteItem;
            deleteItem = L->Act->lptr;
            L->Act->rptr = deleteItem->lptr; // vynechani mazaneho prvku

            if ( deleteItem == L->First )
            {
                L->First = L->Act;
            }
            else
            {
                deleteItem->lptr->rptr = L->Act;
            }
            free(deleteItem);
        }
    }
}

void DLPostInsert (tDLList *L, int val)
{
    /*
    ** Vloží prvek za aktivní prvek seznamu L.
    ** Pokud nebyl seznam L aktivní, nic se neděje.
    ** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
    ** volá funkci DLError().
    **/
    if ( L->Act != NULL )
    {
        tDLElemPtr newItem ;
        if ( ( newItem = malloc(sizeof(tDLElemPtr)) ) == NULL)
        {
            DLError(); // nepodarilo se alokovat novy prvek
        }
        newItem->data = val;
        newItem->lptr = L->Act;
        newItem->rptr = L->Act->rptr;
        L->Act->rptr = newItem;

        // kdybychom vkladali nakonec
        if ( L->Act == L->Last)
        {
            L->Last = newItem; //tedka je novy prvek posledni
        }
        else
        {
            newItem->rptr->lptr = newItem; // pravy kamos musi vedet o novem levem prvku
        }
    }
}

void DLPreInsert (tDLList *L, int val)
{
    /*
    ** Vloží prvek před aktivní prvek seznamu L.
    ** Pokud nebyl seznam L aktivní, nic se neděje.
    ** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
    ** volá funkci DLError().
    **/

    if ( L->Act != NULL )
    {
        tDLElemPtr newItem ;
        if ( ( newItem = malloc(sizeof(tDLElemPtr)) ) == NULL)
        {
            DLError(); // nepodarilo se alokovat novy prvek
        }
        newItem->data = val;
        newItem->rptr = L->Act;
        newItem->lptr = L->Act->lptr;
        L->Act->lptr = newItem;

        // kdybychom vkladali nazacatel
        if ( L->Act == L->First)
        {
            L->First = newItem;
        }
        else
        {
            newItem->lptr->rptr = newItem;
        }
    }
}

void DLCopy (tDLList *L, int *val)
{
    /*
    ** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
    ** Pokud seznam L není aktivní, volá funkci DLError ().
    **/
    if ( L->Act != NULL)
    {
        *val = L->Act->data;
    }
    else
    {
        DLError();
    }
}

void DLActualize (tDLList *L, int val)
{
    /*
    ** Přepíše obsah aktivního prvku seznamu L.
    ** Pokud seznam L není aktivní, nedělá nic.
    **/
    if ( L->Act )
    {
        L->Act->data = val;
    }
}

void DLSucc (tDLList *L)
{
    /*
    ** Posune aktivitu na následující prvek seznamu L.
    ** Není-li seznam aktivní, nedělá nic.
    ** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
    **/
    if ( L->Act )
    {
        if (L->Act == L->First)
        {
            // ztraci aktivitu
            L->Act = NULL;
        }
        else
        {
            L->Act = L->Act->rptr;
        }
    }
}


void DLPred (tDLList *L)
{
    /*
    ** Posune aktivitu na předchozí prvek seznamu L.
    ** Není-li seznam aktivní, nedělá nic.
    ** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
    **/
    if ( L->Act )
    {
        if (L->Act == L->First)
        {
            // ztraci aktivitu
            L->Act = NULL;
        }
        else
        {
            L->Act = L->Act->lptr;
        }
    }

}

int DLActive (tDLList *L)
{
    /*
    ** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
    ** Funkci je vhodné implementovat jedním příkazem return.
    **/
    return (L->Act) ? ( TRUE ) : ( FALSE );
}


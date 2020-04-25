## FLP - 2. projekt 2019/2020 (varianta: Turingův stroj)
###  Alena Tesařová (xtesar36@stud.fit.vutbr.cz)

Druhým projektem do předmětu *FLP* bylo vytvořit program v jazyku *Prolog*. Já jsem si vybrala variantu Turingův stroj tzn. úkolem je vytvořit simulátor nedeterministického Turingova stroje, který na vstupu obdrží pravidla pro Turingův stroj a vstupní obsah pásky. Výstupem bude posloupnost konfigurací stroje.

## Implementace
Algoritmus nejprve rozparsuje vstup. Obsah vstupní pásky je ukládán do seznamu s přidaným počátečním stavem **S** a pravidla jsou dynamicky vložena jako predikáty (prechod) do databáze pomocí assert. Každá konfigurace má tvar **<Seznam před stavem|Stav|Znak|Seznam za>**. Pravidla se vkládají tak, aby byla kompatibilní s tímto stavem konfigurace. 

### Pravidla pro posun doprava
Zde je potřeba ošetřit 3 případy: 1. **Seznam za** má hlavičku a tělo, 2. **Seznam za** má jen hlavičku, 3. **Seznam za** je prázdný
 
     createPredicatesAssert([StateFrom, SymbolAfter, StateNext, 'R']) :- 
    	assert(
    		prechod('right',[BeforeList, StateFrom, SymbolAfter, [Head|Tail]], [ [SymbolAfter|BeforeList], StateNext, Head, Tail]) ), 
    	assert(
    		prechod('right',[BeforeList, StateFrom, SymbolAfter, [Head]], [ [SymbolAfter|BeforeList], StateNext, Head, []]) ),
    	assert(
    		prechod('right',[BeforeList, StateFrom, SymbolAfter, []], [ [SymbolAfter|BeforeList], StateNext, '', []]) ),!.


### Pravidla pro posun doleva
U posunu doleva je potřeba ošetřit 2 případy: 1. **Seznam před** obsahuje hlavičku a tělo, 2. **Seznam před** obsahuje pouze hlavičku.  V případě, že je seznam prázdný, nelze uskuteční posun doleva. Pro jednoduchost je **seznam před** ukládaný v opačném pořadí.

     createPredicatesAssert([StateFrom, SymbolAfter, StateNext, 'L']) :- 
    	assert( 
    		prechod('left', [ [Head|BeforeList], StateFrom, SymbolAfter, EndList], [BeforeList, StateNext, Head, [SymbolAfter|EndList] ] ) ), 
    	assert( 
    		prechod('left', [ [Head], StateFrom, SymbolAfter, EndList], [[], StateNext, Head, [SymbolAfter|EndList] ] ) ), !.

### Pravidlo pro změnu stavu
Pravidlo přepíše stav a aktuální znak pod hlavou.

    createPredicatesAssert([StateFrom, SymbolAfter, StateNext, TapeNext]) :- 
    	assert( prechod('normal',[BeforeList, StateFrom, SymbolAfter, EndList], [BeforeList, StateNext, TapeNext, EndList])), !.
    
## Testování
Testy se nachází ve složce */tests*, je jich celkem 6 a testují referenční vstup, nepovolený přechod na pásce vlevo a posun na pásce, co nejvíce do prava.

**Spuštění** ve složce /tests (předpokládá se Makefile v rodičovské složce):

    ./test.sh

Datum: 25. 4. 2020

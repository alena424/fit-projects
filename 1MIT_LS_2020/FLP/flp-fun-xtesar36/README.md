
## FLP - 1. projekt 2019/2020 (varianta: rka-2-dka)
###  Alena Tesařová (xtesar36@stud.fit.vutbr.cz)

Prvním projektem do předmětu *FLP* bylo vytvořit program v jazyku *Haskell*. Já jsem si vybrala variantu rka-2-dka tedy převod rozšířeného konečného automatu na deterministický automat.

## Struktura programu
Program je členěný do 6 modulů (obsah složky src), kde každý modul má svoji funkci:

 - **Main** - hlavní funkce programu, obsahuje parsování argumentů a volání hlavních částí determinizace
 - **EpsilonRemoval** - odstranění epsilon přechodů z RKA
 - **Determinization** - část determinizace (druhá část je v Main)
 - **Types** - použitý vlastní typ jako např. Rule, NewRule, FiniteMachine
 - **EliminateUnreachable** - odstranění nedostupných stavů z KA
 - **GeneralFunctions** - obecné funkce jako jsou například tisky

## Implementace
Determinizace byla implementovaná podle algoritmů ve studijní podpoře předmětu TIN.
Prvním krokem je správné uložení vstupu do struktury typu FiniteMachine, která obsahuje pravidla typu [Rule]. Pravidla se musí nejprve upravit a odstranit z nich epsilon přechody (pomocí vytvoření epsilon uzávěrů). Po odstranění dostáváme novou struktury pravidel, které je již možné determinizovat. Pro determinizaci se vytvoří tabulka pravidel 2<sup>Q</sup> $\times$  $\Sigma$. Tyto pravidla již nemůžou být typu [Rule], ale ukládají se do nového typu [RuleNew], které se liší tak, že množina stavů počátečních a množina stavů následujících jsou typu [String]. Jelikož nebudeme chtít na výstupu zobrazit všechna pravidla, ale pouze pravidla se stavy, do kterých se lze dostat z počátečního stavu, je potřeba tyto strukturu upravit a nedostupné stavy odstranit. Dále musíme pravidla přejmenovat. Já to dělám tak, že mám pole všech stavů a přejmenovávám stav podle jeho indexu v tomto poli. Tímto způsobem již dostaneme výsledné pole pravidel (RuleNew -> Rule). Počáteční stav se určí jako epsilon uzávěr počátečního stavu a koncové stavy jako sjednocení všech stavů, které obsahují koncový stav.

## Testování
Testy se nachází ve složce */tests*, je jich celkem 16 a jsou testované nedefinované stavy, nedefinovaná abeceda, determinizace na již determinizovaném automatu, bez epsilon přechodů a s epsilon přechody.

**Spuštění** ve složce /tests (pedpokládá se Makefile v rodičovské složce):

    ./test.sh

Datum: 5. 4. 2020

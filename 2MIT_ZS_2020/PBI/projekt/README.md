PBI - projekt
------------------------------
# Zadání č. 2
Implementujte vlastní dokovací algoritmus, který bude vizualizovat průběh přibližování dvou molekul
(jeden uživatelem zvolený protein a jedna libovolná molekula) v Pymolu za účelem nalezení vzájemné polohy
s maximálním počtem kontaktů (pár atomů A,B kde každý atom je z jiné molekuly a vzdálenost |AB| < 5 angstromů) a
minimální střední vzdálenosti sousedících atomů těchto dvou molekul.

Poměr vah těchto dvou kritérií ať je v poměru velikosti molekul (čím větší protein, tím důležitější jsou kontakty).
Není nutné, aby Váš algoritmus našel globálně optimální řešení, musí ale pokud to jde dokovací kriterium zlepšovat a
vypisovat jeho klesající hodnotu na obrazovce.

# Prerekviziti
- vytvořená složka *img* v adresáři, kde se program spouští (tam se budou ukládat screenshoty)
- nainstalovaný Pymol
- soubory: projekt.py, pairwiseidstances.py, GLY.cif

# Spuštění
```
run([pbi_id, step, alg_depth])
```
pbi_id - identifikátor z PDB (default 2c0k)
step - krok algoritmu (default 2)
alg_depth - míra zanoření algoritmu (default 2)
 
nejjednodušší spuštění:
```
run()
```
Pozn: mělo by skončit tak do asi 2 minut

# Princip
Nejprve dojde k načtení proteinu s názvem `protein_name` z PDB. Jako testovací molekulu používám Glycin (soubor `GLY.cif`). Jelikož se na začátku spojí molekula s proteinem je potřeba molekulu oddálit (proto se na začátku mění její souřadnice hned po načtení). Následují tyto kroky:

1. Přiblížení molekuly k proteinu až do vzdálenosti, kdy vidíme nějaké sousedy `find_molecule()`
2. Dokování `find_solution()`
3. Vypsání nejlepšího nalezeného skóre - informace je uložena v `global_score`

## Přiblížení molekuly k proteinu
Jelikož byla molekula posunuta po ose x, přibližování probíhá také po ose x, dokud nenajdeme sousedy nebo x není záporné.

## Dokování - find_solution()
Molekulu posunujeme do směrů `moves` (kterých je celkem 27 a krok je jeden z parametrů). V každém směru spočítáme *počet kontaktů* (do 5 angstromů), *průměrnou vzdálenost* od sousedů (sousedy jsou definovány do vzdálenosti 15 angstromů) a jestli se někde nachází *kolize* (funkce `is_overlapping`). Ze všech směrů získáme ten s nejlepším skóre a uložíme si jeho pozici (`best_position`) a skóre (`global_score`). Spustíme algoritmus znova s touto pozicí. Pokud nedojde ke zlepšení je algoritmus ukončen.

Jelikož toto řešení nedávalo až tak dobré výsledky, byl algoritmus trochu vylepšen:
V případě, že získaná nová pozice nedává lepší výsledek než nejlepší skóre, dostane tato pozice další pokus najít lepší řešení. 
Volá se znova `find_solution` akorát s větším `krokem` o jedna a zvětší se i `level` - ten udává aktuální zanoření. V případě, že bylo nalezené lepší řešení, pokračuje se standardně tímto směrem. Maximální zanoření je dáno parametrem `alg_depth`. Tímto zajistíme, že nedojde k zacyklení.

### Kolize
Kolize se řeší přímo ve funkci `pairwise_dist()`, kde se volá `is_overlapping()`. Počítá se jako součet *van der walsových sil* atomů, aby nebyl větší než skutečná vzdálenost atomů.

## Počítání skóre
Funkce na počítání skóre vypadá následovně:

```
(number_of_contact * protein_bonus)/100 + (1/average_distance)
```
Započítává se průměrná vzdálenost a počet kontaktů. Jelikož je potřeba minimální vzdálenost a maximální počet kontaktů, vzdálost se bere jako 1/average_distance a počet kontaktů se naopak dělí 100 (100 proto, aby hodnoty byly zhruba ve stejném měřitku, získáno na základě experimentů). Dále je zde `protein_bonus`. Hodnota nabývá čísel 1-2 podle velikost proteinu. Pokud je protein malý (zde považuji malý protein za počet atomů <= 2000), tak nedostane počet kontaktů žádný bonus. Pokud je protein velký (maximum je 8000 atomů - hodnota podle největšího existujícího proteinu), hodnota získaných kontaktů se může až zdvojnásobit.

Během vykonávání algoritmu se snažím skóre zvyšovat (čím větší, tím lepší).

## Zobrazení výsledků
Skóre je vypisování na obrazovku průběžně. Na konci je napsáno to nejlepší skóre. Zároveň se v různých okamžicích berou snímky, ze kterých je pak možné vytvořit gif, tímto způsobem:

```
$ cd img
$ convert -delay 100 -loop 0 $(ls -1 *.png | sort -V) animation.gif
```

Během testování braní snímků jsem měla problém s tím, že se mi všechny snímky vyfotily, až když algoritmus skončil - pak byly všechny stejné a nebylo možné vidět průběh přibližování. Proto jsem přidala navíc funkci `mpng` nad `png` - ta mi zajistí, že se snímek vytvoří ve správný okamžik (nevím proč se to takto chová).

## Pairwise distance
Funkce je použita z oficiálního zdroje pymolu: https://pymolwiki.org/index.php/Pairwise_distances, pozměnila jsem ji tak, aby vracela počet kontaktů, průměrnou vzdálenost a detekovala kolize. 

# Testování
Pro testování jsem si vyrobila funkci `test`, kde jsem testovala výsledky různé délky kroku (1 až 10) a zanoření (2 až 8). Z experimentů vyplynulo, že hodnota zanoření neměla až takový vliv na celkový výsledek. Nejlépe dopadl krok s délkou 3, který získal skóre přes 1 (testováno na proteinu 2c0k)

Testováno na Fedoře 32.

# Autor
Alena Tesařová (xtesar36@stud.fit.vutbr.cz)

# Datum
30.12.2020

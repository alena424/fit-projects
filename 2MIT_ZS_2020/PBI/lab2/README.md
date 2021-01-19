## PBI - Pokročilá bioinformatika
## Domácí úkol č. 2
Alena Tesařová (xtesar36)
25.11.2020
----------------------------------------------

Triplet byl vytvořen pomocí nástroje PyMol před build:
- Build -> Alanin
- Build -> GLycin
- Build -> Alanin

A byl následně vyexportován pod názvem *triplet*, abych ho mohla použít v scriptu.
Scrpt má 2 funkce: runAll a run, kde runAll volá funkci run t krát (defaultně je nastaveno na 10) a shromažďuje statistiky (počet kolizí, max, min, průměr). Funkce runAll má navíc argument *control_collision* (boolean) v případě, kdy chceme kontrolovat kolizi.

příklad spuštění:

```
run du2.py
runAll(t=10,p=1,m=0,control_collision=False)
```

poznámka: očekává se, že soubor triplet.cif je ve stejné složce jako script

Získané hodnoty
------------------------------------------------

#### Verze bez kolize
Rotace byla provedena 10000x.

*Minimum*: 0.31
*Maximum*: 5.57

#### Verze s kolizí
- minimum je ovlivněno hodnotou *m*
- maximum vychází stejně, jak u verze bez detekce kolizí
# Zadání
Importujte do R data o pozici všech HOX genů člověka a myši z databáze Ensembl, vytvořte objekt GRanges pro tato data a nechte je zobrazit podél chromosomů (ggbio, GenomeGraphs, genoPlotR nebo quantsmooth). Vyznacte useky, ktere vykazuji podobne usporadani u obou organismu (pokud takove budou) a srovnejte je s informacemi o syntenii mezi genomy mysi a cloveka (https://www.mun.ca/biology/scarr/MGA2-11-33smc.html). Odevzdávejte kód a obrázky s krátkým komentářem dle potřeby.

# Implementace
Přiložena v souboru xtesar36.R.

# Výsledky
Výsledkem jsou dva grafy - jeden zobrazující rozložení genů HOX u myši a druhý u člověka. Porovnání jde vidět na obrázku final.png.
Vyznačené barvy odpovídají syntenické mapě podle následující odkazu: https://www.mun.ca/biology/scarr/MGA2-11-33smc.html. Podle této mapy a vytvořeného grafu pro myš a člověka (pomocí autoplot viz kód) jsem došla k závěrům, že podobné uspořádání mají tyto úseky:

- hox geny chr7 člověka s hox geny chr6 myši (HoxA)
- hox geny chr12 člověka s hox geny chr15 myši (HoxC)
- hox geny chr2 člověka s hox geny chr2 myši (HoxD)
- hox geny chr17 člověka s hox geny chr11 myši (HoxB)

Pořadí Hox genů bylo potvrzeno manuálně, že u těchto dvojic taktéž odpovídá.

# Hodnoty
Pořadí odpovídá důkaz na HOXA:
HOXA člověk
      start_position end_position strand external_gene_name chromosome_name
20451       27092993     27096000     -1              HOXA1               7
20477       27100354     27102686     -1              HOXA2               7
20479       27106184     27152581     -1              HOXA3               7
20506       27128507     27130780     -1              HOXA4               7
20520       27141052     27143681     -1              HOXA5               7
20522       27145396     27150603     -1              HOXA6               7
20527       27153716     27157936     -1              HOXA7               7
20530       27162438     27175180     -1              HOXA9               7
20540       27170592     27180261     -1             HOXA10               7
20550       27181157     27185232     -1             HOXA11               7
20576       27193503     27200091     -1             HOXA13               7

HOXA myš
      start_position end_position strand external_gene_name chromosome_name
30308       52155590     52158317     -1              Hoxa1               6
30833       52189671     52191753     -1              Hoxa4               6
30840       52201754     52204587     -1              Hoxa5               6
30845       52162417     52164831     -1              Hoxa2               6
30864       52206288     52208722     -1              Hoxa6               6
30897       52169062     52213336     -1              Hoxa3               6
30922       52214491     52221854     -1              Hoxa7               6
31860       52223100     52231089     -1              Hoxa9               6
32117       52231197     52240854     -1             Hoxa10               6
32155       52242106     52245810     -1             Hoxa11               6
32496       52257694     52260880     -1             Hoxa13               6



# Autor
Alena Tesařová (xtesar36)
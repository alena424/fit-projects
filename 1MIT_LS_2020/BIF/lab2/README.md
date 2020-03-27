# Zarovnání sekvencí
Janka Puterová ([iputerova@fit.vutbr.cz](mailto:iputerova@fit.vutbr.cz))
Fakulta informačních technologií, Vysoké učení technické v Brně

## Dotlet

Seznamte se s programem [Dotlet](https://dotlet.vital-it.ch/). Zadejte programu různé (např. náhodné) vstupní sekvence ([nukleotidů](http://www.bioinformatics.org/sms2/random_dna.html) / [aminokyselinami](http://www.bioinformatics.org/sms2/random_protein.html)) a vyzkoušejte si vliv vstupních parametrů (jako jsou typ skórovací matice nebo velikost posuvného okýnka) na výsledek dotplot grafu.

### Region s nízkou složitostí

Tento příklad ukazuje vliv regionu s nízkou složitostí na výsledek dotplot grafu.

1.  Jako vstupní sekvenci použijte antigenu [Plasmodium falciparum](data/dotlet_lowcomp.txt) (parazit způsobující malárii) a zarovnejte ji vůči sobě samé.
2.  Zvolte matici Blosum45 a velikost okýnka 15.
3.  Spusťte výpočet a upravte si světlost výsledného grafu např. na -40 a +40 (posuvníky pod histogramem).
4.  Ve výsledném grafu by jste měli spatřit tmavou oblast odpovídající sekvenci identických znaků. Přemístěním kurzoru do tmavé oblasti se vám objeví ve spodní části okna podrobnější výpis sekvence.

_Otázky:_

*   Jak dlouhá a z jakých znaků se skládá uvedená oblast?

### Opakování

Tento příklad ukazuje zajímavé vlastnosti, kterých si můžete všimnout při zarovnání sekvence oproti sobě samé.

1.  Jako vstupní sekvenci použijte [SLIT protein](data/dotlet_rep.txt) Drosophila melanogaster (vinná muška).
2.  Zvolte matici Blosum62 a velikost okýnka 15.
3.  Spusťte výpočet a upravte si světlost výsledného grafu např. na 0 a +90.
4.  Ve výsledném grafu by jste měli spatřit dva shluky, jeden tvořený z delších a druhý z kratších opakujících se podřetězců (zkráceně opakování).

_Otázky:_

*   Jak v dotplot grafu poznáte opakování?
*   Kolik jste napočítali delších a kolik kratších opakování?

## Dynamické programování

### Míra identity u náhodných biologických sekvencí

1.  Vytvořte si 2 náhodné sekvence [nukleotidů](http://www.bioinformatics.org/sms2/random_dna.html) a [aminokyselinami](http://www.bioinformatics.org/sms2/random_protein.html).
2.  Dále budeme pracovat s online nástroji pro globální ([Needle Nucl](https://www.ebi.ac.uk/Tools/psa/emboss_needle/nucleotide.html) a [Needle Prot](https://www.ebi.ac.uk/Tools/psa/emboss_needle/)) a lokální ([LALIGN Nucl](https://www.ebi.ac.uk/Tools/psa/lalign/nucleotide.html) a [LALIGN Prot](https://www.ebi.ac.uk/Tools/psa/lalign/) ) zarovnání nukleotidových/aminokyselinových sekvencí.
3.  Pomocí nástrojů Needle resp. LALIGN vykonajte zarovnání vybraných nukleotidových/aminokyselinových sekvencí. Při zarovnávání aminokyselinových sekvencí zvolte matici BLOSUM50\.  
    Jinak ponechte původní nastavení. (LALIGN nabízí i pěkný grafický výstup v záložce _Visual Output_)
4.  Sledujte hodnoty vyjadřující míru identity (Needle) a parametr E (pouze u LALIGN).

_Otázky:_

*   Co vyjadřuje parametr E?
*   Jaké míry identity a parametru E je obvykle dosahováno u náhodných nukleotidových sekvencí?
*   Jaké míry identity a parametru E je obvykle dosahováno u náhodných sekvencí aminokyselin?

### Objevení podobnosti mezi onkogeny

Russell Doolittle byl průkopníkem v oblasti algoritmů pro analýzu sekvencí v druhé polovině 70 a první polovině 80 let. Doolittle používal tehdejší databáze biologických sekvencí pro své experimenty s geny a hledání jejich funkce na základě podobnosti. V následujícím cvičení si zopakujeme kroky, které Doolittle provedl při objevu funkce **v-mos** onkogenu viru _Moloney Murine Sarcoma_. Ne dlouho poté, co byl nasekvenován v-mos v Salk Institutu, studovala skupina vědců vztah mezi **v-src** onkogen viru _Rous Sarcoma_ a v-mos onkogenu. První pokusy o hledání podobnosti však dopadly neúspěšně.

1.  Pro tenhle úkol budeme potřebovat nástroj [Sixpack](https://www.ebi.ac.uk/Tools/st/emboss_sixpack/) pro překlad nukleotidové sekvence na aminokyselinovou.
2.  S použitím nástrojů Needle resp. LALIGN pro globální resp. lokální zarovnání analyzujte podobnost obou nukleotidových sekvencí [vmos.fasta](data/vmos.fasta) a [src.fasta](data/src.fasta).
3.  Na základě míry identity a parametru E zhodnoťte výsledky zarovnání.
4.  Dále použijte nástroj Sixpack který dokáže přeložit sekvenci nukleotidů na aminokyseliny pro překlad sekvencí _v-mos_ a _v-src_. Ponechte původní nastavení. Všimněte si, že nástroj Sixpack provede celkem 6 překladů s ohledem na různou počáteční pozici překladu fram-u. Tři překlady provede s přímou sekvencí a tři překlady provede s komplementární a reverzovanou sekvencí (simulace komplementárního vlákna DNA).
5.  Z šesti překladů obou sekvencí vyberte takové, které by mohli vést na co největší míru podobnosti (říďte se tabulkou na konci výstupu, výsledné přeložené sekvence najdete v záložce _Tool Output_).
6.  Proveďte globální a lokální zarovnání přeložených sekvencí pomocí nástrojů Needle/LALIGN.
7.  Na základě míry identity a parametru E zhodnoťte výsledky zarovnání.
8.  Pokud si nejste jisti, zda jste vybrali správné fram-y v bodě 5, vyberte jiné a opakujte experiment. Nápověda: hledáme takové posuny, které vedou na překlad jednoho genu v protein (jeden start/stop kodon)

_Otázky:_

*   Jaké fram-y jste vybrali?
*   Jak hodnotíte výsledky zarovnání nukleotidových sekvencí?
*   Nalezli jste lepší zarovnání v případě přeložených sekvencí?

## BLAST

### Hledání původu DinoDNA

1.  Film Michael Crichtona o klonování dinosaurů, Jurský park, ukazuje domnělou DNA sekvenci dinosaura. Identifikujte skutečný zdroj [této](data/dino_dna1.fasta) DNA sekvence s využitím programu [BLAST](https://blast.ncbi.nlm.nih.gov/Blast.cgi?PROGRAM=blastn&PAGE_TYPE=BlastSearch&LINK_LOC=blasthome) a NCBI databáze všech nukleotidů **nr**.
2.  Vědec NCBI Mark Boguski však upozornil na to, že jeho sekvence byla určitě kontaminovaná a zásobil Michaela Crichtona lepší sekvencí, pro pokračování tohoto filmu z názvem The Lost World. Identifikujte zdroj [této](data/dino_dna2.fasta) sekvence.

_Otázky:_

*   Nalezl Mark lepší sekvenci než Michael? Proč?
*   Mark zabudoval do své sekvence také své jméno MARK. Nalezněte toto jméno v sekvenci.

### Hledání komplementárních sekvencí

1.  S využitím databáze [NCBI GenBank](http://www.ncbi.nlm.nih.gov/genbank/) si stáhněte sekvenci nukleotidů libovolného lidského genu napr. KRAS (postačí prvních 1000 znaků genu)
2.  S využitím následujícího webového [nástroje](http://www.bioinformatics.org/sms/rev_comp.html) si ke vstupnímu genu vytvořte:
    *   reverzní sekvenci,
    *   komplementární sekvenci,
    *   reverzní+komplementární sekvenci.
3.  S využitím [BLASTu](http://blast.ncbi.nlm.nih.gov/Blast.cgi) vyhledejte v **nr** databázi všechny výskyty vytvořených sekvencí

_Otázky:_

*   Shodují se výsledky pro všechny alternativy vstupní sekvence? Zdůvodněte proč.
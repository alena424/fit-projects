### Fylogenetické stromy a viacnásobné zarovnanie

##### Janka Puterová ([iputerova@fit.vutbr.cz](mailto:iputerova@fit.vutbr.cz))
##### Fakulta informačních technologií, Vysoké učení technické v Brně

## Potrebné nástroje
Na cvičení budeme pracovať s nasledovnými nástrojmi:

*   nástroj Mega v5.05 na komplexnú fylogenetickú analýzu - dostupný [tu](https://www.fit.vutbr.cz/study/courses/BIF/private/labs/MEGA5.zip) (Win)
*   v prípade problémov s prehliadaním Postscriptu použite online nástroj [http://view.samurajdata.se/](http://view.samurajdata.se/)

## Viacnásobné zarovnanie sekvencie cytochromu C a základná fylogenetická analýza

1.  Stiahnite si aminokyselinové sekvencie nasledovných organizmov z [NCBI](http://www.ncbi.nlm.nih.gov/protein/) a uložte ich do jedného multifasta súboru:
    *   Equus caballus (NP_001157486.1)
    *   Homo sapiens (NP_061820.1)
    *   Bos taurus (NP_001039526.1)
    *   Gallus gallus (NP_001072946.1)
    *   Canis lupus (AEP27248.1)
    *   Sus scrofa (NP_001123442.1)
    *   Pan troglodytes (NP_001065289.1)
    *   Taeoniopygia guttata (NP_001137145.2)
    *   Novosphingobium sp. PP1Y (WP_013831814.1)
2.  Sekvencie importujte do programu MEGA a analyzujte:
    *   Kliknite File->Open a File/Session, How would you like to open this fasta file - kliknite Align, potom z menu Alignment exploreru vyberte Alignment -> Align by ClustalW (ak sedíte v sudom stĺpci), resp. Muscle (ak sedíte v lichom stĺpci). **Vyšiel susedovi rovnaký alignment?**
    *   Vizuálne porovnajte jednotlivé zarovnané sekvencie. **Ktorá zo sekvencií sa zarovnala vzhľadom k ostatným nesprávne/je vysoko divergentná vzhľadom k ostatným? Do akej skupiny organizmov patrí?**
    *   **V koľkých pozíciách sa líšia zvyšné sekvencie?**
    *   V Alignment exploreri vyberte v menu Data->Export alignment->Mega format a uložte.
    *   Zatvorte Alignment explorer (alignment už znova nemusíte ukladať)
    *   Vytvorte v programe Mega jednoduchú fylogenetickú analýzu - ak ste v sudom stĺpci, použite algoritmus NJ, ak ste v lichom stĺpci, použite Maximum Parsimony (Phylogeny->Construct NJ/Maximum parsimony).

        Pre algoritmus **NJ** použite:

        *   Model/Method: Equal Input Model
        *   Gaps/Missing Data Treatment: Partial deletion

        Pre algoritmus **Maximum Parsimony** použite:

        *   Gaps/Missing Data Treatment: Partial deletionOstatné parametre ponechajte implicitné. V TreeExploreri sa Vám zobrazí vygenerovaný fylogenetický strom. Zvoľte v menu TreeExploreru Subtree->Root a kliknite na vetvu Novosphingobium. Týmto strom "zakoreníte" a označíte Outgroup.

        **Zhoduje sa topológia stromov vytvorených pomocou MP a NJ? V čom sa líšia? Diskutujte s Vašimi susedmi výsledky.**

    *   **Vypátrajte české/slovenské názvy skúmaných organizmov. Korešponduje vzniknutý fylogenetický strom s ich fenotypom?**


### Fylogenetická analýza človeka s príbuznými druhmi

1.  Zostavte si jednoduchý dataset (multifasta) pozostávajúci z DNA sekvencií mitochondrial d-loop nasledovných druhov, ktoré si stiahnite z [NCBI](http://www.ncbi.nlm.nih.gov/nuccore/). Ako popis sekvencií v multifasta súbore použite české/slovenské názvy organizmov.
    *   Pongo abelii (FR717938.1) alias orangutan
    *   Pan troglodytes (GU136845.1) alias šimpanz
    *   Homo sapiens (HM009355.1) alias človek dnešného typu
    *   Gorilla graueri (AF050738.1) alias gorila
    *   Homo sapiens neanderthalensis (FM866397.1) alias neandertálec
2.  Sekvencie analyzujte pomocou programu MEGA nasledovne:
    *   Kliknite File->Open a File/Session, How would you like to open this fasta file - kliknite Align, potom z menu Alignment exploreru vyberte Alignment -> Align by ClustalW.
    *   V Alignment exploreri vyberte v menu Data->Export alignment->Mega format a uložte.
    *   Zatvorte Alignment explorer (alignment už znova nemusíte ukladať)
    *   Analyzujte zarovnanie pomocou fylogenetickej analýzy s využitím bootstrapingu. Ak sedíte v lichom stĺpci, použite Phylogeny -> Construct/Test Maximum likelihood tree, ak sedíte v sudom stĺpci, použite Phylogeny -> Construct/Test Neighbor-Joining tree a nastavte parametre:
        *   Test of phylogeny: Bootstrap method
        *   No. of Bootstrap Replications: 1000
        *   Model/Method: Kimura 2-parameter model
        *   Gaps/Missing Data Treatment: Partial deletionOstatné parametre ponechajte implicitné. V TreeExploreri sa Vám zobrazí vygenerovaný fylogenetický strom a consensus strom.

        **Majú obidva stromy rovnakú topológiu? Čo prezrádzajú bootstrapové hodnoty? Čo je najpodobnejšie človeku? Majú na základe získaného stromu neandertálec a človek dnešného typu priameho spoločného predka?** Viď [Neandertálec](http://humanorigins.si.edu/evidence/human-fossils/species/homo-neanderthalensis) a [Primáty](http://whozoo.org/mammals/Primates/primatephylogeny.htm).
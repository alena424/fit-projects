# Bioinformatické databáze
Miloš Musil ([imusilm@fit.vutbr.cz](mailto:imusilm@fit.vutbr.cz))
Fakulta informačních technologií, Vysoké učení technické v Brně

## Sprkovitá anémie

Seznamte se s dědičným genetickým onemocněním zvaným srpkovitá anémie (sickle cell anemia).

*   [text](https://en.wikipedia.org/wiki/Sickle-cell_disease)
*   [video 1](http://www.youtube.com/watch?v=9UpwV1tdxcs)
*   [video 2](http://www.youtube.com/watch?v=9AHFHleYwdU&feature=related)

## Hledání v databázích nukleotidových sekvencí

Vyhledejte mRNA lidského genu beta-globinu (Homo sapiens hemoglobin, beta, HBB), který je součástí většího proteinu hemoglobinu. Použijte databázi [GenBank](http://www.ncbi.nlm.nih.gov/sites/entrez?db=nuccore). Určete a stáhněte kódující oblasti tohoto genu a sekvenci proteinu, který je tímto genem kódován.

1.  Jaký přístupový kód má tento gen?
2.  Určete kódující sekvenci genu beta-globin a stáhněte ji ve FASTA formátu.
3.  Jaký je přístupový kód proteinové sekvence kódované genem beta-globin?
4.  Stáhněte sekvenci proteinu kódovaného genem beta-globin ve FASTA formátu.
5.  Stáhněte článek věnující se genetickému onemocnění beta-talasemie (beta-thalassemia), jenž je důsledkem mutace v lidském hemoglobinu.

## Hledání v databázích proteinových sekvencí

Vyhledejte záznam o proteinu beta-globin (Homo sapiens hemoglobin, beta, HBB) v proteinové databázi [UniProtKB/Swiss-Prot](http://www.uniprot.org/).

1.  Jaký přístupový kód má tento protein?
2.  Z kolika aminokyselin je tento protein tvořen?
3.  Jakou funkci má tento protein?
4.  Do jaké rodiny tento protein patří?
5.  Jaký efekt má mutace E7V?
6.  Stáhněte abstrakt článku popisujícího strukturu neokysličeného hemoglobinu s mutací způsobující srpkovitou anémii.
7.  Zjistěte RS identifikátor pro mutaci E7V.
8.  Stáhněte sekvenci tohoto proteinu ve FASTA formátu.
9.  Stáhněte z GenBank DNA sekvenci, která kóduje tento protein.

## Informace o SNP mutacích
V databázi [dbSNP](http://www.ncbi.nlm.nih.gov/SNP/) vyhledejte informace o mutaci způsobující srpkovitou anémii (využijte RS identifikátor z předchozího úkolu).

1.  Ověřte patogenicitu v databázi Clinvar.
2.  Povšimněte si rozdílných pozic v různých verzích genomových map.
3.  V jakém regionu lidského genomu se mutace nachází?


## Porovnání textových vyhledávacích systémů

Vyhledejte záznam o proteinu beta-globin (Homo sapiens hemoglobin, beta, zkratka HBB) s použitím vyhledávacího systému [GQuery](http://www.ncbi.nlm.nih.gov/sites/gquery).

1.  Prohlédněte si seznam získaných výsledků. Nalezněte záznam spojený s mutací beta-globinu pro nemoc alpha-thalassemia v databázi <span class="kontextNapoveda" title="Zkratka pro Online Mendelian Inheritance in Man">OMIM</span>.

## Informace o genomových projektech

V databázi [Genomes OnLine Database](http://www.genomesonline.org/) (GOLD) zjistěte informace o dokončených a probíhajících genomových projektech.

1.  Kolik bakteriálních a eukaryotických genomů bylo dosud osekvenováno a publikováno?

## Vizualizace struktur molekul

V největší strukturní databázi [PDB](http://www.pdb.org) hledejte informace o proteinu beta-globin.

1.  V databázi PDB najděte článek popisující lidský beta-globin (tip: PDB-101, human hemoglobin). Podívejte se na animaci ukazující rozdíl mezi okysličenou a neokysličenou verzí. V článku nalezněte odkazy na PDB záznamy s okysličenou a neokysličenou verzí a také na verzi s genovou mutací způsobující srpkovitou anémii.
2.  Srovnejte rozlišení a R-faktory výše nalezených struktur. Kterou ze struktur lze považovat za nejkvalitnější.
3.  Stáhněte pdb soubor libovolné struktury.
4.  Stáhněte sekvenci ve formátu FASTA. Použijte staženou sekvenci pro vyhledání struktury Beta-Globinu (tip: advanced search).
5.  Vizualizujte jeden z těchto proteinů prostřednictvím nástroje / apletu JsMol.
6.  Vyzkoušejte si různá zobrazení vybrané struktury.
7.  Uložte si některá zobrazení ve formátu PNG.

## Databáze PDBSum

V databázi [PDBSum](http://www.ebi.ac.uk/thornton-srv/databases/cgi-bin/pdbsum/GetPage.pl?pdbcode=index.html) vyhledejte strukturu [2HHB](http://www.rcsb.org/pdb/explore/explore.do?structureId=2hhb).

1.  Prohlédněte si Ramachandrův diagram. Jedná se o dobře definovanou strukturu? Srovnejte s jinými strukturami (např. 1CHR).
2.  V záložce Proteins si prohlédněte informace o sekundární struktuře. Jaké z nich můžeme vyvodit závěry?
3.  Jaké další informace lze dohledat v PDBSum?
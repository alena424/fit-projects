ITS - 1. projekt BDD scénáře
===========================

Popis testovacích případů - Nákupní proces produktu
---------------------------------------------------
__Alena Tesařová__

Pro testování jsem si vybrala nákupní proces.

Proces jsem rozdělila do tří částí: vybírání produktu, nákupní košík a samotný proces objednávky.

## Vybírání produktu - choosingItem.feature
Abychom mohli vybrat produkt musíme se napřed dostat na stránku online obchodu a pak vyhledat produkt.

Nalézt produkt můžeme buď přes kategorie v hlavním menu anebo přes vyhledávací okno. Měli bychom dostat stejné výsledky po zadání "Cameras" do vyhledávače a po kliknutí na "Cameras" v menu. Jelikož v aplikaci nefunguje vyhledávač, nezahrnula jsem test s vyhledávačem do testů, jelikož neznám správné chování.
V kategoriích testuji 3 případy: v kategorii nejsou žádné produkty, po vybrání kategorie se zobrazí seznam subkategorií a kategorie s produkty bez subkategorie.

## Nákupní košík

Zde testuji moment, kdy je košík prázdný a kdy se zde nachází jeden produkt a to fotoaparát Nikon. Testuji zvětšování množství produktu v košíku a odstranění produktu. Nějaké produkty nejsou již na skladě v požadovaném množství, u těchto produktů očekáváme varování, který se zobrazí v košíku.

V košíku můžeme dále zadat kupón, odhadnout poplatky za dopravu, anebo použít dárkový voucher. Kupón a vouchery testuji, když jsou validní a nevalidní => podle toho určeno chování testů. U odhadu poplatku za dopravu mě zajímá pouze to, jestli se podařilo vytvořit odhad (jestli se zobrazí hláška). 

## Objednávka
Do objednávky vstupuji buď jako již registrovaný uživatel nebo nový uživatel (2 testovací případy). Jinak se celková objednávka skládá z 6 kroků. V prvním kroku otestuji vstup nového uživatele a již registrovaného. Ve druhém kroku testuji zadávání nové adresy: jednou bez požadovaných dat, jednou při zadání příliš dlouhého jména a správné zobrazení třetího kroku. U třetího kroku testuji správné zobrazení kroku čtvrtého po zmáčknutí Continue. Ve čtvrtém správný přechod do pátého krouk. V pátém kroku testuji objevení chybové hlášky při nezaškrtnutí souhlasu čtení podmínek a přechod do šestého kroku. V šestém kroku testuji samotné potvrzení objednávky. 



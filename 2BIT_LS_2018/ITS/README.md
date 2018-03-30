ITS - 1.projekt BDD scénáře
===========================
Alena Tesařová
--------------


Popis testovacích případů - Nákupní proces produktu
---------------------------------------------------

Nákupní proces produktu jsem rozdělila do 3. částí: vybírání produktu, nákupní košík a samotný proces objednávky.

# Vybírání produktu
Nalézt produkt můžeme buď přes kategorie v hlavním menu anebo přes vyhledávací okno. Měli bychom dostat stejné výdledky po zadání "Cameras" do vyhledávače a po kliknutí na "Cameras" v menu. Jelikož v aplikaci nefunguje vyhledávač, nezahrnula jsem test s vyhledávačem do testů, jelikož neznám správné chování. Testovací produkt jsem vybrala fotoaparát Nikon D300, u kterého zvolím počet a vložím do košíku.

# Nákupní košík
Zde testuji moment, kdy je košík prázdný a kdy se zde nachází jeden produkt a to fotoaparát Nikon. Testuji zvětšování množství produktu v košíku a odstranění produktu. Nějaké produkty nejsou již na skladě v požadovaném množství, u těchto produktů očekáváme warning, který se zobrazí v košíku.
V košíku můžeme zadat kupón, odhadnout poplatky za dopravu a použít dárkový voucher. Kupón a vouchery testuji validní a nevalidní. U odhadu oplatku za dopravu mě zajímá pouze to, jestli se podařilo vytvořit odhad (jestli se zobrazí hláška). 

# Objednávka
Do objednávky vstupuji buď jako již regitrovaný uživatel nebo nový uživatel (2 testovací případy). Jinak se celková objednávka skládá z 6 kroků. V prvním kroku otestuji vstup nového uživatele a již registrovaného. Ve druhém kroku testuji zadávání nové adresy: jednou bez požadovaných dat a jednou při zadání příliš dlouhého jména. U třetího kroku testuji správné zobrazení kroku čtvrtého po zmáčknutí Continue. V pátem kroku testuji objevení chybové hlášky při nezaškrknutí souhlasu čtení podmínek a v 6. kroku testuji samotné potvrzení objednávky. 

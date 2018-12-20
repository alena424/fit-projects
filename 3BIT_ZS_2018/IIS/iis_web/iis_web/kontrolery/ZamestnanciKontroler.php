<?php

class ZamestnanciKontroler extends Kontroler{
    
    public function zpracuj($parametry) {
        $this->overUzivatele( array( 'spolumajitel' ) );
        
        $this->pohled = 'sprava_zamestnancu';
        if ( $_GET['novy_zamestnanec'] ){
            $this->pohled = 'novy_zamestnanec';
        }
        $this->hlavicka['titulek'] = 'Správa zaměstnanců';
        $spravceUzivatelu = new SpravceUzivatelu;
        $this->data['pozice_pop'] = $spravceUzivatelu->vratIdNazevPozice();
        
        
        if ( $_GET[ 'novy_zamestnanec' ] && $_GET[ 'uloz_zamestnance' ] ) {
            $this->novyZamestnanec( $spravceUzivatelu );
        } else if ( $_GET[ 'zrus_uziv' ] ) {
            $this->zrusZamestnance( $spravceUzivatelu );
        } else if ( $_GET[ 'uloz_uziv' ] ) {
            //print_r('dsds');
            $this->upravZamestnance( $spravceUzivatelu );
        }
        
        if ( $_GET && ( $_GET[ 'hledej_vse' ] || $_GET[ 'hledej' ] ) ) {
            if ( $_GET[ 'hledej_vse' ] ) {
                $uzivatele = $spravceUzivatelu->vratVsechnyUzivatele();
            } else {
                $uzivatele = $spravceUzivatelu->vratVsechnyUzivatele( $_GET[ 'prijmeni' ], $_GET[ 'pozice' ] );
            }
            
            $this->data[ 'vysledek' ] = array();
            
            if ( ! empty( $uzivatele ) ){
            foreach ( $uzivatele as $radek ) {
                array_push( $this->data[ 'vysledek' ], array(
                    'id_uziv' => $radek[ 'id_zam' ],
                    'jmeno' => $radek[ 'jmeno' ],
                    'prijmeni_uziv' => $radek[ 'prijmeni' ],
                    'rodne_cislo' => $radek[ 'rodne_cislo' ],
                    'adresa' => $radek[ 'adresa' ],
                    'pozice_uziv' => $radek[ 'pozice' ],
                    'login' => $radek[ 'login' ]
                    )
                );
            }
           } else {
               $this->pridejZpravu("Nenalezen žádný zaměstnanec", ZPRAVA_CHYBA);
           }
        }
    }
    
    public function novyZamestnanec( $spravceUzivatelu ) {
        $parametry = array(
            'jmeno' => 'Jméno',
            'prijmeni' => 'Příjmení',
            'rodne_cislo' => 'Rodné číslo',
            'adresa' => 'Adresa',
            'pozice' => 'Pozice',
            'login' => 'Login',
            'heslo' => 'Heslo'
        );
        $chyba = 0;
        foreach ( $parametry as $parametr => $hodnota ) {
            if ( ! $_GET[ $parametr ] ) {
                $this->pridejZpravu( "Nesprávně zadaný údaj: " . $hodnota , ZPRAVA_CHYBA);
                $chyba++;
                continue;
            }
            $this->data[ $parametr ] = $_GET[ $parametr ];
        }
        
        if ( $chyba ) {
            return;
        }
        
        if ( $spravceUzivatelu->novyZamestnanec(
            $_GET[ 'jmeno' ], $_GET[ 'prijmeni' ], $_GET[ 'rodne_cislo' ],
            $_GET[ 'adresa' ], $_GET[ 'pozice' ], $_GET[ 'login' ],
            $_GET[ 'heslo' ]
        ) ) {
            $this->pridejZpravu( "Při zakládání nového zaměstnance došlo k chybě.", ZPRAVA_CHYBA );
            
        } else {
            $this->pridejZpravu( "Nový zaměstnanec byl úspěšně založen.", ZPRAVA_OK );
        }
    }
    
    public function zrusZamestnance( $spravceUzivatelu ) {
        //print_r( $_GET);
        if ( $spravceUzivatelu->zrusZamestnance( $_GET[ 'id' ] ) ) {
            $this->pridejZpravu( "Při rušení zaměstnance " . $_GET[ 'jmeno' ] .
                " " . $_GET[ 'prijmeni' ] .  " došlo k chybě.", ZPRAVA_CHYBA );
        } else {
            $this->pridejZpravu( "Zaměstnanec " . $_GET[ 'jmeno' ] .
                " " . $_GET[ 'prijmeni' ] .  " byl úspěšně zrušen.", ZPRAVA_OK );
        }
    }
    
    public function upravZamestnance( $spravceUzivatelu ) {
        $parametry = array(
            'jmeno' => 'Jméno',
            'prijmeni_uziv' => 'Příjmení',
            'rodne_cislo' => 'Rodné číslo',
            'adresa' => 'Adresa',
            'pozice_uziv' => 'Pozice',
            'login' => 'Login',
        );
        $chyba = 0;
        foreach ( $parametry as $parametr => $hodnota ) {
            if ( ! $_GET[ $parametr ] ) {
                $this->pridejZpravu( "Nesprávně zadaný údaj: " . $hodnota , ZPRAVA_CHYBA);
                $chyba++;
                continue;
            }
            $this->data[ $parametr ] = $_GET[ $parametr ];
        }
        
        if ( $chyba ) {
            return;
        }
        
        //print_r('dsds');
        if ( $_GET[ 'zmenit_heslo' ] ) {
            if ( $spravceUzivatelu->editujZamestnance(
                    $_GET[ 'id' ], $_GET[ 'jmeno' ], $_GET[ 'prijmeni_uziv' ], $_GET[ 'rodne_cislo' ],
                    $_GET[ 'adresa' ], $_GET[ 'pozice_uziv' ], $_GET[ 'login' ], $_GET[ 'zmenit_heslo' ]
                ) ) {
                $this->pridejZpravu( "Při editaci zaměstnance " . $_GET[ 'jmeno' ] .
                    " " . $_GET[ 'prijmeni' ] .  " došlo k chybě.", ZPRAVA_CHYBA );
            } else {
                $this->pridejZpravu( "Zaměstnanec " . $_GET[ 'jmeno' ] .
                    " " . $_GET[ 'prijmeni' ] .  " byl úspěšně upraven.", ZPRAVA_OK );
            }
        } else {
            if ( $spravceUzivatelu->editujZamestnance(
                    $_GET[ 'id' ], $_GET[ 'jmeno' ], $_GET[ 'prijmeni_uziv' ], $_GET[ 'rodne_cislo' ],
                    $_GET[ 'adresa' ], $_GET[ 'pozice_uziv' ], $_GET[ 'login' ]
                ) ) {
                $this->pridejZpravu( "Při editaci zaměstnance " . $_GET[ 'jmeno' ] .
                    " " . $_GET[ 'prijmeni' ] .  " došlo k chybě.", ZPRAVA_CHYBA );
            } else {
                $this->pridejZpravu( "Zaměstnanec " . $_GET[ 'jmeno' ] .
                    " " . $_GET[ 'prijmeni' ] .  " byl úspěšně upraven.", ZPRAVA_OK );
            }
        }
    }

}
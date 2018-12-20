<?php
/**
 * @project IIS - Restaurace
 * 
 * @author Jan Sorm (xsormj00), Alena Tesarova (xtesar36)
 * @file MujUcetKontroler.php
 * @brief Kontroler pro zpracovani argumentu na strance o infu o uchazeci,
 *  kde si muze uchazec naprikald menit heslo.
 */
class MujUcetKontroler extends Kontroler{
    
    public function zpracuj($parametry) {
        $this->overUzivatele( array( 'spolumajitel', 'servirka', 'kuchar', 'sefkuchar' ) );
        $spravceUzivatelu = new SpravceUzivatelu;
        $uzivatel = $spravceUzivatelu->vratUzivatele( $_SESSION[ 'uzivatel' ][ 'id_zam' ] );
        $this->hlavicka['titulek'] = 'Můj účet';
        
        // predavame pohledu zakladni data o uchazeci
        $this->data['jmeno'] = $uzivatel[ 'jmeno' ] . ' ' . $uzivatel[ 'prijmeni' ];
        $this->data['rodne_cislo'] = $uzivatel[ 'rodne_cislo' ];
        $this->data['adresa'] = $uzivatel[ 'adresa' ];
        $this->data['pozice'] = $uzivatel[ 'nazev' ];
        $this->pohled = 'muj_ucet';
        
        // menime si heslo
        if ( $_POST[ 'uloz_heslo' ] ) {
            $this->ulozNoveHeslo();
        }
                
    }
    
    /*
     * @brief Funkce pro kontrolu argumentu pri ukladani noveho hesla
     */
    private function ulozNoveHeslo() {
        // kontrolovane parametry
        $parametry = array(
            'stare_heslo' => 'Staré heslo',
            'nove_heslo' => 'Nové heslo',
            'kontrolni_heslo' => 'Zopakované nové heslo',
        );
        $chyba = 0;
        foreach ( $parametry as $parametr => $hodnota ) {
            if ( ! $_POST[ $parametr ] ) {
                $this->pridejZpravu( "Nesprávně zadaný údaj: " . $hodnota , ZPRAVA_CHYBA);
                $chyba++;
                continue;
            }
            $this->data[ $parametr ] = $_POST[ $parametr ];
        }
        
        if ( $chyba ) {
            return;
        }
        // kontrola, zda souhlasi nove heslo a kontrolni heslo
        if ( strcmp( $_POST[ 'nove_heslo' ], $_POST[ 'kontrolni_heslo' ] ) ) {
            $this->pridejZpravu( "Nové heslo a zopakované nové heslo se liší.", ZPRAVA_CHYBA );
            return;
        }
        // kontrola, zda souhlasi stare heslo s uchazecovym heslem v db
        if ( strcmp( $_POST[ 'stare_heslo' ], $_SESSION['uzivatel'][ 'heslo' ] ) ) {
            $this->pridejZpravu( "Nezadal jste správné staré heslo.", ZPRAVA_CHYBA );
            return;
        }
        
        $spravceUzivatelu = new SpravceUzivatelu;
        // zavolame modelovou funkci na zmenu hesla
        if ( $spravceUzivatelu->zmenHeslo( $_POST[ 'nove_heslo' ] ) ) {
            $this->pridejZpravu( "Při změně hesla došlo k chybě.", ZPRAVA_CHYBA );
            
        } else {
            $this->pridejZpravu( "Nové heslo bylo úspěšně uloženo.", ZPRAVA_OK );
            // v pripade uspechu uchazece odhlasime a presmerujeme zpet na prihlaseni
            $spravceUzivatelu->odhlas();
            $this->presmeruj('login');
        }
    }

}
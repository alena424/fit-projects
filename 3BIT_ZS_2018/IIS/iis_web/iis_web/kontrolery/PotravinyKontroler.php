<?php
/**
 * @project IIS - Restaurace
 * 
 * @author Jan Sorm (xsormj00), Alena Tesarova (xtesar36)
 * @file PotravinyKontroler.php
 * @brief Kontroler pro zpracovani argumentu ve sprave surovin.
 */

class PotravinyKontroler extends Kontroler{
    
    public function zpracuj($parametry) {
        $this->overUzivatele( array( 'spolumajitel', 'sefkuchar', 'kuchar' ) );
        
        $this->pohled = 'sprava_potravin';
        $this->hlavicka['titulek'] = 'Potraviny';
        
        $spravceSurovin = new SpravceSurovin;
        // chceme pridat novou surovinu
        if ( $this->params[ 'pridat_potravinu' ] ) {
            $this->vytvorPotravinu( $spravceSurovin );
        } else if ( $parametry[ 0 ] == 'odstran' && $parametry[ 1 ] ) {
            // chceme smazat surovinu
            // ajoxove mazani
            $smazano = $this->smazPotravinu( $spravceSurovin, $parametry[ 1 ] );
            $this->pohled = 'blank';
            //$this->vratZpravy(); -- aby nezustali v session
            $vysl['success'] = ! $smazano;
            $vysl_json = json_encode($vysl);
            echo $vysl_json;
            return;
        }
        
        // zde vracime pohledu suroviny pro vypis
        $suroviny = $spravceSurovin->vratSuroviny();
        $this->data[ 'jidlo' ] = array();
        foreach ( $suroviny as $surovina ) {
            $pole = array( 'nazev' => $surovina[ 'jmeno_suroviny' ], 'id' => $surovina[ 'id_surovina' ] );
            array_push( $this->data[ 'jidlo' ], $pole );
        }
         
    }
    
    /**
     * @brief Funkce zkontroluje parametry a zavola modelou funkci na pridani suroviny
     * 
     * @param object $spravceSurovin  model na spravu surovin
     * @return 1/0  Chyba/Ok
     */
    private function vytvorPotravinu( $spravceSurovin ) {
        // zkontrolujeme predane parametry
        $parametry = array(
            'nazev' => 'Název'
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

        // zavolame modelouvou funkci pro pridani suroviny a kontrolujeme vysledek
        if ( $spravceSurovin->vytvorSurovinu( $this->data[ 'nazev' ] ) ) {
            $this->pridejZpravu( "Při zakládání potraviny došlo k chybě." , ZPRAVA_CHYBA );
            return 1;
        } else {
            $this->pridejZpravu( "Potravina byla úspěšně přidána." , ZPRAVA_OK );
            return 0;
        }
    }
    
    /**
     * @brief Funkce na odstraneni surovin.
     * 
     * @param object $spravceSurovin  model na spravu surovin
     * @param int $id      id odstranovane suroviny
     * @return 1/0  Chyba/Ok
     */
    private function smazPotravinu( $spravceSurovin, $id ) {

        if ( $spravceSurovin->zrusPotravinu( $id ) ) {
            $this->pridejZpravu( "Při rušení potraviny došlo k chybě." , ZPRAVA_CHYBA );
            return 1;
        } else {
            $this->pridejZpravu( "Potravina byla úspěšně zrušena." , ZPRAVA_OK );
            return 0;
        }
    }

}
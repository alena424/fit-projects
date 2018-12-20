<?php

class StatistikyKontroler extends Kontroler{
    
    public function zpracuj($parametry) {
        $this->overUzivatele( array( 'spolumajitel' ) );
        
        $this->pohled = 'statistiky';
        $this->hlavicka['titulek'] = 'Statistiky';
        
        $this->data[ 'druh_pop' ] = array(
            "1" => 'Tržby po dnech',
            "2" => 'Tržby dle servírek',
            "4" => 'Počty prodaných potravin',
            "5" => 'Počty minut vybraných na rezervovaných stolech po dnech'
        );
        
        if ( $_GET[ 'hledej' ] || $_GET[ 'hledej_vse' ] ) {
            $datum_od = null;
            $datum_do = null;
            $chyby = 0;
            if ( $_GET['hledej']) {
                $od = new DateTime($_GET['datum_od']);
                $datum_od = $od->format('YmdHis');

                $do = new DateTime($_GET['datum_do']);
                // pridame 24 hod chceme cely den at se bere konec dne
                $do->add(new DateInterval('PT' . 24 . 'H'));
                $datum_do = $do->format('YmdHis');

                // tady musi byt i to rovna se
                if ($datum_od >= $datum_do) {
                    $this->pridejZpravu("Datum Odkdy je větší než datum Dokdy", ZPRAVA_CHYBA);
                    $chyby++;
                }
            }
            if ( !$chyba ) {
                $statistiky = new Statistiky;
                if ( $_GET[ 'druh' ] == '1' ) {
                    $this->data[ 'vystup' ] = $statistiky->vratTrzbyPoDnech( $datum_od, $datum_do );
                } elseif ( $_GET[ 'druh' ] == 2 ) {
                    $this->data[ 'vystup' ] = $statistiky->vratTrzbyDleServirek( $datum_od, $datum_do );
                } elseif ( $_GET[ 'druh' ] == 3 ) {

                } elseif ( $_GET[ 'druh' ] == 4 ) {
                    $this->data[ 'vystup' ] = $statistiky->poctyProdanychPotravin( $datum_od, $datum_do );
                } elseif ( $_GET[ 'druh' ] == 5 ) {
                    $this->data[ 'vystup' ] = $statistiky->poctyRezervaciPoDnech( $datum_od, $datum_do );
                }
                
                if ( !$this->data[ 'vystup' ] ) {
                    $this->pridejZpravu("Nebyla nalezena žádná opdovídající data.", ZPRAVA_CHYBA);
                }
            }
           
        }
    }

}
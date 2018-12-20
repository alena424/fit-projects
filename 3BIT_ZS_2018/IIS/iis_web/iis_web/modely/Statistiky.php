<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of Statistiky
 *
 * @author jan.sorm
 */
class Statistiky {
    //put your code here
    
    public function vratTrzbyPoDnech ( $od = null, $do = null ) {
        $parametry = array();
        $podminka = '';
        if ( $od || $do ) {
            $podminka = ' WHERE';
        }
        if ( $od ) {
            $podminka .= ' datum > ?';
            array_push( $parametry, $od );
        }
        if ( $do ) {
            if ( $od ) {
                $podminka .= ' AND';
            }
            $podminka .= ' datum < ?';
            array_push( $parametry, $do );
        }

        $vysledek = Db::select('
            SELECT CONCAT( SUM( konecna_suma ), " Kč" ) AS suma,
                DATE_FORMAT( datum, "%d. %m. %Y" ) datum,
                datum radit
            FROM uctenka' .
            $podminka
            . ' GROUP BY DATE_FORMAT( datum, "%d. %m. %Y" )
            ORDER BY radit',
            $parametry
        );
     //   print_r( $vysledek );
        $formatovanyVysledek = array();
        if ( count( $vysledek ) ) {
            array_push( $formatovanyVysledek, array( "Datum", "Tržba" ) );
        }
        foreach ( $vysledek as $v ) {
            array_push( $formatovanyVysledek, array( $v[ 'datum' ], $v[ 'suma' ] ) );
        }
        return $formatovanyVysledek;
    }
    
    public function vratTrzbyDleServirek ( $od = null, $do = null ) {
        $parametry = array();
        $podminka = '';
        if ( $od ) {
            $podminka .= ' AND u.datum > ?';
            array_push( $parametry, $od );
        }
        if ( $do ) {
            $podminka .= ' AND u.datum < ?';
            array_push( $parametry, $do );
        }

        $vysledek = Db::select('
            SELECT CONCAT( SUM( konecna_suma ), " Kč" ) AS suma,
                CONCAT( z.prijmeni, " ", z.jmeno ) jmeno
            FROM uctenka u, objednavka o, zamestnanec z
            WHERE u.objednavka = o.id_objednavka
                AND o.vytvoril = z.id_zam' .
            $podminka
            . ' GROUP BY CONCAT( z.prijmeni, " ", z.jmeno )
            ORDER BY jmeno',
            $parametry
        );
     //   print_r( $vysledek );
        $formatovanyVysledek = array();
        if ( count( $vysledek ) ) {
            array_push( $formatovanyVysledek, array( "Zaměstnanec", "Zúčtoval" ) );
        }
        foreach ( $vysledek as $v ) {
            array_push( $formatovanyVysledek, array( $v[ 'jmeno' ], $v[ 'suma' ] ) );
        }
        return $formatovanyVysledek;
    }
    
    public function poctyProdanychPotravin ( $od = null, $do = null ) {
        $parametry = array();
        $podminka = '';
        if ( $od ) {
            $podminka .= ' AND u.datum > ?';
            array_push( $parametry, $od );
        }
        if ( $do ) {
            $podminka .= ' AND u.datum < ?';
            array_push( $parametry, $do );
        }
        
        $vysledek = Db::select('
            SELECT CONCAT( SUM( mnozstvi ), " Ks" ) AS kusy,
                p.nazev nazev, CONCAT( p.cena, " Kč" ) cena
            FROM uctenka u, objednavka o, obsahuje_potravina op, potravina p
            WHERE u.objednavka = o.id_objednavka
                AND o.id_objednavka = op.objednavka
                AND op.potravina = p.id_jidlo' .
            $podminka
            . ' GROUP BY p.nazev, CONCAT( p.cena, " Kč" )
            ORDER BY nazev, cena',
            $parametry
        );
     //   print_r( $vysledek );
        $formatovanyVysledek = array();
        if ( count( $vysledek ) ) {
            array_push( $formatovanyVysledek, array( "Potravina", "Cena", "Počet kusů" ) );
        }
        foreach ( $vysledek as $v ) {
            array_push( $formatovanyVysledek, array( $v[ 'nazev' ], $v[ 'cena' ], $v[ 'kusy' ] ) );
        }
        return $formatovanyVysledek;
    }
    
        public function poctyRezervaciPoDnech ( $od = null, $do = null ) {
        $parametry = array();
        $podminka = '';
        if ( $od ) {
            $podminka .= ' AND r.datum > ?';
            array_push( $parametry, $od );
        }
        if ( $do ) {
            $podminka .= ' AND r.datum < ?';
            array_push( $parametry, $do );
        }
        
        $vysledek = Db::select('
            SELECT CONCAT( SUM( r.doba ), " min" ) AS minuty,
                CONCAT( SUM( s.pocet_osob ), " míst" ) AS mist,
                DATE_FORMAT( datum, "%d. %m. %Y" ) datum,
                datum radit
            FROM rezervace r, rezervace_na_stul rs, stul s
            WHERE r.id_rezervace = rs.rezervace
                AND rs.stul = s.id_stul' .
            $podminka
            . ' GROUP BY DATE_FORMAT( datum, "%d. %m. %Y" )
            ORDER BY radit',
            $parametry
        );
     //   print_r( $vysledek );
        $formatovanyVysledek = array();
        if ( count( $vysledek ) ) {
            array_push( $formatovanyVysledek, array( "Datum", "Rezervovaný čas u stolů", "Počet zarezervovaných míst" ) );
        }
        foreach ( $vysledek as $v ) {
            array_push( $formatovanyVysledek, array( $v[ 'datum' ], $v[ 'minuty' ], $v[ 'mist' ] ) );
        }
        return $formatovanyVysledek;
    }
}

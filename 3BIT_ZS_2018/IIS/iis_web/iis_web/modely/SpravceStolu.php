<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of SpravceStolu
 *
 * @author jan.sorm
 */
class SpravceStolu {
    
    private $tabulka = 'stul';
    
    public function stulIdNazev(){
        $hash = array();
        
        $dotaz = Db::select( '
            SELECT id_stul, cislo, umisteni FROM ' . $this->tabulka
        );
                    
        foreach ( $dotaz as $radek ) {
            $hash[ $radek[ 'id_stul' ] ] = 'č. ' . $radek[ 'cislo' ] . ' (' . $radek[ 'umisteni' ] . ')';
        }
        return $hash;
    }
    
    public function vratVolneStoly( $od_kdy, $doba ) {      
        $time = new DateTime($od_kdy);
        $time->add(new DateInterval('PT' . $doba . 'M'));
        $datum_do = $time->format('YmdHis');
        
        $params = array(  $od_kdy , $od_kdy,$do_kdy, $do_kdy );
        
        $stoly = Db::select( '
            SELECT s.id_stul, s.umisteni, s.cislo
            FROM stul s'
        );
        
        $rezervace = Db::select( "
            SELECT s.id_stul, r.datum, r.doba
            FROM stul s, rezervace_na_stul rs, rezervace r
            WHERE s.id_stul = rs.stul
                AND rs.rezervace = r.id_rezervace
                AND ( (r.datum <= ? AND  DATE_ADD( datum, INTERVAL doba MINUTE) >= ? ) OR ( r.datum <= ? AND DATE_ADD(r.datum, INTERVAL doba MINUTE) >= ?) )
                ",$params
        );
        $stoly_rez = array();
        foreach ( $rezervace as $radek ) {
            $stoly_rez[$radek['id_stul']] = 1;
        }
        $vysl = array();
        foreach ($stoly as $value) {
            if ( ! $stoly_rez[$value['id_stul']] ){
                $vysl[ $value['id_stul'] ] = 'č. ' . $value[ 'cislo' ] . ' (' . $value[ 'umisteni' ] . ')';
            }
        }
        /*print_r("<br>vysl<br>");
        print_r($vysl);
        print_r("<br>vyslkon<br>");*/
        return $vysl;
        
    }
    
    public function vratStoly() {
        return Db::select('
            SELECT *
            FROM stul
            WHERE zruseny = 0'
        );
    }
    
    public function vytvorStul( $umisteni, $pocet_osob, $cislo ) {
        if ( ! Db::insert(
            'stul',
            array(
                'cislo' => $cislo,
                'umisteni' => $umisteni,
                'pocet_osob' => $pocet_osob,
                'zruseny' => 0
            )
            ) ) {
            
            return 1;
        }
        
        return 0;
    }
    
    public function zrusStul( $id ){
        // nejdriv je potreba zjistit zda existujou navazane zaznamy v tabulkach
        $nalezeno = Db::selectResult(
            'SELECT stul FROM rezervace_na_stul WHERE stul = ?',
            array( $id )
        );
        
        if ( ! $nalezeno ) {
            $nalezeno = Db::selectResult(
                'SELECT stul FROM objednavka_pro_stul WHERE stul = ?',
                array( $id )
            );
        }
        
        if ( ! $nalezeno ) {
            
            if ( ! Db::smaz(
                'stul', 'WHERE id_stul = ?', array( $id ) ) ) {
                return 1;
            }

        } else {
            if ( ! Db::update(
                    'stul',
                    array(
                        'zruseny' => 1,
                    ),
                    'WHERE id_stul = ?',
                    array( $id )
                ) ) {
                return 1;
            }
        }
        return 0;
    }
}

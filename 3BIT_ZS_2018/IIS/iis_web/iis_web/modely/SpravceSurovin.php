<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of SpravceSurovin
 *
 * @author jan.sorm
 */
class SpravceSurovin {
    
    
    public function surovinaIdNazev(){
        $hash = array();
        
        $dotaz = Db::select( '
            SELECT id_surovina, jmeno_suroviny FROM surovina'
        );
                    
        foreach ( $dotaz as $radek ) {
            $hash[ $radek[ 'id_surovina' ] ] = $radek[ 'jmeno_suroviny' ];
        }
        return $hash;
    }
    
    public function vratSuroviny() {
        return Db::select('
            SELECT *
            FROM surovina
            WHERE zrusena = 0'
        );
    }
    
    public function vytvorSurovinu( $nazev ) {
        if ( ! Db::insert(
            'surovina',
            array(
                'jmeno_suroviny' => $nazev,
                'zrusena' => 0
            )
            ) ) {
            
            return 1;
        }
        
        return 0;
    }
    
    public function zrusPotravinu( $id ){
        // nejdriv je potreba zjistit zda existujou navazane zaznamy v tabulkach
        $nalezeno = Db::selectResult(
            'SELECT potravina FROM obsahuje_surovina WHERE surovina = ?',
            array( $id )
        );
        
        if ( ! $nalezeno ) {
            
            if ( ! Db::smaz(
                'surovina', 'WHERE id_surovina = ?', array( $id ) ) ) {
                return 1;
            }

        } else {
            if ( ! Db::update(
                    'surovina',
                    array(
                        'zrusena' => 1,
                    ),
                    'WHERE id_surovina = ?',
                    array( $id )
                ) ) {
                return 1;
            }
        }
        return 0;
    }
}

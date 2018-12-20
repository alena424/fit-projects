<?php

/**
 * Pracuje s potravinami a s jidlem, pridani, odstraneni, upraveni
 */
class SpravceJidla {

    private $tabulka = 'potravina';

    public function jidloInfo($id, $surovina = null) {
        $params = array( $id );
        
        $where = '';
        $from = '';
        if ( $surovina ){
            $from = ' ,obsahuje_surovina';
            $where = ' AND obsahuje_surovina.potravina = id_jidlo'
                    . ' AND obsahuje_surovina.surovina = ?';
            array_push($params, $surovina);
            
            
        }
        
        return Db::select('
            SELECT * FROM ' . $this->tabulka . $from .' WHERE id_jidlo = ? '. $where, $params
        );
    }

    public function dostupneJidlo() {
        return Db::select('
            SELECT * FROM ' . $this->tabulka . ' WHERE pocet_porci > 0'
        );
    }

    public function jidloIdNazev($piti) {
        $hash = array();
        if ($piti) {
            $dotaz = Db::select('
                SELECT id_jidlo, nazev FROM ' . $this->tabulka . ' WHERE typ_sklenice IS NOT NULL and zrusena = 0'
            );
        } else {
            //print_r();
            $dotaz = Db::select('
                SELECT id_jidlo, nazev FROM ' . $this->tabulka . ' WHERE typ_sklenice IS NULL and zrusena = 0'
            );
        }

        foreach ($dotaz as $radek) {
            $hash[$radek['id_jidlo']] = $radek['nazev'];
        }
        return $hash;
    }

    public function vytvorJidlo($name, $price, $portion, $sacharides, $energy, $proteins, $fats, $expiration, $drink) {
        if (Db::insert($table, array($name, $price, $portion, $sacharides,
                    $energy, $proteins, $fats, $expiration, $drink))) {

            return Db::getLastId();
        } else {
            return 0;
        }
    }

    /*
     * $params $pocet_porci rika, kolik porci mame odebrat
     */

    public function odeberPorce($id, $pocet_porci) {
        $soucasny_pocet = Db::selectResult("SELECT pocet_porci FROM " . $this->tabulka . " WHERE id_jidlo = ?", $id);

        if ($soucasny_pocet - $pocet_porci < 0) {
            return 2; // chyba indikujici, ze jdeme do zaporu
        }

        if (Db::update(
                        $this->tabulka, array(
                    'pocet_porci' => $soucasny_pocet - $pocet_porci,
                        ), 'WHERE id = ?', array($id))) {
            return 1;
        } else {
            return 0;
        }
    }

    public function upravJidlo($id, $name, $price, $portion, $sacharides, $energy, $proteins, $fats, $expiration = null, $drink = null) {
        if (!$id || !is_numeric($id)) {
            return 0;
        }
        if (Db::update(
                        $this->tabulka, array(
                    'nazev' => $name,
                    'cena' => $price,
                    'pocet_porci' => $portion,
                    'mnozstvi_sacharidu' => $sacharides,
                    'energie_hodnota' => $energy,
                    'bilkoviny' => $proteins,
                    'tuky' => $fats,
                        //'typ_sklenice' => $drink // TODO opravit
                        ), 'WHERE id_jidlo = ?', array($id))) {
            return 1;
        } else {
            return 0;
        }
    }

    public function vytvorPotravinu($nazev, $cena, $pocet_porci, $energie, $tuky, $sacharidy, $bilkoviny, $pribor, $typ_sklenice, $expirace, $suroviny) {

        Db::zacniTransakci();

        $chyba = 0;

        if (!Db::insert(
                        'potravina', array(
                    'nazev' => $nazev,
                    'cena' => $cena,
                    'pocet_porci' => $pocet_porci,
                    'energie_hodnota' => $energie,
                    'tuky' => $tuky,
                    'mnozstvi_sacharidu' => $sacharidy,
                    'bilkoviny' => $bilkoviny,
                    'pribor' => $pribor,
                    'typ_sklenice' => $typ_sklenice,
                    'expirace' => $expirace,
                    'zrusena' => 0
                        )
                )) {

            $chyba++;
        }

        $id = Db::getLastId();
        //print_r("jdeme pridat potravinu \n ");
        //print_r($suroviny);
        //print_r('chyba '.$chyba);
        if (!$chyba && $suroviny[0]) {
            
            
            foreach ($suroviny as $surovina) {
                if (!Db::insert(
                                'obsahuje_surovina', array(
                            'surovina' => $surovina['id'],
                            'potravina' => $id,
                            'mnozstvi' => $surovina['mnozstvi']
                                )
                        )) {

                    $chyba++;
                    break;
                }
            }
        }

        if ($chyba) {
            Db::vratZpet();
            return 1;
        } else {
            Db::potvrd();
            return 0;
        }
    }

    public function vratJidlo() {
        return Db::select('
            SELECT *
            FROM potravina p
            WHERE pribor IS NOT NULL
            AND zrusena = 0'
        );
    }
    
    public function upravSurovinuKPotravine($jidlo, $surovina, $mnozstvi) {

        if ( $jidlo && $surovina && $mnozstvi > 0 ){
             if (!Db::update(
                            'obsahuje_surovina', array(
                        'mnozstvi' => $mnozstvi,
                            ), 'WHERE surovina = ? AND potravina = ?', array( $surovina, $jidlo)
                    )) {
                return 1;
            } else {
                return 0;
            }
            
        } else {
            return 1;
        }
    }
    
    public function pridejSurovinyKPotravine($jidlo, $suroviny) {
        // jak s mnozstvim TODO !!!
        $chyba= 0;
        //print_r("pridavam:");
        //print_r('jidlo: '. $jidlo);
        //print_r($suroviny);
        if (!$chyba && $suroviny[0] && $jidlo) {
            foreach ($suroviny as $surovina) {
                //print_r($surovina);
                if (!Db::insert(
                                'obsahuje_surovina', array(
                            'surovina' => $surovina['id'],
                            'potravina' => $jidlo,
                            'mnozstvi' => $surovina['mnozstvi']
                                )
                        )) {

                    $chyba++;
                    break;
                }
            }
        }
        return $chyba;
    }

    public function zrusPotravinuKSurovine($jidlo, $surovina) {
        $nalezeno = Db::selectResult(
                        'SELECT potravina, surovina FROM obsahuje_surovina WHERE potravina = ? AND surovina = ?', array($jidlo, $surovina)
        );

        // mazeme jen pokud existuje
        if ($nalezeno) {
            Db::zacniTransakci();

            $chyba = 0;
            if (!Db::smaz(
                            'obsahuje_surovina', 'WHERE potravina = ? AND surovina = ?', array($jidlo, $surovina))) {
                $chyba++;
            }

            if ($chyba) {
                Db::vratZpet();
                return 1;
            } else {
                Db::potvrd();
                return 0;
            }
        }
    }

    public function vratSurovinyJidlo() {
        $info = Db::select('
            SELECT potravina.id_jidlo, potravina.nazev potravina, surovina.jmeno_suroviny surovina, obsahuje_surovina.mnozstvi, surovina.id_surovina
            FROM potravina, obsahuje_surovina, surovina
            WHERE obsahuje_surovina.potravina = potravina.id_jidlo
                AND obsahuje_surovina.surovina = surovina.id_surovina
             ORDER BY potravina.id_jidlo
                '
        );
        $vysl = array();
        $row = array();
        $i = 0;
        foreach ($info as $value) {


            $row['id_jidlo'] = $value['id_jidlo'];
            $row['id_surovina'] = $value['id_surovina'];
            $row['surovina'] = $value['surovina'];
            $row['mnozstvi'] = $value['mnozstvi'];
            $row['potravina'] = $value['potravina'];

            //$vysl[ $value['id_jidlo'] ][$i] = $row;
            //$i++;

            array_push($vysl, $row);
        }

        return $vysl;
    }

    public function vratPiti() {
        return Db::select('
            SELECT *
            FROM potravina p
            WHERE pribor IS NULL
            AND zrusena = 0'
        );
    }

    public function zrusPotravinu($id) {
        // nejdriv je potreba zjistit zda existujou navazane zaznamy v tabulkach
        $nalezeno = Db::selectResult(
                        'SELECT potravina FROM obsahuje_potravina WHERE potravina = ?', array($id)
        );

        if (!$nalezeno) {
            Db::zacniTransakci();

            $chyba = 0;
            if (!Db::smaz(
                            'obsahuje_surovina', 'WHERE potravina = ?', array($id))) {
                $chyba++;
            }

            if (!$chyba && !Db::smaz(
                            'potravina', 'WHERE id_jidlo = ?', array($id))) {
                $chyba++;
            }

            if ($chyba) {
                Db::vratZpet();
                return 1;
            } else {
                Db::potvrd();
                return 0;
            }
        } else {
            if (!Db::update(
                            'potravina', array(
                        'zrusena' => 1,
                            ), 'WHERE id_jidlo = ?', array($id)
                    )) {
                return 1;
            }
        }
        return 0;
    }

}

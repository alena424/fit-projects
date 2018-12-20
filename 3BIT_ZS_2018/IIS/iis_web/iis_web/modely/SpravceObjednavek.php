<?php

/**
 * trida pracujici s objednavkou
 */
class SpravceObjednavek {

    private $tabulka = 'objednavka';

    public function objednavkaInfo($id, $od = null, $do = null, $zauctovane = 0, $potravina = null) {
        $condition = '';
        $from = '';
        $select = '';
        $params = array();

        if ($id) {
            $condition .= ' AND id_objednavka = ?';
            array_push($params, $id);
        }

        if ($od) {
            $condition .= ' AND obsahuje_potravina.vlozeno >= ?';
            array_push($params, $od);
        }

        if ($do) {
            $condition .= ' AND obsahuje_potravina.vlozeno <= ?';
            array_push($params, $do);
        }

        if ($potravina) {
            $condition .= ' AND potravina.id_jidlo = ?';
            array_push($params, $potravina);
        }
        if ($zauctovane) {
            $condition .= ' AND uctenka.objednavka = objednavka.id_objednavka';
            $from .= ' ,uctenka';
            $select .= ' ,uctenka.datum uctenka_datum, konecna_suma, id_ucet';
        }

        $objednavky = Db::select("
                        SELECT objednavka.id_objednavka, stul.umisteni, potravina.nazev, obsahuje_potravina.id_obsahuje_potravina, obsahuje_potravina.mnozstvi, stul.cislo id_stul, stul.umisteni, objednavka.datum,  
                        potravina.cena, objednavka.rezervace, obsahuje_potravina.vlozeno, vytvoril, potravina.id_jidlo $select
                        FROM objednavka, stul, obsahuje_potravina, potravina $from
                        WHERE objednavka.stul = stul.id_stul
                           AND obsahuje_potravina.objednavka = objednavka.id_objednavka 
                           AND potravina.id_jidlo = obsahuje_potravina.potravina " . $condition, $params);
  

        return $objednavky;
    }

    public function vytvorObjednavku($stoly, $jidlo, $piti, $cena, $datum = null, $rezervace = null, $transakce = 1) {
        if (!$datum) {
            $datum = date("YmdHis");
        }

        if ($transakce) {
            Db::zacniTransakci();
        }
        $chyba = 0;
        //print_r('Stul je ' . $stoly[0]);
        if (!Db::insert(
                        $this->tabulka, array(
                    'datum' => $datum,
                   // 'suma' => $cena,
                    'vytvoril' => $_SESSION['uzivatel']['id_zam'],
                    'rezervace' => $rezervace,
                    'stul' => $stoly[0]
                        )
                )) {

            $chyba++;
        }

        $id = Db::getLastId();



        if (!$chyba) {
            $chyba = $this->pridejKObjednavce($stoly, $jidlo, $piti, $id);
        }


        if ($chyba) {
            if ($transakce) {
                Db::vratZpet();
            }
            return 1;
        } else {
            if ($transakce) {
                Db::potvrd();
            }
            return 0;
        }
    }

    public function existujePotravina($obj, $potravina = null) {
        $objednavky = array();
        $condition = '';
        $params = array();

        array_push($params, $obj);
        if ($potravina) {
            $condition .= ' AND potravina = ?';
            array_push($params, $potravina);
        }

        if ($obj) {
            $objednavky = Db::select("
                        SELECT mnozstvi
                        FROM obsahuje_potravina
                        WHERE objednavka = ?
                            $condition", $params);
        } else {
            //print_r("nezadana objednavka nebo potravina");
        }


        return ( (!empty($objednavky) ) ? 1 : 0 );
    }

    public function pridejKObjednavce($stoly = array(), $jidlo = array(), $piti = array(), $id) {
        if (!$id) {
            return 'chybi ID!';
        }
        $cena = 250;

        $chyba = 0;

        $objednavka = $this->objednavkaInfo($id);
//        print_r($objednavka);
//        if ( empty($objednavka) ){
//            // objednavka neexistuje !
//            $this->vytvorObjednavku($stoly, $jidlo, $piti, $cena, null, null, 0);
//        }
        //$spravceJidla = new SpravceJidla;
//        foreach ($stoly as $stul_id) {
//            if (!Db::insert(
//                            'objednavka_pro_stul', array(
//                        'stul' => $stul_id,
//                        'objednavka' => $id
//                            )
//                    )) {
//
//                $chyba++;
//                break;
//            }
//        }



        if (!$chyba && !empty($jidlo)) {
            foreach ($jidlo as $jidlo_id) {
                if (is_array($jidlo_id)) {

                    if (!Db::insert(
                                    'obsahuje_potravina', array(
                                'potravina' => $jidlo_id['id'],
                                'objednavka' => $id,
                                'mnozstvi' => $jidlo_id['mnozstvi'],
                                //'detska_porce' => $jidlo_id['detska'] ? $jidlo_id['detska'] : 0,
                                'vlozeno' => date("YmdHis"),
                                    )
                            )) {

                        $chyba++;
                        break;
                    }
                } else {

                    if (!Db::insert(
                                    'obsahuje_potravina', array(
                                'potravina' => $jidlo_id,
                                'objednavka' => $id,
                                'mnozstvi' => 1,
                                'vlozeno' => date("YmdHis"),
                                    )
                            )) {

                        $chyba++;
                        break;
                    }

                    /* $chyba = $spravceJidla->odeberPorce( $jidlo_id, 1 );
                      if ( $chyba ) {
                      $break;
                      } */
                }
            }
        }
        //print_r("<br>id objednavky $id<br>");
        if (!$chyba && !empty($piti)) {
            foreach ($piti as $piti_id) {
                if (is_array($piti_id)) {
                    // u piti zadna destka porce neni !

                    if (!Db::insert(
                                    'obsahuje_potravina', array(
                                'potravina' => $piti_id['id'],
                                'objednavka' => $id,
                                'mnozstvi' => $piti_id['mnozstvi'],
                                'vlozeno' => date("YmdHis"),
                                    )
                            )) {

                        $chyba++;
                        break;
                    }
                } else {
                    if (!Db::insert(
                                    'obsahuje_potravina', array(
                                'potravina' => $piti_id,
                                'objednavka' => $id,
                                'mnozstvi' => 1,
                                'vlozeno' => date("YmdHis"),
                                    )
                            )) {

                        $chyba++;
                        break;
                    }
                }

                /*        $chyba = $spravceJidla->odeberPorce( $piti_id, 1 );
                  if ( $chyba ) {
                  $break;
                  } */
            }
        }
        return $chyba;
    }

    public function vratUctenku($id) {
        // volat funkce z uctenek
    }

    public function upravObjednavku($id, $date, $sum, $creator, $rezervation) {
        if (Db::update(
                        $this->tabulka, array(
                    'datum' => $date,
                   // 'suma' => $sum,
                    'vytvoril' => $creator,
                    'rezervace' => $rezervation
                        ), 'WHERE id = ?', array($id))) {
            return 1;
        } else {
            return 0;
        }
    }

//    public function zrusJidloKObjednavce($potravina, $objednavka, $transakce = 1) {
//        if ($transakce) {
//            Db::zacniTransakci();
//        }
//        $chyba = 0;
//        //$objednavka = Db::select('SELECT objednavka WHERE');
//
//        if (!Db::smaz(
//                        'uctenka', 'WHERE obsahuje_potravina = ?', array($id))) {
//            $chyba++;
//        }
//
//        if ($chyba) {
//            if ($transakce) {
//                Db::vratZpet();
//            }
//            return 1;
//        } else {
//            if ($transakce) {
//                Db::potvrd();
//            }
//            return 0;
//        }
//    }

    public function zrusObjednavku($id, $transakce = 1) {
        // nejdriv je potreba zrusit navazane uctenky
        if ($transakce) {
            Db::zacniTransakci();
        }
        $chyba = 0;

        if (!Db::smaz(
                        'uctenka', 'WHERE objednavka = ?', array($id))) {
            $chyba++;
        }
//        if (!$chyba && !Db::smaz(
//                        'objednavka_pro_stul', 'WHERE objednavka = ?', array($id))) {
//            $chyba++;
//        }
        if (!$chyba && !Db::smaz(
                        $this->tabulka, 'WHERE id_objednavka = ?', array($id))) {
            $chyba++;
        }


        if ($chyba) {
            if ($transakce) {
                Db::vratZpet();
            }
            return 1;
        } else {
            if ($transakce) {
                Db::potvrd();
            }
            return 0;
        }
    }

    public function zmenMnozstvi($potravina, $objednavka, $pocet_porci = 1, $transakce = 1, $detska = 0) {
        if ($pocet_porci < 1) {
            return array(fail => "BAF");
        }

        $cena_pred = 0;
        $cena_po = 0;
        $info = $this->objednavkaInfo($objednavka, null, null, null, $potravina);
        if (!empty($info)) {
            $cena_pred = $info[0]['cena'] * $info[0]['mnozstvi'];
        }

        if (!Db::update(
                        'obsahuje_potravina', array(
                    'mnozstvi' => $pocet_porci,
                    //'detska_porce' => $detska,
                        ), 'WHERE potravina = ? AND objednavka = ?', array($potravina, $objednavka))) {



            return array(success => 0, ztracena_cena => 0);
        } else {
            $info2 = $this->objednavkaInfo($objednavka, null, null, null, $potravina);
            if (!empty($info2)) {
                $cena_po = $info2[0]['cena'] * $info2[0]['mnozstvi'];
            }

            return array(success => 1, ztracena_cena => $cena_po - $cena_pred);
        }
    }
    
    /**
     * Funcke pouziva obsahuje_potravina.id_obsahuje_potravina NOVY SLOUPEC
     * @param type $id              id_obsahuje_potravina
     * @param type $pocet_porci     novy pocet porci
     * @param type $transakce       1/0 
     * @param type $detska          1/0
     * @return int                  ztracena cena po updatu
     */
    public function zmenMnozstviId($id, $pocet_porci = 1, $transakce = 1, $detska = 0) {
        if ($pocet_porci < 1) {
            return array(fail => "BAF");
        }
        
        $info1 = $this->vratPotravinaObjednavka($id);
        //print_r($info);
        if (!$info1 || !$info1[0]['potravina'] || !$info1[0]['objednavka'] ) {
            print_r('chyba pozor zrusPotravinuKObjednavce');
            return 1;
        }
        $objednavka = $info1[0]['objednavka'] ;
        $potravina = $info1[0]['potravina'] ;

        $cena_pred = 0;
        $cena_po = 0;
        $info = $this->objednavkaInfo($objednavka, null, null, null, $potravina);
        if (!empty($info)) {
            $cena_pred = $info[0]['cena'] * $info[0]['mnozstvi'];
        }

        if (!Db::update(
                        'obsahuje_potravina', array(
                    'mnozstvi' => $pocet_porci,
                   // 'detska_porce' => $detska,
                        ), 'WHERE id_obsahuje_potravina= ?', array($id))) {



            return array(success => 0, ztracena_cena => 0);
        } else {
            $info2 = $this->objednavkaInfo($objednavka, null, null, null, $potravina);
            if (!empty($info2)) {
                $cena_po = $info2[0]['cena'] * $info2[0]['mnozstvi'];
            }

            return array(success => 1, ztracena_cena => $cena_po - $cena_pred);
        }
    }

    public function zrusPotravinuKObjednavce($potravina, $objednavka, $transakce = 1) {
        // nejdriv je potreba zrusit navazane uctenky
        if (!$potravina || !$objednavka) {
            print_r('chyba pozor zrusPotravinuKObjednavce');
            return 1;
        }
        if ($transakce) {
            Db::zacniTransakci();
        }
        $chyba = 0;
        $ztracena_cena = 0;
        $info = $this->objednavkaInfo($objednavka, null, null, null, $potravina);
        if (!empty($info)) {
            $ztracena_cena = $info[0]['cena'] * $info[0]['mnozstvi'];
        }

        if (!Db::smaz(
                        'obsahuje_potravina', 'WHERE objednavka = ? and potravina = ?', array($objednavka, $potravina))) {
            $chyba++;
        }
//        if (!$chyba && !Db::smaz(
//                        'objednavka_pro_stul', 'WHERE objednavka = ?', array($id))) {
//            $chyba++;
//        }

        if ((!$chyba ) && (!$this->existujePotravina($objednavka) ) && !Db::smaz(
                        $this->tabulka, 'WHERE id_objednavka = ?', array($objednavka))) {
            $chyba++;
        }


        if ($chyba) {
            if ($transakce) {
                Db::vratZpet();
            }
            return array(success => 0, ztracena_cena => 0);
            ;
        } else {
            if ($transakce) {
                Db::potvrd();
            }
            $vysl = array(success => 1, ztracena_cena => -$ztracena_cena);
            return $vysl;
        }
    }
    
    public function vratPotravinaObjednavka($id){
         $obsahuje= Db::select('SELECT objednavka, potravina, mnozstvi
                        FROM obsahuje_potravina 
                        WHERE id_obsahuje_potravina = ?', array($id));
         return $obsahuje;
         
         
    }
    public function zrusPotravinuKObjednavceId($id) {
        // nejdriv je potreba zrusit navazane uctenky
        $info = $this->vratPotravinaObjednavka($id);
        //print_r($info);
        if (!$info || !$info[0]['potravina'] || !$info[0]['objednavka'] ) {
            print_r('chyba pozor zrusPotravinuKObjednavce');
            return 1;
        }
        $objednavka = $info[0]['objednavka'] ;
        $potravina = $info[0]['potravina'] ;
        
        if ($transakce) {
            Db::zacniTransakci();
        }
        $chyba = 0;
        $ztracena_cena = 0;
        $info = $this->objednavkaInfo($objednavka, null, null, null, $potravina);
        if (!empty($info)) {
            $ztracena_cena = $info[0]['cena'] * $info[0]['mnozstvi'];
        }
        

        if (!Db::smaz(
                        'obsahuje_potravina', 'WHERE id_obsahuje_potravina = ?', array($id))) {
            $chyba++;
        }
//        if (!$chyba && !Db::smaz(
//                        'objednavka_pro_stul', 'WHERE objednavka = ?', array($id))) {
//            $chyba++;
//        }

        if ((!$chyba ) && (!$this->existujePotravina($objednavka) ) && !Db::smaz(
                        $this->tabulka, 'WHERE id_objednavka = ?', array($objednavka))) {
            $chyba++;
        }


        if ($chyba) {
            if ($transakce) {
                Db::vratZpet();
            }
            return array(success => 0, ztracena_cena => 0);
            ;
        } else {
            if ($transakce) {
                Db::potvrd();
            }
            $vysl = array(success => 1, ztracena_cena => -$ztracena_cena);
            return $vysl;
        }
    }

    public function historieObjednavek() {
        
    }

    public function tiskUctenky($objednavka) {

        $params_rezervace = array();
        //print_r('aaa');
//        foreach ($objednavky as $value) {
//            array_push($params_rezervace, $value);
//        }
//        $otazniky = join(',', array_map(function($item) {
//                    return '?';
//                }, $params_rezervace));

        $condition = " AND id_objednavka  = ?";
//
//        $objednavky_sel = Db::select("SELECT id_ucet FROM uctenka WHERE objednavka in ($otazniky)", $objednavky);
//        print_r($objednavky_sel);
//        if ($objednavky_sel && !empty($objednavky_sel)) {
//            return 'Již účtované';
//        }
        $params = array($objednavka);
        $chyba = 0;
        $objednavky_sel = Db::select('SELECT objednavka.id_objednavka, sum(potravina.cena*obsahuje_potravina.mnozstvi) cena
                        FROM objednavka, obsahuje_potravina, potravina
                        WHERE  obsahuje_potravina.objednavka = objednavka.id_objednavka 
                           AND potravina.id_jidlo = obsahuje_potravina.potravina
                            
                           ' . $condition
                        . ' GROUP BY id_objednavka', $params);
        //print_r($objednavky_sel);
        if (!empty($objednavky_sel)) {
           
            // vytvorime uctenku
          
            if (!Db::insert(
                            'uctenka', array(
                        'objednavka' => $objednavka,
                        'datum' => date("YmdHis"),
                        'konecna_suma' => $objednavky_sel[0]['cena'] ? $objednavky_sel[0]['cena'] : 1,
                            )
                    )) {

                $chyba++;
            }
        }
        return $chyba;
    }

    public function nezauctovaneObjednavky($stoly = array()) {

        $params_rezervace = array();
        $condition = '';
        $params = array();
        if (!empty($stoly)) {

            foreach ($stoly as $value) {
                array_push($params_rezervace, $value);
            }
            $otazniky = join(',', array_map(function($item) {
                        return '?';
                    }, $params_rezervace));

            $condition = " AND id_stul in ($otazniky)";
        }


        $objednavky = Db::select("
                        SELECT id_obsahuje_potravina, objednavka.id_objednavka, stul.umisteni, potravina.nazev, obsahuje_potravina.mnozstvi, stul.cislo id_stul, stul.umisteni, objednavka.datum,
                        potravina.cena, objednavka.rezervace, vytvoril, obsahuje_potravina.vlozeno, potravina.id_jidlo, potravina.pribor
                        FROM objednavka, stul, obsahuje_potravina, potravina
                        WHERE objednavka.stul = stul.id_stul
                           AND obsahuje_potravina.objednavka = objednavka.id_objednavka 
                           AND potravina.id_jidlo = obsahuje_potravina.potravina
                           AND id_objednavka not in (SELECT objednavka FROM uctenka)
                           $condition"
                        . "ORDER BY objednavka.id_objednavka, vlozeno", $stoly);

      

        $vysledek = array();
        foreach ($objednavky as $value) {
            array_push($vysledek, array(
                'id_obsahuje' => $value['id_obsahuje_potravina'],
                'id' => $value['id_objednavka'],
                'jidlo' => $value['nazev'],
                'mnozstvi' => $value['mnozstvi'],
                //'detska_porce' => $value['detska_porce'],
                'pribor' => $value['pribor'],
                'umisteni' => $value['umisteni'],
                'id_stul' => $value['id_stul'],
                'datum' => $value['datum'],
                'cena' => $value['cena'],
                'vlozeno' => $value['vlozeno'],
                'patri_rezervace' => $value['rezervace'],
                'id_jidlo' => $value['id_jidlo'],
                'vytvoril' => $value['vytvoril'],
                'nazev' => $value['nazev'] . ' (č. ' . $value['id_stul'] . ', ' . $value['umisteni'] . ') ',
                'stul' => ' (č. ' . $value['id_stul'] . ', ' . $value['umisteni'] . ') ',
            ));
        }
        //print_r($vysledek);

        return $vysledek;
    }

    public function vratObjednavkyRezervace($rezervace) {

        $id_objednavek = array();
        $objednavky = Db::select("SELECT id_objednavka FROM " . $this->tabulka .
                        " WHERE rezervace = ?", array($rezervace));

        foreach ($objednavky as $obj) {
            array_push($id_objednavek, $obj['id_objednavka']);
        }

        return $id_objednavek;
    }

}

?>
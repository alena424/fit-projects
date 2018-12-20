<?php

class SpravceRezervace {

    private $tabulka = 'rezervace';

    /**
     * Funkce vrati nasledujici informace o rezervaci:
     * cislo, jmeno, datum, telefon, stoly, objednavky, vysledna cena
     * @param type $id ID rezervace, o krere chceme informace
     * @param $date datum vytvorene rezervace
     * @param $surname - prijmeni rezervace
     * musi byt vyplneno alespon jedno z toho
     */
    public function rezervaceInfo($id = null, $date = null, $surname = null, $od = null, $do = null, $identifikator = null) {
        $condition = '';

        $params = array();
        
        if ($id) {
            $condition .= ' WHERE r.id_rezervace = ?';
            array_push($params, $id);
        }
        if ($identifikator) {
            if ($condition) {
                $condition .= ' AND r.identifikator = ?';
            } else {
                $condition .= ' WHERE r.identifikator = ?';
            }
            array_push($params, $identifikator);
        }

        if ($date) {
            if ($condition) {
                $condition .= ' AND r.datum = ?';
            } else {
                $condition .= ' WHERE r.datum = ?';
            }
            array_push($params, $date);
        }
        if ($od) {
            if ($condition) {
                $condition .= ' AND r.datum >= ?';
            } else {
                $condition .= ' WHERE r.datum >= ?';
            }
            array_push($params, $od);
        }
        if ($do) {
            if ($condition) {
                $condition .= ' AND r.datum <= ?';
            } else {
                $condition .= ' WHERE r.datum <= ?';
            }
            array_push($params, $do);
        }
        if ($surname) {
            if ($condition) {
                $condition .= ' AND r.prijmeni_zakaznika like ?';
            } else {
                $condition .= ' WHERE r.prijmeni_zakaznika like ?';
            }
            array_push($params, '%' . $surname . '%');
        }
        //print_r($condition);
        //print_r($params);
        $rezervace = Db::select('
            SELECT r.id_rezervace, r.identifikator, DATE_FORMAT( r.datum, "%d.%m.%Y %H:%i:%s" ) datum,  
                DATE_FORMAT( r.datum, "%Y-%m-%d" ) datum_format, DATE_FORMAT( r.datum, "%H:%i" ) cas_format,
                r.doba, r.jmeno_zakaznika jmeno,
                r.prijmeni_zakaznika prijmeni, r.kontakt_zakaznika kontakt, s.id_stul stul
            FROM rezervace r
            LEFT JOIN rezervace_na_stul rs ON r.id_rezervace = rs.rezervace
            LEFT JOIN stul s ON rs.stul = s.id_stul
            '
                        . $condition
                        . ' ORDER BY prijmeni_zakaznika', $params
        );
        /*
         *  LEFT JOIN objednavka o ON r.id_rezervace = o.rezervace
          LEFT JOIN objednavka_pro_stul os ON o.id_objednavka = os.objednavka
          LEFT JOIN obsahuje_potravina op ON o.id_objednavka = op.objednavka
          LEFT JOIN potravina p ON op.potravina = p.id_jidlo
         */
        $params_rezervace = array();
        if (empty($rezervace)) {
            array_push($params_rezervace, 0);
        }
        foreach ($rezervace as $value) {
            array_push($params_rezervace, $value['id_rezervace']);
        }
        $otazniky = join(',', array_map(function($item) {
                    return '?';
                }, $params_rezervace));
        //print_r($otazniky);

        $objednavky = Db::select("
            SELECT p.nazev, op.mnozstvi porce, p.cena, p.typ_sklenice,
                p.id_jidlo, objednavka.rezervace
            FROM potravina p, obsahuje_potravina op, objednavka
            WHERE op.objednavka = objednavka.id_objednavka
                AND op.potravina = p.id_jidlo
                AND objednavka.rezervace in ($otazniky)", $params_rezervace
        );
        //print_r("  obj");
        //print_r($objednavky);

        $rezervace_hash = array(
        );

        foreach ($rezervace as $radek) {
            $key = $radek['id_rezervace'];
            if (array_key_exists($key, $rezervace_hash)) {
                //print_r( $radek );   
                if (!in_array($radek['stul'], $rezervace_hash[$key]['stoly'])) {
                    array_push($rezervace_hash[$key]['stoly'], $radek['stul']);
                }
            } else {
                //array_push( $rezervace_hash, $key );
                $rezervace_hash[$key] = array(
                    'cele_jmeno' => $radek['jmeno'] . ' ' . $radek['prijmeni'],
                    'jmeno' => $radek['jmeno'],
                    'prijmeni' => $radek['prijmeni'],
                    'telefon' => $radek['kontakt'],
                    'datum' => $radek['datum_format'],
                    'cas' => $radek['cas_format'],
                    'doba' => $radek['doba'],
                    'stoly' => array($radek['stul']),
                    'jidlo' => array(),
                    'jidlo_id' => array(),
                    'jidlo_mnozstvi' => array(),
                    'piti' => array(),
                    'piti_mnozstvi' => array(),
                    'piti_id' => array(),
                    'cena' => 0,
                    'identifikator' => $radek['identifikator']
                );
            }


            /* if ($radek['nazev']) {
              $jidlo = array('item' => $radek['id_jidlo'], 'nazev' => $radek['nazev'], 'cena' => $radek['cena'], 'amount' => $radek['porce'], 'selected' => 1);
              if ($radek['typ_sklenice']) {
              array_push($rezervace_hash[$key]['piti'], $jidlo);
              array_push($rezervace_hash[$key]['piti_id'], $radek['id_jidlo']);
              } else {
              array_push($rezervace_hash[$key]['jidlo'], $jidlo);
              array_push($rezervace_hash[$key]['jidlo_id'], $radek['id_jidlo']);
              }
              $rezervace_hash[$radek['id_rezervace']]['cena'] += $radek['cena'];
              } */
        }
        foreach ($objednavky as $radek) {

            if ($radek['nazev']) {
                $jidlo = array('item' => $radek['id_jidlo'], 'nazev' => $radek['nazev'], 'cena' => $radek['cena'], 'amount' => $radek['porce'], 'selected' => 1);
                if ($radek['typ_sklenice']) {
                    array_push($rezervace_hash[$radek['rezervace']]['piti'], $jidlo);
                    array_push($rezervace_hash[$radek['rezervace']]['piti_id'], $radek['id_jidlo']);
                } else {
                    array_push($rezervace_hash[$radek['rezervace']]['jidlo'], $jidlo);
                    array_push($rezervace_hash[$radek['rezervace']]['jidlo_id'], $radek['id_jidlo']);
                }
                $rezervace_hash[$radek['rezervace']]['cena'] += $radek['cena'];
            }
        }
        //print_r("-----------------------------------------");
        //print_r($rezervace_hash);

        return $rezervace_hash;
    }

    public function vytvorRezervaci($jmeno, $prijmeni, $telefon, $datum, $doba, $stoly, $jidlo, $piti, $id = null, $identifikator = null, $transakce = 1) {
        if ($transakce) {
            Db::zacniTransakci();
        }
        $chyba = 0;
        // print_r($identifikator . '!!!!');
        $insert = array();
        if ($id) {
            //array_push($insert, array( 'id' => $id )); 
        }
        $insert = array(
            'identifikator' => $identifikator,
            'id_rezervace' => $id,
            'jmeno_zakaznika' => $jmeno,
            'prijmeni_zakaznika' => $prijmeni,
            'kontakt_zakaznika' => $telefon,
            'datum' => $datum,
            'doba' => $doba,
            'vlozil' => $_SESSION['uzivatel']['id_zam']
        );
        //print_r($insert);
        if (!Db::insert(
                        $this->tabulka, $insert
                )) {

            $chyba++;
        }
        $id = Db::getLastId();
        //print_r('po insert pocet chyb ' . $chyba . ' .');

        if (!$chyba) {
            foreach ($stoly as $stul) {
                if (!Db::insert(
                                'rezervace_na_stul', array(
                            'rezervace' => $id,
                            'stul' => $stul
                                )
                        )) {

                    $chyba++;
                    break;
                }
            }
        }
        /*
          if ( ! $chyba && ! Db::insert(
          'rezervace_na_stul',
          array(
          'rezervace' => $id,
          'stul' => $stul
          )
          ) ) {

          $chyba++;
          } */
        //print_r('pocet chyb ' . $chyba . ' .');
        if ((!$chyba ) && ( (!empty($jidlo) ) || (!empty($piti) ) )) {
            $obj = new SpravceObjednavek;
            $chyba = $obj->vytvorObjednavku($stoly, $jidlo, $piti, 100, $datum, $id, 0);
        }
        //print_r('pocet chyb ' . $chyba . ' .');
        if ($transakce) {
            if ($chyba) {
                Db::vratZpet();
                return "Došlo k chybě při zápisu do databáze";
            } else {
                Db::potvrd();
                return 0;
            }
        }
        if ($chyba) {
            return 1;
        } else {
            return 0;
        }
    }

    public function upravRezervaci() {
        
    }

    public function vlozObjednavkuKRezervaci() {
        
    }

    public function odstranRezervaci($id, $transakce = 1) {
        if ($transakce) {
            Db::zacniTransakci();
            //print_r("transakce begin  ");
        }

        $chyba = 0;

        $obj = new SpravceObjednavek;
        $objednavky = $obj->vratObjednavkyRezervace($id);

        foreach ($objednavky as $objednavka) {
            $chyba = $obj->zrusObjednavku($objednavka, 0);
            if ($chyba) {
                continue;
            }
        }

        if (!$chyba && !Db::smaz(
                        'rezervace_na_stul', 'WHERE rezervace = ?', array($id))) {
            $chyba++;
        }

        if (!$chyba && !Db::smaz(
                        $this->tabulka, 'WHERE id_rezervace = ?', array($id))) {
            $chyba++;
        }

        if ($transakce) {
            if ($chyba) {
                Db::vratZpet();
                return 1;
            } else {
                Db::potvrd();
                return 0;
            }
        }
        //print_r("jsme v transakce");
        if ($chyba) {
            return 1;
        } else {
            return 0;
        }
    }

}

?>
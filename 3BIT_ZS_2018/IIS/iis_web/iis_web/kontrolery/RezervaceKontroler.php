<?php

class RezervaceKontroler extends Kontroler {

    public function zpracuj($parametry) {
        $this->hlavicka['titulek'] = 'Správa rezervací';

        $this->pohled = 'sprava_rezervaci';
        $params = $this->params;

        $this->overUzivatele(array('spolumajitel', 'servirka'));

        $stoly = array();

        $this->data['datum'] = $_GET['datum'];
        $this->data['od'] = $_GET['od'];
        $this->data['do'] = $_GET['do'];

        $this->data['prijmeni'] = $_GET['prijmeni'] ? $_GET['prijmeni'] : null;
        $this->data['doba'] = $_GET['doba'];

        # ve filtrech
        $this->data['datum_od'] = $_GET['datum_od'];
        //print_r( $this->data['datum_od']);
        $this->data['datum_do'] = $_GET['datum_do'];

        $nalezene_vysledky = array();
        //Db::connect("c159um.forpsi.com", "f85392", "FN3xs4R", "f85392");
        //Db::connect( 'c159um.forpsi.com', 'f85392', 'FN3xs4R', 'f85392' );
        if ($_GET['nova_rezervace']) {
            $this->pohled = 'nova_rezervace';
        }
        $this->data['stoly_pop_spravce'] = array();
        if ($_GET['najit_stoly']) {
            $spravceStolu = new SpravceStolu();
            $data_sql = $this->zpracujDatumDoSql($params['datum'][0], $params['cas'][0]);
            //print_r(aaaa);
            $this->data['stoly_pop_spravce'] = $spravceStolu->vratVolneStoly($data_sql, $params['doba'][0]);
            //print_r($stoly);
        }

        if ($_GET['hledej']) {
            $za_url .= 'hledej=1&';
            $rez = new SpravceRezervace;
            $od = new DateTime($this->data['datum_od']);

            $datum_od = $od->format('YmdHis');

            $do = new DateTime($this->data['datum_do']);
            // pridame 24 hod chceme cely den at se bere konec dne
            $do->add(new DateInterval('PT' . 24 . 'H'));
            $datum_do = $do->format('YmdHis');

            // tady musi byt i to rovna se
            if ($datum_od >= $datum_do) {
                $this->pridejZpravu("Datum Odkdy je větší než datum Dokdy", ZPRAVA_CHYBA);
                //$this->presmeruj('rezervace');
            }

        //    $za_url = '';
            $nalezene_rezervace = $rez->rezervaceInfo(null, null, $this->data['prijmeni'], $datum_od, $datum_do);
            //print_r($nalezene_rezervace);
            if ($nalezene_rezervace && !empty($nalezene_rezervace)) {
                foreach ($nalezene_rezervace as $id => $value) {
                    if (!$value['jmeno']) {
                        continue;
                    }
                    $rezervace = array(
                        'id_rezervace' => $id,
                        'jmeno' => $nalezene_rezervace[$id]['jmeno'],
                        'prijmeni' => $nalezene_rezervace[$id]['prijmeni'],
                        'telefon' => $nalezene_rezervace[$id]['telefon'],
                        'datum' => $nalezene_rezervace[$id]['datum'],
                        'cas' => $nalezene_rezervace[$id]['cas'],
                        'doba' => $nalezene_rezervace[$id]['doba'],
                        'stul' => $nalezene_rezervace[$id]['stoly'],
                        'jidlo' => $nalezene_rezervace[$id]['jidlo_id'],
                        'piti' => $nalezene_rezervace[$id]['piti_id'],
                        'cena' => $nalezene_rezervace[$id]['cena'] . ' Kč',
                        'jidlo_mnozstvi' => $nalezene_rezervace[$id]['jidlo'],
                        'piti_mnozstvi' => $nalezene_rezervace[$id]['piti'],
                        'identifikator' => $nalezene_rezervace[$id]['identifikator'],
                    );
                    array_push($nalezene_vysledky, $rezervace);
                }
            } else {
                $this->pridejZpravu("Nebyly nalezeny žádné rezervace", ZPRAVA_CHYBA);
            }
        } elseif ($_GET['hledej_vse']) {
            $za_url .= 'hledej_vse=1&';
            $rez = new SpravceRezervace;
            $nalezene_rezervace = $rez->rezervaceInfo();
            //print_r($nalezene_rezervace);

            if ($nalezene_rezervace) {
                //print_r($nalezene_rezervace);
                foreach ($nalezene_rezervace as $id => $value) {
                    //continue;

                    $rezervace = array(
                        'id_rezervace' => $id,
                        'jmeno' => $nalezene_rezervace[$id]['jmeno'],
                        'prijmeni' => $nalezene_rezervace[$id]['prijmeni'],
                        'telefon' => $nalezene_rezervace[$id]['telefon'],
                        'datum' => $nalezene_rezervace[$id]['datum'],
                        'cas' => $nalezene_rezervace[$id]['cas'],
                        'doba' => $nalezene_rezervace[$id]['doba'],
                        'stul' => $nalezene_rezervace[$id]['stoly'],
                        'jidlo' => $nalezene_rezervace[$id]['jidlo_id'],
                        'jidlo_mnozstvi' => $nalezene_rezervace[$id]['jidlo'],
                        'piti_mnozstvi' => $nalezene_rezervace[$id]['piti'],
                        'piti' => $nalezene_rezervace[$id]['piti_id'],
                        //'cena' => $nalezene_rezervace[$id]['cena'] . ' Kč',
                        'identifikator' => $nalezene_rezervace[$id]['identifikator'],
                    );
                    array_push($nalezene_vysledky, $rezervace);
                }
            }
        }
        $za_url .= 'datum_od=' . $this->data['datum_od'] . '&datum_do=' . $this->data['datum_do'];

        if ($_GET['nova_rezervace'] && $_GET['uloz_rezervaci']) {


            $vysl = $this->vytvorRezervaci($params);
            //$this->pridejZpravu('Rezervace byla vytvořena', ZPRAVA_OK);
            if ($vysl) {
                $this->pridejZpravu('Nepodařilo se vytvořit rezervaci: ' . $vysl, ZPRAVA_CHYBA);
            } else {
                $this->pridejZpravu("Rezervace byla úspěšně vytvořena", ZPRAVA_OK);
                $this->presmeruj('rezervace');
            }
            //$this->presmeruj('rezervace');
        } else if ($_GET['uloz_rezervaci']) {
            $id = $params['id_rezervace'][0];
            $zprava = $this->upravRezervaci($params, $id);

            if (!$zprava) {
                $this->presmeruj('rezervace?' . $za_url);
            }
            //$this->pridejZpravu($zprava, ZPRAVA_OK);
            //$this->presmeruj('rezervace');
        } else if ($_GET['zrus_rezervaci']) {
            $vysl = $this->zrusRezervaci($_GET['id']);
            if ($vysl) {
                $this->pridejZpravu("Záznam nelze odstranit, chyba na straně databáze", ZPRAVA_CHYBA);
            } else {
                $this->pridejZpravu("Záznam byl úspěšně odstraněn", ZPRAVA_OK);
                $this->presmeruj('rezervace?' . $za_url);
            }
        }

        $this->data['vysledek'] = $nalezene_vysledky;
        $spravceStolu = new SpravceStolu;
        $stoly = $spravceStolu->stulIdNazev();

        $this->data['stoly_pop'] = $stoly; // popup pro stoly

        $spravceJidla = new SpravceJidla;
        $jidlo = $spravceJidla->jidloIdNazev(0);
        $piti = $spravceJidla->jidloIdNazev(1);
        $this->data['piti_pop'] = $piti;
        $this->data['jidlo_pop'] = $jidlo;

        //potrebujeme datum v takovem formatu
        SetLocale(LC_ALL, "Czech");
        //$this->data['dnes'] = date("Y-m-d\TH:i");
        $this->data['dnes'] = date("Y-m-d");
        $this->data['vybrano'] = 4;
        //$this->pridejZpravu('Chyba', ZPRAVA_CHYBA);
        //$this->data['zpravy'] = $this->vratZpravy();
    }

    private function vytvorRezervaci($params, $id = null, $transakce = 1) {
        //print_r($params);
        if ($params['stoly'] && !empty($params['stoly'])) {
            foreach ($params['stoly'] as $stul) {
                if (!is_numeric($stul) && !empty($stul)) {
                    return 'Nezadali jste validní stůl';
                }
                if (count(array_keys($params['stoly'], $stul)) > 1) {
                    return 'Zarezervovali jste jeden stůl vícekrát.';
                }
            }
        } else {
            return 'Není vybrán žádný stůl';
        }
        $jidlo_mnozstvi = array();
        $piti_mnozstvi = array();

        if ($params['jidlo'] && !empty($params['jidlo'])) {
            if (isset($params['mnozstvi_jidlo']) && !empty($params['mnozstvi_jidlo'])) {
                if (count($params['mnozstvi_jidlo']) < count($params['jidlo'])) {
                    return "Nebylo zadáno množství u jídla";
                }
            }
            $i = 0;
            foreach ($params['jidlo'] as $jidlo) {
                //print_r($jidlo . '<br>');
                if (!is_numeric($jidlo)) {
                    return "Nevybrali jste validní jídlo";
                }
                if (!$params['mnozstvi_jidlo'][$i]) {
                    $params['mnozstvi_jidlo'][$i] = 1;
                }
                array_push($jidlo_mnozstvi, array(id => $jidlo, mnozstvi => $params['mnozstvi_jidlo'][$i]));
                $i++;

                if (count(array_keys($params['jidlo'], $jidlo)) > 1) {
                    return "Nepřidávejte stejné jídlo vícekrát, použijte množství";
                }
            }
        }

        if ($params['piti'] && !empty($params['piti'])) {
            if (isset($params['mnozstvi_piti']) && !empty($params['mnozstvi_piti'])) {
                if (count($params['mnozstvi_piti']) < count($params['piti'])) {
                    return "Nebylo zadáno množství u pití";
                }
            }
            $i = 0;
            foreach ($params['piti'] as $piti) {
                if (!is_numeric($piti) && !empty($piti)) {
                    return 'Pití není validní';
                }
                if (!$params['mnozstvi_piti'][$i]) {
                    $params['mnozstvi_piti'][$i] = 1;
                }
                array_push($piti_mnozstvi, array(id => $piti, mnozstvi => $params['mnozstvi_piti'][$i]));
                $i++;
                if (count(array_keys($params['piti'], $piti)) > 1) {
                    return "Nepřidávejte stejné pití vícekrát, použijte množství";
                }
            }
        }
        
        if (!$params['jmeno'][0] || !$params['prijmeni'][0] || !$params['telefon'][0] || !$params['datum'][0] || !$params['cas'][0] || !$params['doba'][0]) {
            return "Není vyplněno jméno, kontakt, doba, čas nebo datum";
        }
        $rez = new SpravceRezervace;
        $identifikator = $params['identifikator'][0];
        if (!$params['identifikator'][0]) {
            // vytvorime ho
            do {
                $identifikator = uniqid();
                //print_r($identifikator);
                // existuje uz takovy identifikator?
                $nalezene_rezervace = $rez->rezervaceInfo(null, null, null, null, null, $identifikator);
            } while (!empty($nalezene_rezervace));
            $this->pridejZpravu("Identifikátor nové rezervace je $identifikator, dobře si ho uschovejte", ZPRAVA_OK);
        }


        $data_sql = $this->zpracujDatumDoSql($params['datum'][0], $params['cas'][0]);
        //print_r('datum:' . $data_sql);
        // tady to chce prevest datum na datum i s casem
        return $rez->vytvorRezervaci($params['jmeno'][0], $params['prijmeni'][0], $params['telefon'][0], $data_sql, $params['doba'][0], $params['stoly'], $jidlo_mnozstvi, $piti_mnozstvi, $id, $identifikator, $transakce);
    }

    public function zpracujDatumDoSql($datum, $cas) {
        // prevedeni casu a data
        $cas = explode(':', $cas);
        //print_r('explode ');
        //print_r($cas);
        $datum = explode('-', $datum);

        $date = mktime($cas[0], $cas[1], 0, $datum[1], $datum[2], $datum[0]);
        //echo 'minuta ' . $cas[1];
        $data_sql = date("YmdHis", $date);


        return $data_sql;
    }

    private function zrusRezervaci($id, $transakce = 1) {
        $rez = new SpravceRezervace;
        $vysl = $rez->odstranRezervaci($id, $transakce);

        return $vysl;
    }

    private function upravRezervaci($params, $id) {
        // zatim to umi asi jen mazat, zprovoznit
        $transakce = 0;
        Db::zacniTransakci();
        $vysl = $this->zrusRezervaci($id, $transakce);

        if ($vysl) {
            // nastala nekde chyba, nebudeme vkladat
            $this->pridejZpravu("Záznam nelze odstranit, chyba na straně databáze", ZPRAVA_CHYBA);
            Db::vratZpet();
            return 1;
        } else {
            $vysl = $this->vytvorRezervaci($params, $id, $transakce);
            if ($vysl) {
                $this->pridejZpravu("Záznam nelze upravit", ZPRAVA_CHYBA);
                Db::vratZpet();
            } else {
                $this->pridejZpravu("Záznam byl úspěšně upraven", ZPRAVA_OK);
                Db::potvrd();
            }
        }
        return $vysl;
    }

}

?>
<?php
/**
 * @project IIS - Restaurace
 * 
 * @author Jan Sorm (xsormj00), Alena Tesarova (xtesar36)
 * @file ObjednavkyKontroler.php
 * @brief Kontroler pro zpracovani argumentu ve sprave objednavek a pri zakladani nove objednavky.
 */
class ObjednavkyKontroler extends Kontroler {

    public function zpracuj($parametry) {
        $this->overUzivatele(array('spolumajitel', 'servirka'));

        $this->pohled = 'sprava_objednavek';
        $this->hlavicka['titulek'] = 'Správa objednávek';
        $spravceObjednavek = new SpravceObjednavek();
        $params = $this->params;
        
        $this->data['zobraz_historii'] = 0;
        if ( $_SESSION['uzivatel']['pozice'] == 'spolumajitel' ) {
            $this->data['zobraz_historii'] = 1;
        }

        // JQUERY
        // odstranujeme potravinu u objednavky
        if ($parametry[0] == 'odstran' && $parametry[1]) {
            //echo ($parametry[1] );
            $data = explode('-', $parametry[1]);

            $vysl = $spravceObjednavek->zrusPotravinuKObjednavceId($data[0]);
            $this->pohled = 'blank'; // pokud volame ajaxove, vzdy musi byt pohled blank !!!
            //$vysl['success'] = !$vysl['vysl'];
            $vysl_json = json_encode($vysl);
            echo $vysl_json;
            return;
        }
        // JQUERY
        // upravujeme mnozstvi potraviny u objednavky
        if ($parametry[0] == 'saveOnClick') {
            $this->pohled = 'blank';
            $data = explode('-', $params['id'][0]);
            $pocet_porci = $params['pocet_porci'][0];
            //echo $_GET['bilkoviny'];
            //echo $_GET['cena'];
            
            $vysl = $spravceObjednavek->zmenMnozstviId($data[0], $pocet_porci);

            $vysl_json = json_encode($vysl);
            // vracime vysledek
            echo $vysl_json;
            return 0;
        }

        // pokud chceme novou objednavku, tak menime pohled
        if ($_GET['nova_objednavka']) {
            $this->pohled = 'nova_objednavka';
        }
        // zde obsluhujeme filtrovani ve sprave objednavek
        if ($params['hledej'] || $params['hledej_vse']) {

            $datum_od = null;
            $datum_do = null;
            $chyby = 0;

            if (!$params['hledej_vse']) {

                $od = new DateTime($params['datum_od'][0]);
                $datum_od = $od->format('YmdHis');

                $do = new DateTime($params['datum_do'][0]);
                // pridame 24 hod chceme cely den at se bere konec dne
                $do->add(new DateInterval('PT' . 24 . 'H'));
                $datum_do = $do->format('YmdHis');

                // tady musi byt i to rovna se
                if ($datum_od >= $datum_do) {
                    $this->pridejZpravu("Datum Odkdy je větší než datum Dokdy", ZPRAVA_CHYBA);
                    $chyby++;
                }
            }

            // pokud je vse bez chyby, tak volame modelove funkce, ktere nam vraci nalezena data, ktera predavame modelu
            if (!$chyby) {
                $objednavka = $spravceObjednavek->objednavkaInfo(null, $datum_od, $datum_do, 1);
                //print_r($objednavka);

                $nalezene_vysledky = array();
                if ($objednavka && !empty($objednavka)) {

                    $nalezene_vysledky = $objednavka;
                } else {
                    $this->pridejZpravu("Nebyly nalezeny žádné objednávky", ZPRAVA_CHYBA);
                    //$this->presmeruj('objednavky');
                }
                $this->data['vysledek_historie'] = $objednavka;
            }
        }

        // vytvarime novou objednavku
        if ($_GET['nova_objednavka'] && $_GET['uloz_objednavku']) {
            $this->vytvorObjednavku($params);
        }
        // pridavame dalsi udaje, k jiz existujici objednavce
        if ($params['uloz_objednavku'] && $params['objednavka']) {
            $this->vytvorObjednavku($params, 1);
        }


        // zde volame modelovou funkci pro vypsani nezauctovanych objednavek v pohledu
        if ($params['ok']) {
            $info = $spravceObjednavek->nezauctovaneObjednavky($params['stoly']);
            $nalezene_vysledky = array();

            if ($info && !empty($info)) {
            } else {
                $this->pridejZpravu("Nebyla nalezena žádná objednávka", ZPRAVA_CHYBA);
                $this->presmeruj('objednavky');
            }
            $this->data['vysledek'] = $info;
        }
        // zde probiha tisk uctenky
        if ($params['tisk_uctenky'] && $params['objednavka'][0]) {
            $info = $spravceObjednavek->tiskUctenky($params['objednavka'][0]);
            if ($info) {
                $this->pridejZpravu("Nebylo možné účtenku vytiknout", ZPRAVA_CHYBA);
            } else {
                $this->pridejZpravu("Účtenka byla úspěšně vytvořena", ZPRAVA_OK);
                $this->presmeruj('objednavky');
            }
            //$this->presmeruj('objednavky');
        }

        // zde davame pohledu doplnujici data pro popupy
        $spravceStolu = new SpravceStolu;
        $stoly = $spravceStolu->stulIdNazev();
        $this->data['stoly_pop'] = $stoly; // popup pro stoly

        $spravceJidla = new SpravceJidla;
        $jidlo = $spravceJidla->jidloIdNazev(0);
        $piti = $spravceJidla->jidloIdNazev(1);
        $this->data['piti_pop'] = $piti;
        $this->data['jidlo_pop'] = $jidlo;
    }

    /**
     * @brief   Funkce kontroluje predane parametry a vytvari novou objednavku.
     * 
     * @param type $parametry           predavane parametry v url
     * @param 1/0 $pridej_k_objednavce  zda chceme vytvorit novou objednavku
     * @return Vracime popis chyby nebo 0.
     */
    public function vytvorObjednavku($parametry, $pridej_k_objednavce = 0) {
        $chyba = 0;

        $jidlo_mnozstvi = array();
        $piti_mnozstvi = array();
        $spravceObjednavek = new SpravceObjednavek;

        // kontrolujeme, zda je vybrana nejaka potravina
        if (!( $parametry['jidlo'][0] || $parametry['piti'][0] )) {
            $this->pridejZpravu("Nevybráno, žádné jídlo nebo pití.", ZPRAVA_CHYBA);
            return 1;
        }
        // kontrolujeme stoly
        if ($parametry['stul'][0]) {
            foreach ($parametry['stul'] as $stul) {
                if (!is_numeric($stul)) {
                    $this->pridejZpravu("Nesprávně vybraný stůl.", ZPRAVA_CHYBA);
                    $chyba++;
                    break;
                }
                if (count(array_keys($parametry['stul'], $stul)) > 1) {
                    $this->pridejZpravu("Nesprávně vybraný stůl.", ZPRAVA_CHYBA);
                    $chyba++;
                    break;
                }
            }
        } else {
            $this->pridejZpravu("Nebyl vybrán žádný stůl.", ZPRAVA_CHYBA);
            $chyba++;
        }
        $i = 0;
        if ($parametry['jidlo'][0]) {
            if (isset($parametry['mnozstvi_jidlo']) && !empty($parametry['mnozstvi_jidlo'])) {
                if (count($parametry['mnozstvi_jidlo']) < count($parametry['jidlo'])) {
                    $this->pridejZpravu("Nebylo zadáno množství u jídla", ZPRAVA_CHYBA);
                    return 'Nebylo zadáno množství u jídla';
                }
            }
            // kontrolujeme spravnost zadanych jidel - ciselnost, kladnost, pocet stejnych
            foreach ($parametry['jidlo'] as $jidlo) {
                if (!is_numeric($jidlo)) {
                    $this->pridejZpravu("Nesprávně vybrané jídlo.", ZPRAVA_CHYBA);
                    $chyba++;
                    break;
                }
                if (count(array_keys($parametry['jidlo'], $jidlo)) > 1) {
                    $this->pridejZpravu("Nemůžeš vybrat dvakrát stejné jídlo.", ZPRAVA_CHYBA);
                    $chyba++;
                    break;
                }
                if (!$parametry['mnozstvi_jidlo'][$i]) {
                    //print_r('mnozstvi je 1');
                    $parametry['mnozstvi_jidlo'][$i] = 1;
                }
                if ($parametry['mnozstvi_jidlo'][$i] < 0) {
                    $this->pridejZpravu("Nemůžeš zadat záporné množství", ZPRAVA_CHYBA);
                    $chyba++;
                    break;
                }
                
                array_push($jidlo_mnozstvi, array(id => $jidlo, mnozstvi => $parametry['mnozstvi_jidlo'][$i]));
                $i++;

                if ($pridej_k_objednavce && $spravceObjednavek->existujePotravina($parametry['objednavka'][0], $jidlo)) {
                    $this->pridejZpravu("Nemůžeš přidat k objednávce stejné jídlo, navyš pouze množství", ZPRAVA_CHYBA);
                    $chyba++;
                    break;
                }
            }
        }
        $i = 0;
        // to stejne, co bylo u jidla, zkontrolujeme i u piti
        if ($parametry['piti'][0]) {
            if (isset($parametry['mnozstvi_piti']) && !empty($parametry['mnozstvi_piti'])) {
                if (count($parametry['mnozstvi_piti']) < count($parametry['piti'])) {
                    $this->pridejZpravu("Nebylo zadáno množství u pití", ZPRAVA_CHYBA);
                    return 'Nebylo zadáno množství u pití';
                }
            }

            foreach ($parametry['piti'] as $piti) {
                if (!is_numeric($piti)) {
                    $this->pridejZpravu("Nesprávně vybrané pití.", ZPRAVA_CHYBA);
                    $chyba++;
                    break;
                }
                if (count(array_keys($parametry['piti'], $piti)) > 1) {
                    $this->pridejZpravu("Nemůžeš vybrat dvakrát stejné pití.", ZPRAVA_CHYBA);
                    $chyba++;
                    break;
                }
                if (!$parametry['mnozstvi_piti'][$i]) {
                    $parametry['mnozstvi_piti'][$i] = 1;
                }
                if ($parametry['mnozstvi_piti'][$i] < 0) {
                    $this->pridejZpravu("Nemůžeš zadat záporné množství", ZPRAVA_CHYBA);
                    $chyba++;
                    return 0;
                }

                array_push($piti_mnozstvi, array(id => $piti, mnozstvi => $parametry['mnozstvi_piti'][$i]));
                $i++;
                $potr = $spravceObjednavek->existujePotravina($parametry['objednavka'][0], $piti);

                if ($pridej_k_objednavce && $spravceObjednavek->existujePotravina($parametry['objednavka'][0], $piti)) {
                    $this->pridejZpravu("Nemůžeš přidat k objednávce stejné pití, navyš pouze množství", ZPRAVA_CHYBA);
                    $chyba++;
                    break;
                }
            }
        }

        if ($chyba) {
            return 1;
        }
        $vysl = 0;

        // rozlisujeme, zda pridavame uz k existujici objednavce nebo vytvarime uplne novou objednavku
        if ($pridej_k_objednavce) {
            if (!$parametry['objednavka'][0]) {
                return "Chybi ID objednavky";
            }
            // pokud pridavame, vola se jina funkce
            //print_r('pridavam k obj');
            $objednavka = $spravceObjednavek->objednavkaInfo($parametry['objednavka'][0]);
            //print_r($objednavka);
            if (empty($objednavka)) {
                // objednavka neexistuje !
                $vysl = $spravceObjednavek->vytvorObjednavku($parametry['stul'], $jidlo_mnozstvi, $piti_mnozstvi, 100);
            } else {
                $vysl = $spravceObjednavek->pridejKObjednavce($parametry['stul'], $jidlo_mnozstvi, $piti_mnozstvi, $parametry['objednavka'][0]);
            }
        } else {
            //print_r('vytvarim');
            $vysl = $spravceObjednavek->vytvorObjednavku($parametry['stul'], $jidlo_mnozstvi, $piti_mnozstvi, 100);
        }

        // kontrolujeme vysledek modelovych funkci
        if ($vysl) {
            $this->pridejZpravu("Při zakládání objednávky došlo k chybě.", ZPRAVA_CHYBA);
            return 1;
        } else {
            $this->pridejZpravu("Objednávka byla úspěšně založena.", ZPRAVA_OK);
            return 0;
        }
    }

}

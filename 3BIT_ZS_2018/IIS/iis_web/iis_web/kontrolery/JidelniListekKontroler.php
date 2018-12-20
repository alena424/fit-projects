<?php
/**
 * @project IIS - Restaurace
 * 
 * @author Jan Sorm (xsormj00), Alena Tesarova (xtesar36)
 * @file JidelniListekKontroler.php
 * @brief Kontroler pro pohledy pracujici s jidelnim listek.
 *      Resi se v nem pridavani a editovani jidla a piti.
 */
class JidelniListekKontroler extends Kontroler {

    public function zpracuj($parametry) {
        $this->overUzivatele(array('spolumajitel', 'sefkuchar'));
        $spravceJidla = new SpravceJidla;
        $params = $this->params;
        $this->hlavicka['titulek'] = 'Jídelní lístek';

        if ($parametry[0] == 'saveOnClick' && $parametry[1] && !$parametry[2]) {
            ##############################
            # ukladame potravinu ajaxove #
            $this->pohled = 'blank';

            // kontrolujeme spravnost predanych parametru - zadani, ciselnost, kladnost
            if (!$params['cena'][0] || !$params['pocet_porci'][0] || !$params['mnozstvi_sacharidu'][0] ||
                    !$params['energie'][0] || !$params['bilkoviny'][0] || !$params['tuky'][0]) {
                $vysl['fail'] = "Nejsou zadané všechny hodnoty.";
            } else if (!is_numeric($params['cena'][0]) || !is_numeric($params['pocet_porci'][0]) ||
                    !is_numeric($params['mnozstvi_sacharidu'][0]) || !is_numeric($params['energie'][0]) ||
                    !is_numeric($params['bilkoviny'][0]) || !is_numeric($params['tuky'][0])) {
                $vysl['fail'] = "Byly zadány nečíselné hodnoty.";
            } else if ($params['cena'][0] < 1 || $params['pocet_porci'][0] < 1 || $params['mnozstvi_sacharidu'][0] < 1 ||
                    $params['energie'][0] < 1 || $params['bilkoviny'][0] < 1 || $params['tuky'][0] < 1) {
                $vysl['fail'] = "Hodnoty musí být větší nebo rovny 1.";
            } else {
                // vse je v poradku, muzeme ulozit upravene jidlo
                $vysl2 = $spravceJidla->upravJidlo($params['id'][0], $params['nazev'][0], $params['cena'][0], $params['pocet_porci'][0], $params['mnozstvi_sacharidu'][0], $params['energie'][0], $params['bilkoviny'][0], $params['tuky'][0]);

                $vysl['success'] = $vysl2;
            }
            //$vysl['fail'] = 'Zly';
            $vysl_json = json_encode($vysl);
            // tim echo mi vratis vysledek
            echo $vysl_json;
            return 0;
        } else if ($parametry[0] == 'saveOnClick' && $parametry[1] && $parametry[2] == 'surovina' && $parametry[3]) {
            #########################################
            # ukladame surovinu k potravine ajaxove #

            $vysl2 = $spravceJidla->upravSurovinuKPotravine($params['id'][0], $params['id_surovina'][0], $params['mnozstvi_surovina'][0]);
            $this->pohled = 'blank';
            //print_r($params);

            $vysl['success'] = !$vysl2;
            //$vysl['fail'] = 'Zly';
            $vysl_json = json_encode($vysl);
            // vracime vysledek
            echo $vysl_json;
            return 0;
        }

        // na zaklade parametru se rozlisuje, ktery pouzijeme pohled
        $this->pohled = 'jidelni_listek';
        if ($_GET['nove_jidlo']) {
            $this->pohled = 'nove_jidlo';
        }
        if ($_GET['nove_piti']) {
            $this->pohled = 'nove_piti';
        }

        $jidlo = ( $_GET['nove_jidlo'] && $_GET['uloz_jidlo'] );

        if ($params['pridat_surovinu_potravina'] && $params['id_jidlo'][0] && $params['surovina'][0]) {
            // neni funkcni !
            $this->pridejSurovinuPotravina($params, $params['id_jidlo'][0]);
        }


        if (( $_GET['nove_jidlo'] && $_GET['uloz_jidlo'] ) || ( $_GET['nove_piti'] && $_GET['uloz_piti'] )) {
            $this->vytvorPotravinu($params, $jidlo);
        } else {
            /** AJAX ODSTRANENI */
            if ($parametry[0] == 'odstran' && $parametry[1] && !$parametry[2]) {
                // odstranujeme celou potravinu
                $smazano = $this->zrusPotravinu($parametry[1]);
                $this->pohled = 'blank'; // pokud volame ajaxove, vzdy musi byt pohled blank !!!
                $vysl['success'] = !$smazano;
                $vysl_json = json_encode($vysl);
                echo $vysl_json;
                return;
            }
            if ($parametry[1] && $parametry[0] == 'odstran' && $parametry[2] == 'surovina' && $parametry[3]) {
                // odstran surovinu od potraviny
                $smazano = $spravceJidla->zrusPotravinuKSurovine($parametry[1], $parametry[3]);
                $this->pohled = 'blank'; // pokud volame ajaxove, vzdy musi byt pohled blank !!!
                $vysl['success'] = !$smazano;
                $vysl_json = json_encode($vysl);
                echo $vysl_json;
                return;
            }
            // vracime data pro pohled
            $this->data['suroviny_jidlo'] = $spravceJidla->vratSurovinyJidlo();
            $this->data['jidlo'] = $spravceJidla->vratJidlo();
            $this->data['piti'] = $spravceJidla->vratPiti();
        }

        // popup s vyberem surovin
        $spravceSurovin = new SpravceSurovin;
        $this->data['surovina_pop'] = $spravceSurovin->surovinaIdNazev();
    }

    /*
     * @brief Funkce pro kontrolu parametru a zalozeni nove potraviny.
     * 
     * @params parametry parametry url
     * @params jidlo    zda pridavame jidlo nebo potravinu
     * @return 1/0      chyba/ok
     */
    private function vytvorPotravinu($parametry, $jidlo) {
        // jake vsechny parametry kontrolujeme
        $parametryHash = array(
            'nazev' => 'Název',
            'cena' => 'Cena',
            'pocet_porci' => 'Počet porcí',
            'mnozstvi_sacharidu' => 'Sacharidy',
            'energie' => 'Energie',
            'bilkoviny' => 'Bílkoviny',
            'tuky' => 'Tuky',
            'expirace' => 'Expirace'
        );
        if ($jidlo) {
            $parametryHash['pribor'] = 'Příbor';
        } else {
            $parametryHash['typ_sklenice'] = 'Typ sklenice';
        }

        // pro kazdy parametr zkontrolujeme, zda je zadany, zda je to cislo a dame ho k dispozici pohledu
        $chyba = 0;
        foreach ($parametryHash as $parametr => $hodnota) {
            if (!$parametry[$parametr][0]) {
                $this->pridejZpravu("Nezadali jste povinný údaj: " . $hodnota, ZPRAVA_CHYBA);
                $chyba++;
                continue;
            }
            if ($parametr != 'nazev' && $parametr != 'expirace' && $parametr != 'pribor' && $parametr != 'typ_sklenice') {
                if (!is_numeric($parametry[$parametr][0])) {
                    $this->pridejZpravu("Údaj " . $hodnota . " musí být číslo.", ZPRAVA_CHYBA);
                    $chyba++;
                }
            }
            $this->data[$parametr] = $parametry[$parametr][0];
        }

        $surovina_mnozstvi = array();
        // kontrola, zda je zadane mnozstvi u surovin
        if ($parametry['mnozstvi_surovina'] && !empty($parametry['mnozstvi_surovina'])) {
            if (isset($parametry['mnozstvi_surovina']) && !empty($parametry['mnozstvi_surovina'])) {
                if (count($parametry['mnozstvi_surovina']) < count($parametry['piti'])) {
                    $this->pridejZpravu("Nebylo zadáno množství u suroviny", ZPRAVA_CHYBA);
                    return 1;
                }
            }
        }
        $i = 0;

        // kontrola, zda je zadana spravne samotna surovina
        foreach ($parametry['surovina'] as $surovina) {

            if (!is_numeric($surovina) && !empty($surovina)) {
                $this->pridejZpravu('Surovina není validní', ZPRAVA_CHYBA);
                $chyba++;
            }
            if (!$parametry['mnozstvi_surovina'][$i]) {
                $parametry['mnozstvi_surovina'][$i] = 1;
            }
            
            if (!is_numeric($parametry['mnozstvi_surovina'][$i])) {
                $this->pridejZpravu('Množství není validní, očekáváme číslo', ZPRAVA_CHYBA);
                $chyba++;
            }
            array_push($surovina_mnozstvi, array(id => $surovina, mnozstvi => $parametry['mnozstvi_surovina'][$i]));
            $i++;

            if (count(array_keys($parametry['mnozstvi_surovina'], $surovina)) > 1) {
                $this->pridejZpravu("Nepřidávejte stejnou surovinu, použijte množství", ZPRAVA_CHYBA);
                $chyba++;
            }
        }

        /* if ($jidlo) {
          if ($parametry['suroviny'][0]) {
          foreach ($parametry['suroviny'] as $surovina) {
          if (!is_numeric($surovina)) {
          $this->pridejZpravu("Nesprávně vybraná surovina.", ZPRAVA_CHYBA);
          $chyba++;
          break;
          }
          if (count(array_keys($parametry['suroviny'], $surovina)) > 1) {
          $this->pridejZpravu("Vybrali jste dvě stejné suroviny.", ZPRAVA_CHYBA);
          $chyba++;
          break;
          }
          }
          } else {
          $this->pridejZpravu("Nebyla vybrána žádná surovina.", ZPRAVA_CHYBA);
          $chyba++;
          }
          } */


        if ($chyba) {
            return 1;
        }

        // vytvorime potravinu pomoci funkce v modelu
        $spravceJidla = new SpravceJidla;
        if ($spravceJidla->vytvorPotravinu(
                        $parametry['nazev'][0], $parametry['cena'][0], $parametry['pocet_porci'][0], $parametry['energie'][0], $parametry['tuky'][0], $parametry['mnozstvi_sacharidu'][0], $parametry['bilkoviny'][0], $parametry['pribor'][0], $parametry['typ_sklenice'][0], $parametry['expirace'][0], $surovina_mnozstvi
                )) {

            $this->pridejZpravu("Při zakládání potraviny došlo k chybě.", ZPRAVA_CHYBA);
            return 1;
        } else {
            $this->pridejZpravu("Potravina byla úspěšně přidána.", ZPRAVA_OK);
            return 0;
        }
    }

    /**
     * @brief Funkce pro kontrolu parametru a pridani suroviny k jidlu
     * 
     * @param int $params   parametry v url
     * @param bool $jidlo   zda pridavame k jidlu nebo k piti
     * @return 1/0  chyba/ok
     */
    private function pridejSurovinuPotravina($params, $jidlo) {

        $chyba = 0;
        if ($jidlo) {
            $spravceJidla = new SpravceJidla;
            $surovina_mnozstvi = array();
            // existuje jiz tato surovina u potraviny?

            // kontrola, zda bylo zadano mnostvi u suroviny
            if ($params['mnozstvi_surovina'] && !empty($params['mnozstvi_surovina'])) {
                if (isset($params['mnozstvi_surovina']) && !empty($params['mnozstvi_surovina'])) {
                    if (count($params['mnozstvi_surovina']) < count($params['piti'])) {
                        $chyba++;
                        $this->pridejZpravu("Nebylo zadáno množství u suroviny", ZPRAVA_CHYBA);
                    }
                }
            }
            $i = 0;

            // Kontrola, zda je surovina validni a zda uz neni evidovana u dane potraviny
            foreach ($params['surovina'] as $surovina) {

                $info = $spravceJidla->jidloInfo($jidlo, $surovina);

                if (!empty($info)) {
                    $this->pridejZpravu("Daná surovina už je evidována u jídla", ZPRAVA_CHYBA);
                    $chyba++;
                    return 1;
                }

                if (!is_numeric($surovina) && !empty($surovina)) {
                    $this->pridejZpravu('Surovina není validní', ZPRAVA_CHYBA);
                    $chyba++;
                    return 1;
                }
                if (!$params['mnozstvi_surovina'][$i]) {
                    $params['mnozstvi_surovina'][$i] = 1;
                }

                if ($params['mnozstvi_surovina'][$i] < 1) {
                    $chyba++;
                    $this->pridejZpravu("Při přidávání suroviny došlo k chybě.", ZPRAVA_CHYBA);
                    return 1;
                }

                array_push($surovina_mnozstvi, array(id => $surovina, mnozstvi => $params['mnozstvi_surovina'][$i]));
                $i++;

                print_r($params['mnozstvi_surovina']);
               
                if (count(array_keys($params['surovina'], $surovina)) > 1) {
                    $this->pridejZpravu("Nepřidávejte stejnou surovinu, použijte množství", ZPRAVA_CHYBA);
                    return 1;
                    $chyba++;
                }
            }

            if ($chyba || empty($surovina_mnozstvi)) {
                return 1;
            }


            if ($spravceJidla->pridejSurovinyKPotravine($jidlo, $surovina_mnozstvi)) {
                $this->pridejZpravu("Při přidávání suroviny došlo k chybě.", ZPRAVA_CHYBA);
                return 1;
            } else {
                $this->pridejZpravu("Surovina byla úspěšně přidána.", ZPRAVA_OK);
                return 0;
            }
        }
    }

    /**
     * @brief Funkce pro zruseni potraviny.
     * 
     * @param int $id   id rusene potraviny
     * @return 1/0  chyba/ok
     */
    private function zrusPotravinu($id) {
        // volame odebiraci funkci z modelu
        $spravceJidla = new SpravceJidla;
        if ($spravceJidla->zrusPotravinu($id)) {
            $this->pridejZpravu("Při rušení potraviny došlo k chybě.", ZPRAVA_CHYBA);
            return 1;
        } else {
            $this->pridejZpravu("Potravina byla úspěšně zrušena.", ZPRAVA_OK);
            return 0;
        }
    }

}

?>
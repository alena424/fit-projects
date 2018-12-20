<?php

class StolyKontroler extends Kontroler {

    public function zpracuj($parametry) {
        $this->overUzivatele(array('spolumajitel'));

        $this->pohled = 'sprava_stolu';
        $this->hlavicka['titulek'] = 'Stoly';

        $spravceStolu = new SpravceStolu;
        if ($this->params['pridat_stul']) {
            $this->vytvorStul($spravceStolu);
        } else if ($parametry[0] == 'odstran' && $parametry[1]) {
            // ajoxove mazani
            $smazano = $this->smazStul($spravceStolu, $parametry[1]);
            $this->pohled = 'blank';
            //$this->vratZpravy(); -- aby nezustali v session
            $vysl['success'] = !$smazano;
            $vysl_json = json_encode($vysl);
            echo $vysl_json;
            return;
        }

        $stoly = $spravceStolu->vratStoly();
        $this->data['stoly'] = array();
        foreach ($stoly as $stul) {
            $pole = array('cislo' => $stul['cislo'], 'umisteni' => $stul['umisteni'], 'id' => $stul['id_stul'], 'pocet_osob' => $stul['pocet_osob']);
            array_push($this->data['stoly'], $pole);
        }
    }

    private function vytvorStul($spravceStolu) {
        $parametry = array(
            'cislo' => 'Číslo stolu',
            'umisteni' => 'Označení',
            'pocet_osob' => 'Počet osob'
        );
        $chyba = 0;
        foreach ($parametry as $parametr => $hodnota) {
            if (!$_GET[$parametr]) {
                $this->pridejZpravu("Nesprávně zadaný údaj: " . $hodnota, ZPRAVA_CHYBA);
                $chyba++;
                continue;
            }
            $this->data[$parametr] = $_GET[$parametr];
        }
        if (!$chyba) {
            if ($spravceStolu->vytvorStul($this->data['umisteni'], $this->data['pocet_osob'], $this->data['cislo'])) {
                $this->pridejZpravu("Při zakládání stolu došlo k chybě.", ZPRAVA_CHYBA);
                return 1;
            } else {
                $this->pridejZpravu("Stůl byl úspěšně přidán.", ZPRAVA_OK);
                return 0;
            }
        }
        return 1;
    }

    private function smazStul($spravceStolu, $id) {
        //print_r($id);
        if ($spravceStolu->zrusStul($id)) {
            $this->pridejZpravu("Při rušení stolu došlo k chybě.", ZPRAVA_CHYBA);
            return 1;
        } else {
            $this->pridejZpravu("Stůl byl úspěšně zrušen.", ZPRAVA_OK);
            return 0;
        }
    }

}

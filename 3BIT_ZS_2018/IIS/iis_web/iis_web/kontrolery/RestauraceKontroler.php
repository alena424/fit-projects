<?php

class RestauraceKontroler extends Kontroler {

    public function zpracuj($parametry) {
        // Vytvoření instance modelu, který nám umožní pracovat s články
        // Není zadáno URL článku, vypíšeme všechny
        $this->hlavicka['titulek'] = "WAWE";
       // $this->data['clanky'] = "pujee";
        $this->pohled = 'hlavni_strana';
        //$this->data['zpravy'] = $this->vratZpravy();

        if ($_GET && $_GET['id_rezervace']) {
            $id = $_GET['id_rezervace'];
            // chce si zobrazit rezervaci
            $rez = new SpravceRezervace();
            $rezervace = $rez->rezervaceInfo(null, null, null, null, null, $id);
            //$rez->rezervaceInfo($id, $date, $surname, $od, $do, $identifikator)

            if ($rezervace && (!empty($rezervace) )) {
                $this->data['vysledek'] = $rezervace;
                foreach ($rezervace as $id => $value) {
                    $this->data['id'] = $id;
                    $this->data['identifikator'] = $rezervace[$id]['identifikator'];

                    $this->data['jmeno'] = $rezervace[$id]['cele_jmeno'];
                    $this->data['telefon'] = $rezervace[$id]['telefon'];
                    $this->data['datum'] = $rezervace[$id]['datum'];
                    $this->data['cas'] = $rezervace[$id]['cas'];
                    $this->data['doba'] = $rezervace[$id]['doba'];
                    $this->data['stoly'] = $rezervace[$id]['stoly'];
                    $this->data['objednavky'] = $rezervace[$id]['jidlo'];
                    $this->data['piti'] = $rezervace[$id]['piti'];
                    $this->data['celkova_cena'] = $rezervace[$id]['cena'];
                }
            } else {
                // chyba
                $this->pridejZpravu('Neexistující ID rezervace', ZPRAVA_CHYBA);
                $this->presmeruj('restaurace/uvod');
            }

            $this->pohled = 'rezervace_info';
        } else {
            // tady by to chtelo nejake informace o jidle a piti
            $this->data['jidlo'] = array();
            $this->data['piti'] = array();

            $jidloSpravce = new SpravceJidla;
            $data = $jidloSpravce->dostupneJidlo();
            foreach ($data as $radek) {
                if ($radek['typ_sklenice']) {
                    $piti = array('nazev' => $radek['nazev'], 'cena' => $radek['cena'] . ' Kč');
                    array_push($this->data['piti'], $piti);
                } else {
                    $jidlo = array('nazev' => $radek['nazev'], 'cena' => $radek['cena'] . ' Kč');
                    array_push($this->data['jidlo'], $jidlo);
                }
            }
        }
    }

}

?>
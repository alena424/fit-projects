<?php

class SmerovacKontroler extends Kontroler {

    protected $kontroler;
    public $params;

    public function zpracuj($parametry) {
        $naparsovanaURL = $this->parsujURL($parametry[0]);
        $params = $this->parametryUrl();
        define("ZPRAVA_CHYBA", -1);
        define("ZPRAVA_OK", 1);

        // nemame zadne parametry, presmerujeme na uvodni stranku
        if (empty($naparsovanaURL[0]))
            $this->presmeruj('restaurace/uvod');

        //print_r($naparsovanaURL);
        //array_shift = 1. parametr
        $nazev_kontroleru = array_shift($naparsovanaURL);
        

        $tridaKontroleru = $this->pomlckyDoVelbloudiNotace($nazev_kontroleru) . 'Kontroler';


        if (file_exists('kontrolery/' . $tridaKontroleru . '.php'))
            $this->kontroler = new $tridaKontroleru;
        else
            $this->presmeruj('chyba');

        $this->kontroler->params = $params;

        $this->kontroler->zpracuj($naparsovanaURL);

        $this->data['titulek'] = $this->kontroler->hlavicka['titulek'];
        $this->data['popis'] = $this->kontroler->hlavicka['popis'];
        $this->data['klicova_slova'] = $this->kontroler->hlavicka['klicova_slova'];
        $this->data['zpravy'] = $this->vratZpravy();
        $this->data['zalozka'] = $nazev_kontroleru;
        $this->kontroler->data['zalozka'] = $nazev_kontroleru;
        $this->kontroler->data['params'] = $params;
        if ($this->kontroler->pohled != 'blank') {
            // kvuli jquery
            $this->pohled = 'rozlozeni';
        }
        //$this->pohled = 'rozlozeni';
    }

    private function parametryURL() {
        $query = explode('&', $_SERVER['QUERY_STRING']);
        $params = array();
        if (!empty($query)) {
            foreach ($query as $param) {
                list( $name, $value ) = explode('=', $param, 2);
                if ($value) {
                    $params[urldecode($name)][] = urldecode($value);
                }
            }
        }
        return $params;
    }

    // vraci pole parametru v url
    private function parsujURL($url) {
        $naparsovanaURL = parse_url($url);
        $naparsovanaURL["path"] = ltrim($naparsovanaURL["path"], "/");
        $naparsovanaURL["path"] = trim($naparsovanaURL["path"]);

        $rozdelenaCesta = explode("/", $naparsovanaURL["path"]);
        return $rozdelenaCesta;
    }

    // prevede vypis-uzivatele na VypisUzivatelu -- nazev kontroleru
    private function pomlckyDoVelbloudiNotace($text) {
        $veta = str_replace('-', ' ', $text);
        $veta = ucwords($veta);
        $veta = str_replace(' ', '', $veta);
        return $veta;
    }

}

?>
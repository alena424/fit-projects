<?php
/**
 * @project IIS - Restaurace
 * 
 * @author Jan Sorm (xsormj00), Alena Tesarova (xtesar36)
 * @file Kontroler.php
 * @brief 
 */
abstract class Kontroler {

    public $data = array();
    public $pohled = "";
    public $hlavicka = array('titulek' => '', 'klicova_slova' => '', 'popis' => '');

    // trida na zpracovani argumentu, kazdy kontroler musi mit
    abstract function zpracuj($parametry);

    // vypise pohled uzivateli
    public function vypisPohled() {
        //echo ("pohled: " . $this->pohled );
        if ($this->pohled) {
            extract($this->osetri($this->data)); // napr. barva => cerva vytvori promennou $barva
            // vybalime promene jeste jednou, tentokrat neosetrene s podtrzitkem
            extract($this->data, EXTR_PREFIX_ALL, "");
            require("pohledy/" . $this->pohled . ".phtml");
        }
    }

    public function presmeruj($url) {
        //print_r($url);
        header("Location: /$url");
        header("Connection: close");
        exit;
    }

    public function pridejZpravu($zprava, $typ) {
        $typ_text = '';
        if ($typ == ZPRAVA_OK) {
            $typ_text = 'zprava_ok';
        } else {
            $typ_text = 'zprava_ko';
        }
        if (isset($_SESSION['zpravy']))
            $_SESSION['zpravy'][] = array(obsah => $zprava, typ => $typ_text);
        else
            $_SESSION['zpravy'] = array(array(obsah => $zprava, typ => $typ_text));

        $_SESSION['typ_zpravy'] = $typ;
    }

    public static function vratZpravy() {
        if (isset($_SESSION['zpravy'])) {
            $zpravy = $_SESSION['zpravy'];
            unset($_SESSION['zpravy']);
            return $zpravy;
        } else
            return array();
    }

    /* public static function vratTypZpravy(){
      if (isset($_SESSION[ 'typ_zpravy' ])){
      if ( $_SESSION[ 'typ_zpravy' ] == ZPRAVA_OK ){
      return 'zprava_ok';
      }
      }
      return 'zprava_ko';
      } */

    private function osetri($x = null) {
        if (!isset($x))
            return null;
        elseif (is_string($x))
            return htmlspecialchars($x, ENT_QUOTES);
        elseif (is_array($x)) {
            foreach ($x as $k => $v) {
                $x[$k] = $this->osetri($v);
            }
            return $x;
        } else
            return $x;
    }

    /**
     * @brief Funkce, ktera kontroluje, zda soucasny uzivatel ma opravneni pro pristup.
     * 
     * @param type $pozice  pozice, ktera je potreba pro opravneny pristup
     */
    public function overUzivatele($pozice) {
        $spravceUzivatelu = new SpravceUzivatelu();
        $uzivatel = $spravceUzivatelu->vratUzivatele();
        if (!$uzivatel) {
            $this->pridejZpravu('Nedostatečné oprávnění.', ZPRAVA_ERROR);
            $this->presmeruj('login');
        }

        $opravneni = 0;
        foreach ($pozice as $p) {
            if ($uzivatel['pozice'] == $p) {
                $opravneni = 1;
            }
        }

        if (!$opravneni) {
            $this->pridejZpravu('Nedostatečné oprávnění.', ZPRAVA_ERROR);
            $this->presmeruj('login');
        }
    }

}

?>
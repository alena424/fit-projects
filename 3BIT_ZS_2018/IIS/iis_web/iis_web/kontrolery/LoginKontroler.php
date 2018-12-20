<?php
/**
 * @project IIS - Restaurace
 * 
 * @author Jan Sorm (xsormj00), Alena Tesarova (xtesar36)
 * @file LoginKontroler.php
 * @brief Kontroler pro zpracovani argumentu pri prihlasovani
 */
class LoginKontroler extends Kontroler {

    public function zpracuj($parametry) {

        // pokud uz je prihlaseny, tak ho presmerujeme na info o jeho uctu
        $spravceUzivatelu = new SpravceUzivatelu();
        if ($spravceUzivatelu->vratUzivatele()){
            $this->presmeruj('muj-ucet');
        }    
            

        // hlavicka
        $this->hlavicka['titulek'] = 'Přihlášení';
        // zkusime se prihlasit
        if ( $_POST ) {
            try {
                $spravceUzivatelu->prihlas( $_POST['login'], $_POST['heslo'] );
                $this->pridejZpravu( 'Byl jste úspěšně přihlášen.', ZPRAVA_OK );
                $this->presmeruj('muj-ucet');
            } catch (ChybaUzivatele $chyba) {
                // chyba nastala
                $this->pridejZpravu($chyba->getMessage(), ZPRAVA_CHYBA);
            }
        }
        $this->pohled = 'prihlaseni' ;
    }
    

}

?>
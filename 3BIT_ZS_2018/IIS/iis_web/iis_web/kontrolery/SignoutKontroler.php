<?php

class SignoutKontroler extends Kontroler{
    
    public function zpracuj($parametry) {
        $spravce = new SpravceUzivatelu();
        $spravce->odhlas();
        $this->pridejZpravu('Byl jste úspěšně odhlášen', ZPRAVA_OK);
        $this->presmeruj('restaurace/hlavni_strana');
       
    }

}


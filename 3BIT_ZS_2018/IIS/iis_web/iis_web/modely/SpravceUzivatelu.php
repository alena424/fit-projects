<?php
/**
 * Trida umoznuje spravovat uzivatele
 */
class SpravceUzivatelu
{

        // tady by se melo nejake heslo vygenerovat
        public function vratHeslo($heslo)
        {         
                
                $pass = password_hash($heslo, PASSWORD_DEFAULT);
                return $pass;
        }

        // registruj noveho uzivatele
        // tuto funci bude delat pouze majitel
        // hesla se vygeneruje -> posle se email uzivateli s heslem
        public function registruj($jmeno, $prijmeni, $rc, $adresa, $pozice, $email)
        {
                $uzivatel = array(
                        'jmeno' => $jmeno,
                       // 'heslo' => $this->vratHeslo(121),
                        'email' => $email,
                        'adresa' => $adresa,
                        'pozice' => $pozice,
                        'prijmeni' => $prijmeni,
                );
                try
                {
                        Db::vloz('uzivatele', $uzivatel);
                }
                catch (PDOException $chyba)
                {
                        throw new ChybaUzivatele('Uživatel s tímto jménem je již zaregistrovaný.');
                }
                // tady by se pak posilal email :)
        }

        // prihlasi se uzivatel do systemu
        public function prihlas( $login, $heslo )
        {
            // vyhledani uzivatele
                $uzivatel = Db::selectRow('
                    SELECT z.id_zam, z.jmeno, z.prijmeni, z.pozice, z.heslo, z.login, p.nazev
                    FROM zamestnanec z, pozice p
                    WHERE z.login = ?
                        AND p.id_pozice = z.pozice
                    ',
                    array( $login )
                );
               
                if ( ! $uzivatel || ! password_verify( $heslo, $uzivatel[ 'heslo' ] ) ) {
                    throw new ChybaUzivatele( 'Neplatné jméno nebo heslo.' );
                }
                
                // ulozime si to sessny
                $_SESSION['uzivatel'] = array(
                    'id_zam' => $uzivatel[ 'id_zam' ],
                    'login' => $uzivatel[ 'login' ],
                    'heslo' => $uzivatel[ 'heslo' ],
                    'jmeno' => $uzivatel[ 'jmeno' ],
                    'prijmeni' => $uzivatel[ 'prijmeni' ],
                    'pozice_id' => $uzivatel[ 'pozice' ],
                    'pozice' => $uzivatel[ 'nazev' ]
                );
        }

        // odlaseni uzivatele
        public function odhlas()
        {
                unset($_SESSION['uzivatel']);
        }

        // zjisti uzivatele
        public function vratUzivatele( $id = null )
        {
                if ( !$id && isset($_SESSION['uzivatel']) ) {
                        return $_SESSION['uzivatel'];
                } elseif ( $id ) {
                    return Db::selectRow('
                        SELECT *
                        FROM zamestnanec, pozice
                        WHERE zamestnanec.pozice = pozice.id_pozice
                            AND id_zam = ?
                            AND zruseny = 0',
                        array( $id )
                    );
                }
                return null;
        }
        
        /**
         * Funkce vrati vsechny uzivatele splnujici zadana kriteria.
         * 
         * @param string $prijmeni
         * @param int $pozice
         * @return array
         */
        public function vratVsechnyUzivatele( $prijmeni = null, $pozice = null )
        {
            $parametry = array();
            if ( $prijmeni ) {
                //$podminka = " WHERE z.prijmeni = ?";
                $podminka .= " AND z.prijmeni like ?"; 
                array_push( $parametry, '%' . $prijmeni . '%' );
            }
            
            if ( $pozice ) {
                /*if ( ! $podminka ) {
                    $podminka .= " WHERE z.pozice = ?"; 
                } else {
                    $podminka .= " AND z.pozice = ?"; 
                }*/
                $podminka .= " AND z.pozice = ?"; 
                array_push( $parametry, $pozice );
            }
            
            return Db::select('
                SELECT *
                FROM zamestnanec z
                WHERE z.zruseny = 0' .
                $podminka,
                $parametry
            );
        }
        
        // vrati prava uzivatele apodle toho budeme zobrazovat nabidku v navbaru
        // 1 - muze vse
        // 2 - zamestnanec
        // 3 - externista
        public function vratUzivatelePrava()
        {
            if (isset($_SESSION['uzivatel'])) {
                        return $_SESSION[ 'uzivatel' ][ 'pozice_id' ];
            } else {
                return 0;
            }
        }
        
        public function vratIdNazevPozice()
        {
            $pozice = Db::select( '
                SELECT id_pozice, nazev FROM pozice'
            );
            
            $vysledek = array();
            foreach ( $pozice as $radek ) {
                $vysledek[ $radek[ 'id_pozice' ] ] = $radek[ 'nazev' ];
            }
            return $vysledek;
        }
        
        public function novyZamestnanec( $jmeno, $prijmeni, $rodne_cislo, $adresa, $pozice, $login, $heslo ) {
            if ( ! Db::insert(
                'zamestnanec',
                array(
                    'jmeno' => $jmeno,
                    'prijmeni' => $prijmeni,
                    'rodne_cislo' => $rodne_cislo,
                    'adresa' => $adresa,
                    'pozice' => $pozice,
                    'login' => $login,
                    'heslo' => $this->vratHeslo($heslo)
                )
                ) ) {

                return 1;
            }

            return 0;
        }
        
        public function zrusZamestnance( $id ) {
        /*    if ( ! Db::smaz( 'zamestnanec', 'WHERE id_zam = ?', array( $id ) ) ) {
                return 1;
            }
            return 0;*/
            
            if ( ! Db::update(
                'zamestnanec', array(
                    'zruseny' => 1
                ), 'WHERE id_zam = ?', array( $id ) ) ) {
                return 1;
            } else {
                return 0;
            }
        }
        
        public function editujZamestnance( $id, $jmeno, $prijmeni, $rodne_cislo, $adresa, $pozice, $login, $heslo = null ) {
            
            if ( $heslo ) {
                if ( ! Db::update(
                    'zamestnanec', array(
                        'jmeno' => $jmeno,
                        'prijmeni' => $prijmeni,
                        'rodne_cislo' => $rodne_cislo,
                        'adresa' => $adresa,
                        'pozice' => $pozice,
                        'login' => $login,
                        'heslo' => $this->vratHeslo($heslo),
                    ), 'WHERE id_zam = ?', array( $id ) ) ) {
                    return 1;
                } else {
                    return 0;
                }
            } else {
                if ( ! Db::update(
                    'zamestnanec', array(
                        'jmeno' => $jmeno,
                        'prijmeni' => $prijmeni,
                        'rodne_cislo' => $rodne_cislo,
                        'adresa' => $adresa,
                        'pozice' => $pozice,
                        'login' => $login,
                    ), 'WHERE id_zam = ?', array( $id ) ) ) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
        
        public function zmenHeslo( $heslo ) {
            
            if ( ! Db::update(
                'zamestnanec', array(
                    'heslo' => $this->vratHeslo($heslo),
                ), 'WHERE id_zam = ?', array( $_SESSION['uzivatel'][ 'id_zam' ] ) ) ) {
                return 1;
            } else {
                return 0;
            }
        }
}
?>
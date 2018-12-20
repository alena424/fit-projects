<?php

class Db {

    private static $connect;
    private static $settings = array(
        PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
        PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8",
        PDO::ATTR_EMULATE_PREPARES => false, # vkladani dotazu na datazi
    );

    /*
     * Funkce pro nastaveni spojeni s databazi.
     * 
     * @param $host
     * @param $user
     * @param $password
     * @param $database
     */
    public static function connect( $host, $user, $password, $database ) {
        if ( !isset( self::$connect ) ) {
            self::$connect = @new PDO(
                    "mysql:host=$host;dbname=$database", $user, $password, self::$settings
            );
        }
    }

    # vraci jeden radek

    /*
     * Funkce vrací první řádek výsledku dotazu.
     * @param $query    dotaz do databaze
     * @param $params   omezujici podminky
     */
    public static function selectRow( $query, $params = array() ) {
        $result = self::$connect->prepare( $query );
        try {
            $result->execute( $params );
        } catch ( PDOException $e ) {
            return FALSE;
        }
        return $result->fetch();
        
    }

    /*
     * Funkce vraci cely vysledek dotazu.
     * @param $query    dotaz do databaze
     * @param $params   omezujici podminky
     */
    public static function select( $query, $params = array() ) {
        $result = self::$connect->prepare( $query );
        try {
            $result->execute( $params );
        } catch ( PDOException $e ) {
            return FALSE;
        }
        return $result->fetchAll();
    }

    /*
     * Funkce vraci prvni hodnotu v prvnim radku vysledku dotazu.
     * @param $query    dotaz do databaze
     * @param $params   omezujici podminky
     */
    public static function selectResult( $query, $params = array() ) {        
        $result = self::selectRow( $query, $params );
        if ( $result ) {
            return $result[ 0 ];
        } else {
            return FALSE;
        }
    }

    // Spustí dotaz a vrátí počet ovlivněných řádků
    /*
    public static function dotaz($dotaz, $parametry = array()) {
        $navrat = self::$spojeni->prepare($dotaz);
        $navrat->execute($parametry);
        return $navrat->rowCount();
    }*/

    /*
     * Funkce pro upravu hodnoty v tabulce.
     * @param $table    tabulka, ve ktere se edituje
     * @param $values   hash upravovanych hodnot
     * @param $condition omezujici podminka
     * @param $params   pole hodnot do omezujici podminky
     */
    public static function update( $table, $values = array(), $condition, $params = array() ) {
        $uprav = self::$connect->prepare(
            "UPDATE `$table` SET `" .
            implode( '` = ?, `', array_keys( $values ) ) .
            "` = ? " . $condition
        );
        
        try {
            $uprav->execute( array_merge( array_values( $values ), $params ) );
        } catch ( PDOException $e ) {
            return FALSE;
        }
        return TRUE;
    }

    /*
     * Funkce pro vkladani hodnot do tabulky.
     * @param $table    tabulka, do ktere se vklada
     * @param $params   pole vkladanych hodnot
     */
    public static function insert( $table, $params = array() ) {
        $vloz = self::$connect->prepare(
            "INSERT INTO `$table` (`" .
            implode( '`, `', array_keys( $params ) ) .
            "`) VALUES (" . str_repeat( '?,', sizeOf( $params ) - 1 ) . "?)"
        );
        //print_r('execute ');
        
        try {
            $vloz->execute( array_values( $params ) );
        } catch ( PDOException $e ) {
            return FALSE;
        }
        return TRUE;
    }
    
    /*
     * Funkce pro smazani hodnot v tabulce.
     * @param $table    tabulka, do ktere se vklada
     * @param $condition omezujici podminka
     * @param $params   pole hodnot do omezujici podminky
     */
    public static function smaz( $table, $condition, $params = array() ) {
        $smaz = self::$connect->prepare(
            "DELETE FROM `$table` " .
            $condition
        );
        
        try {
            $smaz->execute( array_values( $params ) );
        } catch ( PDOException $e ) {
            return FALSE;
        }
        return TRUE;
    }
    
    public static function zacniTransakci() {
        self::$connect->beginTransaction();
       // print_r("begin transaction ");
    }
    
    public static function potvrd() {
        self::$connect->commit();
         //print_r("commit");
    }
    
    public static function vratZpet() {
        self::$connect->rollBack();
         //print_r("rollback");
    }

    public static function getLastId() {
        return self::$connect->lastInsertId();
    }

}

?>
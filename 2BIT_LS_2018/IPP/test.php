<?php

class Tests{
    private $directory;
    private $parse;
    private $interpret;
    private $recursive;
    private $html_out;
    
    public function __construct( $directory, $parse, $interpret, $recursive ){
        $this->directory = realpath( $directory );
        $this->parse = $parse;
        $this ->interpret = $interpret;
        $this->recursive = $recursive;
        $this->html_out = "<!DOCTYPE html>\n<html>\n <head>\n<title>TESTS</title>\n </head>\n  <body>\n   <h1>TESTS</h1>\n";
    }
    
    public function get_tests(){
        $recursive_dir = array();
        # pushneme  adresar do pole nezpracovanych adresaru
        array_push( $recursive_dir, $this->directory );
        
        while ( ! empty ( $recursive_dir ) ) {
            # zpracuju aktualni adresar a delam testy
            $this->directory = array_pop( $recursive_dir );
            $dir_files = scandir( $this->directory );
            $src = preg_grep( "/^.+\.src/", $dir_files );
            $this->get_tests_src( $src );
            
            # pridam vsechny dalsi adresare do pole
            if ( $this->recursive ) {
                    foreach ( $dir_files as $file ) {
                    if ( $file != '.' and $file != '..' and is_dir( $this->directory .'/'. $file )  ) {
                        array_push( $recursive_dir, realpath( $this->directory .'/'. $file ) );
                    }
                }
            }       
        }  
    }
    
    # funkce dostane .src soubory v adresari $this->directory
    public function get_tests_src( $src ){
        
        foreach ( $src as $entry ) {
            $xmlfile = tempnam( $this->directory, "xml_parse_" );
            $outfile = tempnam( $this->directory, "out_int" );
            $rc_out = tempnam( $this->directory, "rc_out" );
           
            preg_match( "/^(.+)\.src/", $entry, $matches );
            #echo "\n DIR: ".$this->directory. '/' . $matches[0]  ."\n";
            $out_xml = exec ( "php ". $this->parse . " < " . $this->directory .'/'.$entry . " > ".  $xmlfile, $output, $exit_code);
            
            
            # existuje .rc soubor?
            $this->_createFile( $rc_out, $exit_code );
            $rc_name = $this->directory .'/'.$matches[1] .'.rc';
            if ( file_exists( $rc_name ) == FALSE ){
                #vytvorime
                $this->_createFile( $rc_name, 0 );
            }
            # existuje .out soubor?
            $out_name = $this->directory .'/'.$matches[1] .'.out';
            if ( file_exists( $out_name ) == FALSE ){
                #vytvorime
                $this->_createFile( $out_name, '' );
            }
            
            # existuje .in soubor?
            $in_name = $this->directory .'/'.$matches[1] .'.in';
            if ( file_exists( $in_name ) == FALSE ){
                #vytvorime
                $this->_createFile( $in_name, '' );
            }
            
            ########
            # DIFF #
            ########
            $this->html_out .= "   <h2>".$this->directory .'/'.$entry."</h2>\n";
            if ( $exit_code == 0 ) {
                # bez chyby, pustime interpret
                # musim dat na stdin soubor s stdin
                $out_xml = exec ( "python3 ". $this->interpret . " < " . $xmlfile . " > ".$outfile , $output, $exit_code);
        
            }
            # porovname vystupy
            #echo "diff ".  $outfile. " ".$out_name;
            $differences_out = exec( "diff ".  $outfile. " ".$out_name  );
            if ( empty ( $differences_out ) ){
                # out je spravne
                $this->html_out .= "    <p>Output si correct</p>\n";
            } else {
                 $this->html_out .= "    <p>Output si not correct</p>\n";
                 $this->html_out .= "    Difference diff: ".htmlentities( $differences_out ) ."\n";
            }
                
            # porovname chybovy vystup
            #echo "diff ".  $rc_out . " ".$rc_name;
            $differences_rc = exec( "diff ".  $rc_out . " ".$rc_name  );
            if ( empty ( $differences_rc ) ){
                    # out je spravne
                $this->html_out .= "    <p>Stderr si correct</p>\n";
            } else {
                 $this->html_out .= "    <p>Stderr si not correct</p>\n";
                 $this->html_out .= "    Diff: ".htmlentities( $differences_out ) ."\n";
            }
                 
            # odstranime docasne soubory
            unlink( $xmlfile  );
            unlink( $outfile  );
            unlink( $rc_out  );
            $this->html_out .= "   <br>\n";
        }
    }
    # funkce vytvori slozku a naplni ji obsahem contain
    private function _createFile( $name, $contain ){
        $handler = fopen( $name, 'w+' );
        fwrite( $handler, $contain );
        fwrite( $handler, PHP_EOL );
        fclose( $handler );
    }
    public function getHtml(){
        $this->html_out .= "  </body>\n</html>\n";
        return $this->html_out;
    }
}

class Help{
    public function __construct(){
        echo "NAPOVEDA TODO";
    }   
}


function main( $argv ) {
    $shortopts = "";
    
    $longopts  = array(
        "help",
        "directory:",
        "parse-script:",
        "int-script:",
        "recursive",                  
    );
    
    $options = getopt($shortopts, $longopts);
    
    #kontrola parametru
    
    # aktualni adrsar
    $directory = ".";
    #implicitni soubor pro parser
    $parse = "parse.php";
    # implicitni coubor pro interpret
    $interpret = "interpet.py";
    # implicitni neni rekurzivne
    $recursive = 0;
    if ( ! empty ( $options ) ) {
        if ( isset( $options[ "help" ]  ) and $options[ "help" ] == false ) {
            $help = new Help();
            exit( 0 );
        }
        if ( isset( $options[ "directory" ] ) and is_dir( $options[ "directory" ] ) ) {
          
            $directory = $options[ "directory" ] ;
        }
        
        if ( isset( $options[ "recursive" ] ) ) {
            $recursive = 1;
        }
        
        # jake obsahuje directory slozky?
        $dir_files = scandir( $directory );
        
        if ( isset(  $options[ "parse-script" ] ) ){
            
            if ( file_exists( $options[ "parse-script" ] ) ) {
                $parse = $options[ "parse-script" ] ;
            } else {
                fwrite(STDERR, "Nepodarilo se najit soubor ".$options[ 'parse-script' ]."  v tomto adresari: $directory");
                # neexistuje
                exit( 11 );
            }
            
        }
        if ( isset(  $options[ "int-script" ] ) ){
             if ( file_exists( $options[ "int-script" ] ) ) {
                $parse = $options[ "int-script" ];
            } else {
                fwrite(STDERR, "Nepodarilo se najit soubor ".$options[ 'int-script' ]." v tomto adresari: $directory");
                # neexistuje
                exit( 11 );
            }
        }
    }
    #echo "Directory: $directory\n";
    #echo "Parse: $parse\n";
    #echo "Interpet: $interpret\n";
    #echo "Recursive: $recursive\n";
    
    $tests = new Tests( $directory, $parse, $interpret, $recursive );
    
    # provedou se testy
    $tests->get_tests();
    echo $tests->getHtml();
}
main( $argv );
?>
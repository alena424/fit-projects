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
        $this->interpret = $interpret;
        $this->recursive = $recursive;
        $this->html_out = "<!DOCTYPE html>\n<html>\n <head>\n<title>TESTS</title>\n </head>
        <style>
        .test{ background:#f9f9f9; width: 1000px; margin-bottom: 20px}
        .correct{
            color:#5cb85c;
          }
        .error{
            color:#d9534f;
           }
            </style>
        <body>\n
        <h1>TESTS</h1>\n";
    }
    
    
    public function get_tests(){
        $recursive_dir = array();
        # pushneme  adresar do pole nezpracovanych adresaru
        array_push( $recursive_dir, $this->directory );
        #echo $this->directory ;
        while ( ! empty ( $recursive_dir ) ) {
            # zpracuju aktualni adresar a delam testy
            $this->directory = array_pop( $recursive_dir );
            $dir_files = scandir( $this->directory );
            #print_r( $dir_files);
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
        $counter = 1;
        foreach ( $src as $entry ) {
            $this->html_out .= "<div class='test'>";
            $this->html_out .= "   <b>". $counter . ". " . $this->directory .'/'.$entry."<b>\n";
            # temporary slozky
            $xmlfile_tmp = tempnam( $this->directory, "xml_parse_" );
            $outfile_tmp = tempnam( $this->directory, "out_int" );
            $rc_out_tmp = tempnam( $this->directory, "rc_out" );
           
            preg_match( "/^(.+)\.src/", $entry, $matches );
            $exit_code = 0;
            #echo "\n DIR: ".$this->directory. '/' . $matches[0]  ."\n";
            #echo  ("php ". $this->parse . " < " . $this->directory .'/'.$entry . " > ".  $xmlfile_tmp);
            $out_xml = exec ( "php ". $this->parse . " < " . $this->directory .'/'.$entry . " > ".  $xmlfile_tmp , $output, $exit_code);
            
            # existuje .rc soubor?
            #echo $entry . " " . $exit_code;
            #echo "\n";
            #$this->_createFile( $rc_out_tmp, 0 );
            $rc_name = $this->directory .'/'.$matches[1] .'.rc';
            if ( file_exists( $rc_name ) == FALSE ){
                #vytvorime
                $this->_createFile( $rc_name, 0 );
            }
            # existuje .out soubor?
            $out_name = $this->directory .'/'.$matches[1] .'.out';
            #echo "DIR: " .$this->directory;
            #echo "match: " . $matches[1];
            
            if ( file_exists( $out_name ) == FALSE ){
                #vytvorime
                //echo $out_name;
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
            
            $counter++;
            ##################
            # INTERPRET, OUT #
            ##################
            if ( $exit_code == 0 ) {
                # bez chyby, pustime interpret a porovname
                # musim dat na stdin soubor s stdin
                $interpret = $this->interpret;
                $out_xml = exec ( "python3 ". $interpret. " --source=" . $xmlfile_tmp . " > ". $outfile_tmp . "< $in_name", $output, $exit_code);
                #var_dump( $output);
        
                # porovname vystupy
                # tady pak bude out[utfile]
                $differences_out = exec( "diff -u ".  $outfile_tmp. " ".$out_name  );
                if ( empty ( $differences_out ) ){
                    # out je spravne
                    $this->html_out .= "    <div class='correct'> Output si correct</div>";
                } else {
                     $this->html_out .= "    <div class='error'> Output si not correct</div>\n";
                     $this->html_out .= "    <div class='diff' >Difference diff: ".$differences_out  ."</div>\n";
                }
            } 
            
            ##################    
            # CHYBOVY VYSTUP #
            ##################
            
            $differences_rc = exec( "diff ".  $rc_out_tmp . " ".$rc_name  );
            $rc_file = fopen( $rc_name, 'r+b' );
            $code = fgets( $rc_file );
          
            if ( $code == $exit_code ){
                    # out je spravne
                $this->html_out .= "    <div class='correct'> Stderr si correct<br>Return val is: $exit_code</div>";
            } else {
                 $this->html_out .= "   <div class='error'> Stderr si not correct <br>";
                 $this->html_out .=  "Your return value is: $exit_code <br>Correct return value is: $code </div>";
                 $this->html_out .= "  <div class='diff' >Difference diff: ".htmlentities( $differences_out ) ."</div>\n";
            }
                
            
            
            
                 
            # odstranime docasne soubory
            unlink( $xmlfile_tmp  );
            unlink( $outfile_tmp  );
            unlink( $rc_out_tmp  );
            
            $this->html_out .= "</div>";
            #$this->html_out .= "   <br>\n";
        }
    }
    # funkce vytvori slozku a naplni ji obsahem contain
    private function _createFile( $name, $contain ){
        $handler = fopen( $name, 'w+b' );
        fwrite( $handler, $contain );
        //fwrite( $handler, PHP_EOL );
        fclose( $handler );
    }
    public function getHtml(){
        $this->html_out .= "  </body>\n</html>\n";
        return $this->html_out;
    }
}

class Help{
    public function __construct(){
        echo "php test.php
                --help               : vypise napovedu
                --directory=path     : testy hleda v zadanem adresari (implicitne aktualni adresar)
                --recursive          : testy bude hledat nejen v zadanem adresari, ale i podadresarich
                --parse-script=file  : soubor se scriptem v PHP 5.6 pro analyzu zdrojoveho kodu (implicitne parse.php)
                --int-scipt=file     : soubor se scriptem v Python 3.6 pro interpret XML reprezentace kodu v IPPcode18 (implicitne interpret.py)\n";
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
    $interpret = "interpret.py";
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
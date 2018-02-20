<?php
setlocale(LC_ALL, 'cs_CZ.UTF-8');

# vyrobim si asociativni pole
$instruction_map = array(
            "ADD"=> "A1", "SUB" => "A1", "MUL" => "A1", "IDIV" => "A1", "LT" => "A1", "GT" => "A1", "EQ" => "A1", "AND" => "A1",
            "OR" => "A1", "STRI2INT" => "A1", "GETCHAR" => "A1", "SETCHAR" => "A1","CONCAT" => "A1",
            
            "JUMPIFEQ" => "A2", "JUMPIFNEQ" => "A2",
            
            "INT2CHAR" => "B1", "STRLEN" => "B1", "TYPE" => "B1", "MOVE" => "B1","NOT" => "B1",
            
            "READ" => "B2",
            
            "DEFVAR" => "C1", "POPS" => "C1",
            
            "PUSHS" => "C2", "WRITE" => "C2", "DPRINT" => "C2",
            
            "CALL" => "C3", "LABEL" => "C3", "JUMP" => "C3",
            
            "BREAK" => "D", "CREATEFRAME" => "D", "PUSHFRAME" => "D", "POPFRAME" => "D", "RETURN" => "D",
    );

##################################
# REGEX pro SYNTAKTICKOU ANALYZU #
##################################

 # isntrukce stejny zapis:
    # a) 1. ADD, SUB, MUL, IDIV, LT, GT, EQ, AND, OR, STRI2INT, GETCHAR, SETCHAR, CONCAT (3 operandy) <var> <symm1> <symb2> ^
    #    2. JUMPIFEQ, JUMPIFNEQ, <label> <symm1> <symb2>
    #
    # b) 1. INT2CHAR, STRLEN, TYPE, MOVE, NOT <var> <symb> 
    #    2. READ, <var> <type>  (2 operandy)
    #
    # c) 1. DEFVAR, POPS (1 operand) <var>
    #    2. PUSHS, WRITE, DPRINT <symb>
    #    3. CALL, LABEL, JUMP <label>
    #
    # d) BREAK, CREATEFRAME, PUSHFRAME, POPFRAME, RETURN (0 operandu)
    
    # REGEXY
    # promenna - (\s)+(GF|LF|TF)@(\w|%|\*|&|$|-)+(\s)+
    # konstanta2 - ((string@([^\\\\\s]|(\\\\[0-9]{3}))*)|(int@((\+|-)?\d+|))|bool@(false|true))
    # komentar - (#.*)*
    # type - (string|int|bool)
    # label - (\w|%|\*|&|$|-)+ (jako nazev promene)
    
$type_reg = "(string|int|bool)";
$label_reg = "\D(\w|%|\*|&|\\$|-|)+";
$variable_reg = "(GF|LF|TF)@($label_reg)";
$constant_reg = '(string@([^\\\\\s#]|(\\\\[0-9]{3}))*)|(int@((\+|-)?\d+|))|bool@(false|true)';
$comment_reg = "(#.*)*";
$symbol_reg = "(". $constant_reg ."|". $variable_reg .")";
$order = 1; # pocitadlo

# funkce proceed_symbol
# funkce zjisti, jestli se jedna o promennou nebo konstantu a ulozi do asociativniho pole, ktere vraci 
function proceed_symbol( $symbol ){
    
    global $variable_reg;
    $type_value_array_help = array();
    if ( preg_match( '/'.$variable_reg.'/', $symbol ) == TRUE ) {
        $type_value_array_help[ 'var' ] = $symbol;
    } else {
        # rozrezeme konstantu
        $cut_konstant = preg_split("/@/", $symbol, -1, PREG_SPLIT_NO_EMPTY );
        if ( isset( $cut_konstant[ 1 ] ) ) {
            # existuje neco uvnitr
            if ( $cut_konstant[ 0 ] == 'bool' ) {
                # bool ma byt malymi pismeny
                $cut_konstant[ 1 ] = strtolower( $cut_konstant[ 1 ] );
            }
             $type_value_array_help[ $cut_konstant[ 0 ] ] = $cut_konstant[ 1 ];
        } else {
             $type_value_array_help[ $cut_konstant[ 0 ] ] = '';
        }
       
    }
        return $type_value_array_help;  
}

# funkce instruction_control( nazev instrukce, cely radek s instrukci a operandy )
# funkce zkontroluje jednu instrukci a jeji operandy
function instruction_control ( $name, $row_code ) {
   
    global $instruction_map;
    $arg = 1;
    global $order;
    global $xw;
    global $variable_reg, $symbol_reg, $comment_reg, $label_reg, $type_reg;
    $instruction_type = $instruction_map[ $name ];
    
    $type_value_array = array();
    # rozezeme po mezerach, nechceme prazdne
    $cut_row = preg_split("/\s/", $row_code, -1, PREG_SPLIT_NO_EMPTY );
    
    # /u pro unicode - UTF
    # A1: VAR SYMB1 SYMB2
    $a1_reg = "/^\s*$name\s+". '('.$variable_reg . ')'."\s+" . '('. $symbol_reg . ')'."\s+" . $symbol_reg ."\s*$comment_reg$/u";
    
    # A2: LABEL SYMB1 SYMB2
    $a2_reg = "/^\s*$name\s+". '(' .$label_reg. ')'. "\s+" . '(' . $symbol_reg . ')'. "\s+" . '('. $symbol_reg . ')'. "\s*$comment_reg$/u";
    # B1: VAR SYMB
    $b1_reg = "/^\s*$name\s+". '(' . $variable_reg .')'."\s+" . '(' . $symbol_reg . ')'. "\s*$comment_reg$/u";
    
    # B2: VAR TYPE
    $b2_reg = "/^\s*$name\s+". '(' . $variable_reg . ')'. "\s+". $type_reg . "\s*". $comment_reg."$/u";
    # C1: VAR
    $c1_reg = "/^\s*$name\s+". '(' .$variable_reg . ')'."\s*".$comment_reg."$/u";
    
    # C2: SYMB
    $c2_reg = "/^\s*$name\s+". '(' .$symbol_reg . ')'."\s*".$comment_reg."$/u";
    
    # C3: LABEL
    $c3_reg = "/^\s*$name\s+". '('.$label_reg . ")\s*".$comment_reg."$/u";
    
    # D:
    $d_reg = "/^\s*$name\s*".$comment_reg."$/u";
    
    switch ( $instruction_type ) {
        case "A1":
            # naplnime $type_value_array
            #echo ( $a1_reg .' '. $row_code );
            # aplikujeme regular
            $return_val = preg_match( $a1_reg, $row_code );
            if ( $return_val == FALSE ) {
                return FALSE;
            }
            # prvni je promenna
            $type_value_array[ 'var' ] = $cut_row[ 1 ];
            # druhy a treti je symbol -> konstnata nebo promenna?
            $symbol_array = proceed_symbol( $cut_row[ 2 ] ) ;
             foreach ( $symbol_array as $index => $value ) {
                $type_value_array[ $index ] = $value;
             }
             $symbol_array = proceed_symbol( $cut_row[ 3 ] ) ;
             foreach ( $symbol_array as $index => $value ) {
                $type_value_array[ $index ] = $value;
             }
            break;
        case "A2":
            $return_val = preg_match( $a2_reg, $row_code );
            if ( $return_val == FALSE ) {
                return FALSE;
            }
            
             $type_value_array[ 'label' ] = $cut_row[ 1 ];
             
            $symbol_array = proceed_symbol( $cut_row[ 2 ] ) ;
             foreach ( $symbol_array as $index => $value ) {
                $type_value_array[ $index ] = $value;
             }
             $symbol_array = proceed_symbol( $cut_row[ 3 ] ) ;
             foreach ( $symbol_array as $index => $value ) {
                $type_value_array[ $index ] = $value;
             }
            break;
        case "B1":
            $return_val = preg_match( $b1_reg, $row_code );
            if ( $return_val == FALSE ) {
                return FALSE;
            }
            
             $type_value_array[ 'var' ] = $cut_row[ 1 ];
             # jedna se o konstantu nebo promennou (symbol)?
             $symbol_array = proceed_symbol( $cut_row[ 2 ] ) ;
             foreach ( $symbol_array as $index => $value ) {
                $type_value_array[ $index ] = $value;
             }    
            
            break;
        case "B2":
            $return_val = preg_match( $b2_reg, $row_code );
            
            if ( $return_val == FALSE ) {
                return FALSE;
            }
            
            $type_value_array[ 'var' ] = $cut_row[ 1 ];
            $type_value_array[ 'type' ] = $cut_row[ 2 ];
            break;
        case "C1":
            #echo ("$c1_reg $row_code");
            $return_val = preg_match( $c1_reg, $row_code );
            if ( $return_val == FALSE ) {
                return FALSE;
            }
            $type_value_array[ 'var' ] = $cut_row[ 1 ];
            break;
        case "C2":
          
            $return_val = preg_match( $c2_reg, $row_code );
            if ( $return_val == FALSE ) {
                return FALSE;
            }
            
            $symbol_array = proceed_symbol( $cut_row[ 1 ] ) ;
             foreach ( $symbol_array as $index => $value ) {
                $type_value_array[ $index ] = $value;
             }
            
            break;
        case "C3":
    
            $return_val = preg_match( $c3_reg, $row_code );
            if ( $return_val == FALSE ) {
                return FALSE;
            }
            $type_value_array[ 'label' ] = $cut_row[ 1 ];
            break;
        case "D":
            $return_val = preg_match( $d_reg, $row_code );
            break;
        default:
            $return_val = FALSE;
    }
    if ( $return_val == TRUE ) {
        # pokud proslo a je vse spravne, tvorime xml
        xmlwriter_start_element($xw, 'instruction');
        
        xmlwriter_start_attribute($xw, 'order');
        xmlwriter_text($xw, $order++);
        xmlwriter_end_attribute($xw);
        
        xmlwriter_start_attribute($xw, 'opcode');
        xmlwriter_text($xw, $name);
        xmlwriter_end_attribute($xw);
        
        foreach ( $type_value_array as $index => $value ) {
            xmlwriter_start_element($xw, 'arg' . $arg++);
            xmlwriter_start_attribute($xw, 'type' );
            xmlwriter_text($xw, $index );
            xmlwriter_end_attribute($xw);
            xmlwriter_text($xw, $value );
            xmlwriter_end_element($xw);
        }
        
        xmlwriter_end_element($xw);
    }

    return $return_val;
} 

########
# MAIN #
########

$statistics = 0;
$loc = 0;
$comments = 0;
# muze existovat pouze jeden nebo dva parametry a to napoveda anebo statistiky
if ( count ( $argv ) > 1 ) {
    ########
    # HELP #
    ########
    if ( isset( $argv[ 1 ] ) and $argv[ 1 ] == '--help' ) {
        echo( 
        "Nestrukturovaný imperativní jazyk IPPcode18 vznikl úpravou jazyka IFJcode17 (jazyk pro mezikód
        překladače jazyka IFJ17, viz [3]), který zahrnuje instrukce tříadresné (typicky se třemi argumenty)
        a zásobníkové (typicky méně parametrů a pracující s hodnotami na datovém zásobníku). Každá instrukce
        se skládá z operačního kódu (klíčové slovo s názvem instrukce), u kterého nezáleží na velikosti
        písmen (tj. case insensitive). Zbytek instrukcí tvoří operandy, u kterých na velikosti písmen záleží
        (tzv. case sensitive). Operandy oddělujeme libovolným nenulovým počtem mezer či tabulátorů. Také
        před operačním kódem a za posledním operandem se může vyskytnout libovolný počet mezer či tabulátorů.
        Odřádkování slouží pro oddělení jednotlivých instrukcí, takže na každém řádku je maximálně
        jedna instrukce a není povoleno jednu instrukci zapisovat na více řádků. Každý operand je tvořen
        proměnnou, konstantou, typem nebo návěštím. V IPPcode18 jsou podporovány jednořádkové komentáře
        začínající mřížkou (#). Kód v jazyce IPPcode18 začíná úvodním řádkem s tečkou následovanou
        jménem jazyka (nezáleží na velikosti písmen): .IPPcode18
        " );
        exit( 0 );
    }
    elseif ( preg_match( '/--stats=(\S+)/', $argv[ 1 ], $matches ) and count ($argv) <= 4 ) {
        #print_r( $matches );
        $file_stat = fopen( $matches[1], "w+" );
        if ( $file_stat == FALSE or $file_stat == 0 ) {
            fwrite(STDERR, "Nepodarilo se vytvorit soubor $matches[1]");
            exit( 11 );
        }
        if ( preg_match( '/(--comments)/', $argv[ 2 ] ) ) {
            $comments = 1;
            if ( isset($argv[ 3 ] ) and preg_match( '/(--loc)/', $argv[ 3 ] ) ){
                $loc = 2;
            }
        } elseif ( preg_match( '/(--loc)/', $argv[ 2 ] )  ) {
            $loc = 1;
            if ( isset($argv[ 3 ] ) and preg_match( '/(--comments)/', $argv[ 3 ] ) ) {
                $comments = 2;
            }
        } else {
            fwrite(STDERR, "Spatna kombinace parametru");
            exit( 10 ); 
        }
        # priznak, ze chceme statistiky
        $statistics = 1;
       
    } else {
        # nepovolena kombinace parametru
        fwrite(STDERR, "Nepovolena kombinace parametru");
        exit( 10 );
    }    
}

$file = fopen( 'php://stdin', "r" );

if ( $file == FALSE ) {
    fwrite(STDERR, "Nepodarilo se najit soubor");
    exit( 11 );
}
$xw = xmlwriter_open_memory();
xmlwriter_set_indent($xw, 1);
$res = xmlwriter_set_indent_string($xw, ' ');


xmlwriter_start_document($xw, '1.0', 'UTF-8');
xmlwriter_start_element($xw, 'program');
xmlwriter_start_attribute($xw, 'language');
xmlwriter_text($xw, 'ippcode18');
xmlwriter_end_attribute($xw);

# preskocime prazdne radky
while ( $row = fgets( $file ) and preg_match( '/^(\s)*$/', $row ) ) {};
#echo $row;
if ( $row ) {
     if ( preg_match( '/^(\s)*\.IPPcode18(\s)*(#.*)*$/', $row ) == FALSE ) {
        fwrite( STDERR, "Kod musi zacinat .IPPcode18\n" );
        exit( 21 );   
      }  
}
$number_of_comments = 0;
$empty_rows = 0;
$instructions = 0;
$number_row = 1;
while ( $row = fgets( $file ) ) {
    #echo ( $row );
     # potrebuju vzit prvni nazev instrukce
    $number_row++;
    $arr = explode(' ',trim( $row ) );
    $instr_name = $arr[0];
    $instr_name_upp = strtoupper( $arr[0] );
    $row = str_replace( $instr_name, $instr_name_upp, $row);
    
     $arr[ 0 ] = strtoupper( $arr[0] );
   
    if ( array_key_exists( $arr[ 0 ], $instruction_map ) ) {
        # zkontrolujeme regex
        # o jaky typ se jedna
        if ( instruction_control( $arr[ 0 ], $row ) ) {
            $instructions++;
        } else {
            fwrite( STDERR, "Operand spatne u instrukce $arr[0] na radku $number_row: $row \n" );
            # 21 - lexikalni nebo syntakticka chyba
            exit( 21 ); 
        }
        
    } else if ( preg_match( '/^(\s)*(#.*)+$/', $row ) ) {
        # zkontrolujeme jestli se nejedna o radek s komentarem
        $number_of_comments++;
    } else if ( preg_match( '/^(\s)*$/', $row ) ){
        # kontrola, jestli se jedna o prazdny radek
        $empty_rows++;
    } else {
        # lexikalni chyba
        fwrite( STDERR, "Neznama instrukce $arr[0]" );
        exit( 21 );
    }
}

##############
# STATISTIKY #
##############

if ( $statistics ) {
    if ( $comments == 1 ) {
        # chceme komentare
        fwrite( $file_stat, "$number_of_comments\n" );
    }
    elseif (  $loc == 1) {
        # pocet radku kodu
         fwrite( $file_stat, "$instructions\n" );
    }
    
    if ( $comments == 2 ) {
        # chceme komentare
        fwrite( $file_stat, "$number_of_comments\n" );
    }
    elseif (  $loc == 2 ) {
        # pocet radku kodu
         fwrite( $file_stat, "$instructions\n" );
    }
    fclose( $file_stat );
    
}

##############
# TVORBA XML #
##############

xmlwriter_end_document($xw); # konec program
echo xmlwriter_output_memory($xw);
fclose( $file );
?>
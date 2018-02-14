<?php

setlocale(LC_ALL, 'cs_CZ.UTF-8');

# vyrobim si asociativni pole
$instruction_map = array(
            "ADD"=> "A1", "SUB" => "A1", "MUL" => "A1", "IDIV" => "A1", "LT" => "A1", "GT" => "A1", "EQ" => "A1", "AMD" => "A1",
            "OR" => "A1","NOT" => "A1", "STRI2INT" => "A1", "GETCHAR" => "A1", "SETCHAR" => "A1","CONCAT" => "A1",
            
            "JUMPIFEQ" => "A2", "JUMPIFNEQ" => "A2",
            
            "INT2CHAR" => "B1", "STRLEN" => "B1", "TYPE" => "B1", "MOVE" => "B1",
            
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
    # a) 1. ADD, SUB, MUL, IDIV, LT, GT, EQ, AND, OR, NOT, STRI2INT, GETCHAR, SETCHAR, CONCAT (3 operandy) <var> <symm1> <symb2> ^
    #    2. JUMPIFEQ, JUMPIFNEQ, <label> <symm1> <symb2>
    #
    # b) 1. INT2CHAR, STRLEN, TYPE, MOVE <var> <symb> 
    #    2. READ, <var> <type>  (2 operandy)
    #
    # c) 1. DEFVAR, POPS (1 operand) <var>
    #    2. PUSHS, WRITE, DPRINT <symb>
    #    3. CALL, LABEL, JUMP <label>
    #
    # d) BREAK, CREATEFRAME, PUSHFRAME, POPFRAME, RETURN (0 operandu)
    
    # REGEXY
    # promenna - (\s)+(GF|LF|TF)@(\w|%|\*|&|$|-)+(\s)+
    # konstanta - (string|int|bool)@([^\\\s]|(\\[0-9]{3}))*
    # komentar - (#.*)*
    # type - (string|int|bool)
    # label - (\w|%|\*|&|$|-)+ (jako nazev promene)
    
$type_reg = "(string|int|bool)";
$label_reg = "\D(\w|%|\*|&|\\$|-)+";
$variable_reg = "(GF|LF|TF)@($label_reg)";
$constant_reg = $type_reg .'@([^'.'\\\\'. '\s]|(\\\\[0-9]{3}))*';
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
    $cut_row = preg_split("/\s/", $row_code, -1, PREG_SPLIT_NO_EMPTY );
    
    # A1: VAR SYMB1 SYMB2
    $a1_reg = "/^\s*$name\s+". '('.$variable_reg . ')'."\s+" . '('. $symbol_reg . ')'."\s+" . $symbol_reg ."\s*$comment_reg$/";
    
    # A2: LABEL SYMB1 SYMB2
    $a2_reg = "/^\s*$name\s+". '(' .$label_reg. ')'. "\s+" . '(' . $symbol_reg . ')'. "\s+" . '('. $symbol_reg . ')'. "\s*$comment_reg$/";
    # B1: VAR SYMB
    $b1_reg = "/^\s*$name\s+". '(' . $variable_reg .')'."\s+" . '(' . $symbol_reg . ')'. "\s*$comment_reg$/";
    
    # B2: VAR TYPE
    $b2_reg = "/^\s*$name\s+". '(' . $variable_reg . ')'. "\s+". $type_reg . "\s*". $comment_reg."$/";
    # C1: VAR
    $c1_reg = "/^\s*$name\s+". '(' .$variable_reg . ')'."\s*".$comment_reg."$/";
    
    # C2: SYMB
    $c2_reg = "/^\s*$name\s+". '(' .$symbol_reg . ')'."\s*".$comment_reg."$/";
    
    # C3: LABEL
    $c3_reg = "/^\s*$name\s+". '('.$label_reg . ")\s*".$comment_reg."$/";
    
    # D:
    $d_reg = "/^\s*$name\s*".$comment_reg."$/";
    
    switch ( $instruction_type ) {
        case "A1":
            # naplnime $type_value_array
            #echo ( $a1_reg .' '. $row_code );
            $return_val = preg_match( $a1_reg, $row_code );
            $type_value_array[ 'var' ] = $cut_row[ 1 ];
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
             $type_value_array[ 'var' ] = $cut_row[ 1 ];
             # jedna se o konstantu nebo promennou (symbol)?
             $symbol_array = proceed_symbol( $cut_row[ 2 ] ) ;
             foreach ( $symbol_array as $index => $value ) {
                $type_value_array[ $index ] = $value;
             }
             
            
            break;
        case "B2":
            $return_val = preg_match( $b2_reg, $row_code );
            $type_value_array[ 'var' ] = $cut_row[ 1 ];
            $type_value_array[ 'type' ] = $cut_row[ 2 ];
            break;
        case "C1":
            #echo ("$c1_reg $row_code");
            $return_val = preg_match( $c1_reg, $row_code );
            $type_value_array[ 'var' ] = $cut_row[ 1 ];
            break;
        case "C2":
          
            $return_val = preg_match( $c2_reg, $row_code );
            
            $symbol_array = proceed_symbol( $cut_row[ 1 ] ) ;
             foreach ( $symbol_array as $index => $value ) {
                $type_value_array[ $index ] = $value;
             }
            
            break;
        case "C3":
    
            $return_val = preg_match( $c3_reg, $row_code );
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

# budeme mit vstupni soubor
# otevreni $handle = fopen( "file.txt","r" );
# prochazeni while ($c = fgetc) { $c ... }
# existuji $argc - pocet parametru
# $argv[0] - prvni parametr

# VSTUP --source=file nebo --source="file" | --help

#if ( ! $argv[ 1 ] ) {
 #    fwrite( STDERR, "Pokud si prejete zobrazit napovedu, pridejte argument --help");
  #   exit( 0 );
#}
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

# chceme dostat nas soubor z regexu - interpret.py
#preg_match( '/(--source=)((\S)+)/', $argv[ 1 ], $matches );
#print_r( $matches );

$file = fopen( 'php://stdin', "r" );
# nezopomenout fclose()

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
while ( $row = fgets( $file ) and preg_match( '/^(\s)*$/', $row ) ) { echo "radek"; };
#echo $row;
if ( $row ) {
     if ( preg_match( '/^(\s)*\.IPPcode18(\s)*(#.*)*$/', $row ) == FALSE ) {
        fwrite( STDERR, "Kod musi zacinat .IPPcode18" );
        exit( 21 );   
      }  
}
$komentar = 0;
$prazdne_radky = 0;
while ( $row = fgets( $file ) ) {
    #echo ( $row );
     # potrebuju vzit prvni nazev instrukce
    $arr = explode(' ',trim( $row ) );
    $instr_name = $arr[0];
    $instr_name_upp = strtoupper( $arr[0] );
    $row = str_replace( $instr_name, $instr_name_upp, $row);
    echo $row;
     $arr[ 0 ] = strtoupper( $arr[0] );
   
    
   
    if ( array_key_exists( $arr[ 0 ], $instruction_map ) ) {
        # zkontrolujeme regex
        # o jaky typ se jedna
        if ( instruction_control( $arr[ 0 ], $row ) ) {
            
        } else {
            fwrite( STDERR, "Operand spatne u instrukce $arr[0]" );
            # 21 - lexikalni nebo syntakticka chyba
            exit( 21 ); 
        }
        
    } else if ( preg_match( '/^(\s)*(#.*)+$/', $row ) ) {
        # zkontrolujeme jestli se nejedna o radek s komentarem
        $komentar++;
    } else if ( preg_match( '/^(\s)*$/', $row ) ){
        # kontrola, jestli se jedna o prazdny radek
        $prazdne_radky++;
    } else {
        # lexikalni chyba
        fwrite( STDERR, "Neznama instrukce $arr[0]" );
        exit( 21 );
    }
}

#echo ( "Pocet prazdnych radku: $prazdne_radky \n" );
#echo ( "Pocet komentaru: $komentar \n" );

##############
# TVORBA XML #
##############

xmlwriter_end_document($xw); # konec program
echo xmlwriter_output_memory($xw);
fclose( $file );

    
# FRAME
# mam vse v temporary frame, zavolam pushframe, vsechno se mi da do local frame
   
  
    

?>
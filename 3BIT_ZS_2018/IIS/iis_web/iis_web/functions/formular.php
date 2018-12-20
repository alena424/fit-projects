<?php

include 'functions/iis.php';

function tableItem($data, $info, &$form, $key) {

    // pridame styl inputu
    $class = "my_input_form";
    $force = "";
    $req = '';
    if ($data[$key]['force']) {
        $class .= " force";
        $req = 'required';
    }
    //type text
    $type_text = '';
    $readonly = '';

    if ($data[$key]['readonly']) {
        $readonly = 'readonly';
    }
    $poc = 0;
    if ($data[$key]['type'] == 'datum') {
        // tady by to chtelo doplnit neco pro typ datum
    } else if ($data[$key]['type'] == 'popup') {
        // mame nejaky popup k dispozici
        if (isset($data[$key]['popup'])) {
            //echo "<div class='custaGroupContainer' >";
            if ($data[$key]['multiple']) {
                if ($data[$key]['value']) {
                    foreach ($data[$key]['value'] as $value) {
                        // naplnime
                        //$send_val = ''; // udelana hodnota v popupu
                        $val_amount = 1;
                        if (!is_array($value)) {
                            // value by melo byt pole pro kompatibilitu
                            $value = array($value);
                        } else if (isset($value['amount'])) {
                            $val_amount = $value['amount'];
                        }
                       //print_r($val_amount);
                        $form .= popup_menu($data[$key]['popup'], $value, $data[$key]['force'], $key, 0, 1, $data[$key]['amount'], $val_amount, $data[$key]['addition'], $info['in_uisbox'], $data[$key]['child_active'], 0);
                        $poc++;
                        //echo '<hr>';
                    }
                } else {
                    // nesjou zadne preddefinovane hodnoty
                    //print_r($data[$key]['child_active']);
                    $form .= popup_menu($data[$key]['popup'], array(), $data[$key]['force'], $key, 0, 1, $data[$key]['amount'], 1, $data[$key]['addition'], $info['in_uisbox'], $data[$key]['child_active'], 0);
                }
            } else {
                // pridame obycejny popup
                $form .= popup_menu($data[$key]['popup'], array($data[$key]['value']), $data[$key]['force'], $key, 0, 0);
            }
        } else {
            echo 'Expected popup! ' . $key;
            return 1;
        }
    } else {
        $type_text = $data[$key]['type'];
    }
    // pro popup jiny vypis
    if ($data[$key]['type'] == 'number') {
        // chceme pouze kladna cisla bez vyjimek
        $pattern = "min='1'";
    }
    if ($data[$key]['hidden']) {
        // chceme pouze kladna cisla bez vyjimek
        $pattern .= "style='display:none;'";
    }
    if ($data[$key]['type'] != 'popup') {
        $addition = '';

        if (isset($data[$key]['addition']) && $data[$key]['addition']) {
            $addition = $data[$key]['addition'];
        }

        $form .= "<input $readonly $req $pattern " . ' value="' . $data[$key]['value'] . '" type=' . $type_text . ' class=\'form-control ' . $class . '\' name=' . $key . ' placeholder=' . $data[$key]['place'] . "  >$addition";
        #$form .= "<div class='invalid-tooltip'> Vyplnte prosim toto pole</div>";
    }
}

/**
 * @function addFormular
 * @brief Funkce zobrazuje editacni formular podle parametru
 * 
 * example:
 *   $data[ 'login' ] = array( 'name' => 'Login', 'type' => 'text', 'force' => 1, place = 'honza', value => 'aa' );
 *   $data[ 'heslo' ] = array( 'name' => 'Heslo', 'type' => 'password', 'force' => 1 );   
 *   $info[ 'button_text' ] = array( values => "Přihlásit se", name => 'prihlasit' );
 * 
 * Parametry do data
 * name         =>  : string        - nazev
 * type         =>  : string        - typ [text, date, popup]
 * force        =>  : bool          - povinny/nepovinny
 * place        =>  : string        - placeholder
 * value        =>  : string/array  - predvyplnena hodnota, array pouze pokud je multiple = 1 -> cekame vice hodnot jinak string
 * popup        =>  : array         - obsah popup, pokud jsme dali type popup 
 * multiple     =>  : bool          - 1/0 pokud je umozneno zadat vice hodnot
 * readonly     =>  : bool          - 1/0 je pole readonly, pujde editovat
 * next_item    =>  : array         - dalsi prvek, ktery se zobrazi vedle (struktura jako data!), 'takovy pomocny ukazatel'
 * addition     =>  : string        - prida se za data, vhodne pro pridavani Kg, Cal atd. jednotek
 * amount       =>  : 1/0           - pridat za popup mnozstvi s inputem typu number
 * selected     =>  : (array( 'amount' => Int, 'item' => ID) | string ) - pole predvolenych hodnot, pro popup je to array, pro string je to string
 * hidden       =>  : bool 
 * 
 * Parametry do info
 * buttons      - array( array( 
 *                  name    => 'nazev tlacitka',
 *                  value   => 'hodnota v tlacitku' ),
 * 
 *                  array( -- pokud na kliknuti ma vyjet modularni box, kde si muzeme zadat title a body 
 *                  value => "Tisk účtenky", 
 *                  name => 'tisk_uctenky', 
 *                  modals => 1, 
 *                  'title' => 'ÚČTENKA', 
 *                  'body' => 'Účtenka byla vytisknuta.', 
 *                  'id' => 'example' )
 *  )
 * hiddens      - array( array( 
 *                  name    => 'nazev hiddenu',
 *                  value   => 'hodnota hiddenu' )
 *  )
 * cols         - pocet sloupcu
 * method       - [GET/POST]
 * no_form      - boolean, 1 pokud nechceme prifat <form>
 * vrat_kod     - BOOLEAN 1/0 defaultne 0
 */
function addFormular($data, $info = array()) {

    $expected_attr_text = array('name', 'type', 'place', 'value');
    $expected_attr_number = array('force', 'multiple', 'readonly', 'amount', 'hidden');
    $expected_info = array('button_text', 'cols', 'no_form');
    //finalni string na vystup
    $form = '';
    foreach ($data as $key => $value) {
        if (isset($data[$key])) {
            // projdeme, jestli obsahuje vsechny atributy, pokud ne doplnime defaultni hodnoty
            foreach ($expected_attr_text as $attr) {
                if (!isset($data[$key][$attr])) {
                    // neni, dame default
                    $data[$key][$attr] = "";
                }
            }

            foreach ($expected_attr_number as $attr) {
                if (!isset($data[$key][$attr])) {
                    // neni, dame default
                    $data[$key][$attr] = 0;
                }
            }
        }
    }
    // zadavame methosu posilani dat
    if (!isset($info['method'])) {
        $info['method'] = "GET";
    }
    // budeme defaultne davat formular
    if (!isset($info['no_form'])) {
        $info['no_form'] = 0;
    }
    if (!isset($info['vrat_kod'])) {
        $info['vrat_kod'] = 0;
    }

    foreach ($expected_info as $info_attr) {
        if (!isset($info[$info_attr]) && !isset($info['button_text'])) {
            // neni, dame default u tlacitka
            $info[$info_attr] = "Přidat";
        }
        // sloupce nemuzou byt 0
        if (!isset($info[$info_attr]) || !isset($info['cols']) || !$info['cols']) {
            // neni, dame default u tlacitka
            $info[$info_attr] = 1;
        }
    }


    // Vypsani tabulky
    if (!$info['no_form']) {
        $form .= "<form method='" . $info["method"] . "' class='form-horizontal needs-validation' novalidate>";
    }

    $sloupce = $info['cols'];

    // 2 je vzdy velikost labelu a ten zbytek dopocitame podle toho, kolik tech sloupcu bude
    $colsm = ( 12 / $sloupce ) - 2;
    $pocitadlo_radku = 1;
    foreach ($data as $key => $value) {
        $pattern = '';
        if ($data[$key]['hidden']) {
            // chceme pouze kladna cisla bez vyjimek
            $pattern .= "style='display:none;'";
        }
        if ($pocitadlo_radku % $sloupce == 0) {
            // chceme vice sloupcu
            $form .= "<div $pattern class='form-group'>";
        }


        // pridame nazev do formulare
        $dvojtecka = ': ';
        if ($data[$key]['dvojtecka'] == 2) {
            $dvojtecka = '';
        }
        $form .= "<label class=col-sm-2>" . $data[$key]['name'] . $dvojtecka . "</label>";

        $form .= "<div class='col-sm-$colsm custaGroupContainer'>";
        $save_data = $data;
        do {
            // pridame item
            tableItem($save_data, $info, $form, $key);
            //print_r($data[$key]['next_item']);
        } while (isset($save_data[$key]['next_item']) && is_array($save_data[$key]['next_item']) && $save_data = $data[$key]['next_item']);
        //} while ( 0 );
        $form .= "</div>";
        if ($pocitadlo_radku % $sloupce == 0) {
            $form .= "</div>";
        }
        $pocitadlo_radku++;
    }

    $form .= "<br>";
    if ($sloupce > 1) {

        $form .= "<br>";
    }


    if (isset($info['buttons']) && !empty($info['buttons'])) {

        foreach ($info['buttons'] as $value) {

            if ($value['modals']) {
                $form .= "<button data-toggle='modal' type='submit' data-target='#" . $value['id'] . "' value='" . $value['value'] . "' name=" . $value['name'] . " class='red_but_small'>" . $value['value'] . " </button>";
                modal_box(array('title' => $value['title'], 'body' => $value['body'], 'id' => $value['id']));
            } else {
                $form .= "<button type='submit' value='" . $value['value'] . "' name=" . $value['name'] . " class='red_but_small'>" . $value['value'] . " </button>";
                $form .= "&emsp;&emsp;";
            }
        }
    }
    if (isset($info['hiddens']) && !empty($info['hiddens'])) {

        foreach ($info['hiddens'] as $value) {
            $form .= "<input type='hidden' value='" . $value['value'] . "' name='" . $value['name'] . "' />";
            $form .= "&emsp;&emsp;";
        }
    }
    if (!$info['no_form']) {
        $form .= "</form>";
    }

    if (isset($info['vrat_kod']) && $info['vrat_kod']) {
        return $form;
    } else {
        echo $form;
    }
}

/**
 * @function zobrazFormular( ARRAY )
 * kazda polozka ma:
 *      name   - nazev    
 *      type   - typ      
 *      value - hodnota
 *      multiple - ocekavam pole ano/ne
 *      deliminer - pokud ocekavam pole, jak chci polozky rozdelit
 * Priklad:
 *  $formular['jmeno'] = array('name' => 'Jméno', 'type' => 'text', 'value' => $data['jmeno'] );
  $formular['telefon'] = array('name' => 'Telefon', 'type' => 'tel',  'value' => $data['telefon']);
  $formular['datum'] = array('name' => 'Datum', 'type' => 'datum',  'value' => $data['datum']);
  $formular['stoly'] = array('name' => 'Stoly', 'type' => 'text', 'multiple' => 1, 'deliminer' => ', ', 'value' => $data['stoly']);
  $formular['objednavky'] =  array('name' => 'Objednávky', 'multiple' => 1, 'delimiter' => '<br>','type' => 'text', 'value' => $data['objednavky']);
 */
function zobrazFormular($formular) {

    echo "<table>\n";

    foreach ($formular as $item) {
        // item je jedna polozka
        echo "<tr>\n";
        echo "<th>\n";
        echo $item['name'] . ': ';
        echo "</th>\n";

        echo "<th class='vysledek_th'>\n";
        if (isset($item['multiple']) && $item['multiple'] == 1) {
            echo implode($item['delimiter'], $item['value']);
        } else if (isset($item['multiple']) && $item['multiple'] == 2) {

            foreach ($item['value'] as $row) {
                echo "<tr>\n";
                foreach ($row as $val) {
                    echo "<th class='vysledek_th'>\n";
                    echo $val;
                    echo "</th>\n";
                }
                echo "</tr>\n";
            }
        } else {
            echo $item['value'];
        }

        echo "</th>\n";
        echo "</tr>\n";
    }
    echo "</table>\n";
}

/**
 * 
 * @param type $data : array( 
 * data             =>    : array( array() ), pr. array( ( 
 *                                              name => 'Jidlo', 
 *                                              id => 32, 
 *                                              align => [center], 
 *                                              type => [checkbox|text|number], - pokud chci editovat
 *                                              url => 'url po odstraneni', 
 *                                              checked => 1/0,
 *                                              name_identif => 'identif' - pokud upravujeme jmeno je ve tvaru '$name_identif-$id' 
 *                                              class => trida - trida se zatim prida pouze pokud type jine nez checkbox
 * ) 
 * headers          =>    : array( array() ), pr. array( ( name => 'Jidlo', id => 32, align => [center],  ) ) 
 * delete           =>    : boolean umozni pridat krizek nakonci
 * edit             =>    : boolean je mozne editovat tabulku
 * linebackground   =>    : boolean
 * vrat_kod         =>    : boolean defaultne 0
 * ))
 */
function upravTabulka($data = array()) {
    // kontrola parametru
    //print_r($data);
    if (!is_array($data) || empty($data) || !is_array($data['data']) || empty($data['data'])) {
        // nemame data -> konec
        echo 'data missing in formular';
        return '';
    }
    // sem budeme ukladat tabulku
    $tabulka = '<table>';
    // napred projdeme headers
    $tabulka .= "<tr class='headers'>";
    if (isset($data['headers'])) {

        $class_align = '';
        foreach ($data['headers'] as $zahlavi) {
            if (isset($zahlavi['align'])) {
                if ($zahlavi['align'] == 'center') {
                    $class_align = 'stred';
                } else {
                    $class_align = 'left';
                }
            }
            $tabulka .= "<th class='$class_align'>" . $zahlavi['name'] . '</th>';
        }
    }



    $ulozit = 0; // umoznime mazat hodnotu
    if (isset($data['save']) && $data['save']) {
        // umoznime smazat hodnotu
        if (isset($data['headers'])) {
            //print_r($data);
            $tabulka .= '<th class=stred> Uložit </th>';
            // pouze pokud mame 
        }

        $ulozit = 1;

        //pozor at je zadane id
    }
    $mazani = 0; // umoznime mazat hodnotu
    if (isset($data['delete']) && $data['delete']) {
        // umoznime smazat hodnotu
        if (isset($data['headers'])) {
            //print_r($data);
            $tabulka .= '<th class=stred> Odstranit </th>';
            // pouze pokud mame 
        }

        $mazani = 1;

        //pozor at je zadane id
    }
    $delete_upresneni = $data['delete_upresneni'];

    if (!isset($data['vrat_kod'])) {
        $data['vrat_kod'] = 0;
    }

    if (isset($data['headers'])) {
        $tabulka .= "<tr><td colspan=" . ( count($data['headers']) + $mazani + $ulozit ) . "> <hr></td></tr>";
    }

    $tabulka .= '</tr>';

    // projdeme samotna data
    $class_align = '';
    $id = 0;
    $url = '';
    if (isset($data['url'])) {
        $url = $data['url'];
    }
    if (isset($data['url'])) {
        $class_tr = $data['class_tr'];
    } else {
        $class_tr = '';
    }

    foreach ($data['data'] as $row) {

        $tabulka .= "<tr class='body_table $class_tr'>";
        foreach ($row as $value) {

            if (isset($value['align'])) {
                if ($value['align'] == 'center') {
                    $class_align = 'stred';
                } else {
                    $class_align = 'left';
                }
            }
            if (isset($data['in_uisbox']) && $data['in_uisbox']) {
                $class_align .= ' uisbox_radky_tabulka ';
            }
            if (isset($value['id'])) {
                $id = $value['id'];
            }
            if (isset($value['type'])) {
                // jine chovani
                if ($value['type'] == 'checkbox') {
                    $checked = '';
                    if (isset($value['checked']) && $value['checked']) {
                        $checked = 'checked';
                    }
                    //$data = array(name => $value['name'], value => $value['id'], checked => $checked);
                    //$tabulka .= "<td width='20px' class='" . $class_align . "'>" . checkbox($data) . '</td>';
                   $tabulka .= "<td  class='" . $class_align . "'><input class='" . $value['class'] . "' type='" . $value['type'] . "' $checked name='" . $value['name_identif'] . "' value=" . $value['name'] . " />" . $value['addition'] . "</td>";
                } else if ($value['type'] == 'ikona') {
                    if ($value['uisbox']) {
                        // $tabulka .= "<button data-toggle='modal'  data-target='#" . $id . "' value='" . 'name1' . "' name=" . $id . " class='red_but_small'>" . 'kuk' . " </button>";


                        $tabulka .= "<td class=stred> <a href='#myModal' data-toggle='modal' data-target='#" . $id . "' ><img data-toggle='tooltip' data-placement='top' title='" . $value['tooltip'] . "' src='" . $value['src'] . "' width='24px'> </a></td>";
                        modal_box(array('title' => $value['title'], 'body' => $value['body'], 'id' => $id));
                    } else {
                        $tabulka .= "<td class=stred> <a href='" . $value['href'] . "/$id" . "'><img data-toggle='tooltip' data-placement='top' title='" . $value['tooltip'] . "' src='" . $value['src'] . "' width='24px'> </a></td>";
                    }
                } else {
                    if ($value['type'] == 'number') {
                        // chceme pouze kladna cila bez vyjimek
                        $pattern = "min='1'";
                    }
                    $tabulka .= "<td  class='" . $class_align . "'><input $pattern class='my_input_form " . $value['class'] . "' type='" . $value['type'] . "' name='" . $value['name_identif'] . "' value='" . $value['name'] . "' />" . $value['addition'] . "</td>";
                }
            } else {
                $tabulka .= "<td class='" . $class_align . "'>" . $value['name'] . '</td>';
            }
            //echo $value['name'];
        }

        if ($ulozit) {
            if ($data['in_uisbox']) {
                $tabulka .= "<td class=stred> <a href='javascript:void(0);'><img data-toggle='tooltip' data-placement='top' title='Uložit objednávku' onclick='saveForm(this, 1);' src='img/saveping.png' width='20px'></a></td>";
            } else {
                $tabulka .= "<td class=stred> <a href='javascript:void(0);'><img data-toggle='tooltip' data-placement='top' title='Uložit objednávku' onclick='saveForm(this, 1);' src='img/baseline-save-24px.png' width='24px'></a></td>";
            }
        }

        if ($mazani) {
            if ($data['in_uisbox']) {
                $tabulka .= "<td class=stred> <a href='javascript:void(0);'> <img data-toggle='tooltip' data-placement='top' title='Odstranit'"
                        . "onclick='deleteRow(this, " . '"' . $url . '"' . ");' src='img/closeping.png' width='20px'></a></td>";
            } else {
                $tabulka .= "<td class=stred> <a href='javascript:void(0);'> <img data-toggle='tooltip' data-placement='top' title='Odstranit'"
                        . "onclick='deleteRow(this, " . '"' . $url . '"' . ");' src='img/close-white.png' width='30px'></a></td>";
            }
        }
        // pridame hiddenute id pro ukladani
        //print_r($data);
        if (isset($delete_upresneni)) {
            $id .= '-' . $value[$delete_upresneni];
        }

        $tabulka .= "<td class=neni_videt><input name='id' value=$id ></td>";
        if ($value['id_surovina']) {
            $tabulka .= "<td class=neni_videt><input name='id_surovina' value='" . $value['id_surovina'] . "' ></td>";
            $id = $value['id_surovina'];
            
        } 
        
        if ( isset( $data['identif_id'] ) && $data['identif_id']) {
            $id = $id . '-' . $data['identif_id'];
        }
        

        $tabulka .= "<td class=stred> <img class=neni_videt id='$id' src='img/yes.png' width='35px'></td>";



        $tabulka .= '</tr>';
    }

    $tabulka .= '</table>';

    if (isset($data['vrat_kod']) && $data['vrat_kod']) {
        return $tabulka;
    } else {
        echo $tabulka;
    }
}

?>

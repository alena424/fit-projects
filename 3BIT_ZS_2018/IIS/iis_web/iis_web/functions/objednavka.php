<?php
/**
 * 
 * @param type $objednavka      hidden input povinny
 * @param type $piti_pop
 * @param type $jidlo_pop
 * @param type $stoly_pop
 * @param type $stul            pokud je zadan pak se bere automaticky jako hidden input
 */
function novaPotravina($objednavka, $piti_pop, $jidlo_pop, $stoly_pop, $stul = null, $child_active = 0) {
    //include 'functions/formular.php';

    $default_vals = array();
    $params_amount = array('piti', 'jidlo');

    foreach ($params_amount as $attr) {
        if (isset($params[$attr]) && !empty($params[$attr]) && $params[$attr][0]) {
            $default_vals[$attr] = array();
            $poc = 0;
            // budeme potrebovat pridat i mnozstvi
            foreach ($params[$attr] as $value) {
                $pom_array = array();
                // jake obsahuje mnozstvi?
                $pom_array['item'] = $value;

                if (isset($params['mnozstvi_' . $attr]) && $params['mnozstvi_' . $attr][$poc]) {
                    //echo 'param' .$params['mnozstvi_' . $attr][$poc] .' ' .$poc .'<br>';
                    $pom_array['amount'] = $params['mnozstvi_' . $attr][$poc];
                } else {
                    $pom_array['amount'] = 1;
                }
                
                if (isset($params['detska' . $attr]) && $params['detska' . $attr][$poc]) {
                    //echo 'param' .$params['mnozstvi_' . $attr][$poc] .' ' .$poc .'<br>';
                    $pom_array['detska'] = $params['detska' . $attr][$poc];
                } else {
                    $pom_array['detska'] = 1;
                }

                $default_vals[$attr][$poc] = $pom_array;
                $poc++;
            }
        }
    }

    $formular['piti'] = array('name' => 'Pití', 'type' => 'popup', 'force' => 0, 'value' => $default_vals['piti'], 'multiple' => 1, 'popup' => $piti_pop, 'amount' => 1 );
    $formular['jidlo'] = array('name' => 'Jídlo', 'type' => 'popup', 'force' => 0, 'value' => $default_vals['jidlo'], 'multiple' => 1, 'popup' => $jidlo_pop, 'amount' => 1, 'child_active' => 0);

    if ($stul) {
        
        $formular['stul'] = array('name' => '', 'type' => 'number', 'force' => 1, 'value' => $stul, 'hidden' => 1, 'dvojtecka' => 2);
    } else {
        $formular['stul'] = array('name' => 'Stůl', 'type' => 'popup', 'force' => 1, 'value' => $params['stul'][0], 'multiple' => 0, 'popup' => $stoly_pop);
    }
    $formular['objednavka'] = array('name' => '', 'type' => 'number', 'force' => 1, 'value' => $objednavka, 'hidden' => 1, 'dvojtecka' => 2);

    $info['buttons'] = array(
        array(value => "Přidat k objednávce", name => 'uloz_objednavku'),
        array(value => "Tisk účtenky", 'name' => 'tisk_uctenky', )//'modals' => 1, 'title' => 'ÚČTENKA', 'body' => 'Účtenka byla vytisknuta.', 'id' => 'example' )
    );
    $info['hiddens'] = array(
        array(value => $_GET['objednavka'], name => $objednavka),
        array(value => $_GET['ok'], name => 'ok'),
        array(value => $_GET['stoly'], name => 'stoly'),
    );

    $info['cols'] = 1;
    addFormular($formular, $info);
}

?>
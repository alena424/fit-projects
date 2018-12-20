
<?php

/**
 * 
 * @param type $data            : array data do popupu
 * @param type $selected        : array vybrane moznosti popupu
 * @param type $required        : bool je popup povinny
 * @param type $name            : string jmeno popupu
 * @param type $multiple        : bool umozenuje vice moznosti
 * @param type $can_be_deleted  : bool muze byt hodnota smazana
 * @return string               : popup
 */
function popup_menu(
$data = array(),
        $selected = array(), 
        $required = 0, 
        $name = 'popup', 
        $multiple = 0,
        $can_be_deleted = 0, 
        $amount = 0, 
        $sel_amount = 1,
        $addition = null, 
        $in_uisbox = 0,
        $child_active = 0, # aktuvuje vlozeni checkboxu na destskou porci
        $child_val = 0) { # defaultni hodnota checkboxu v destske porci
    $popup = '';

    $text_required = '';
    if ($required) {
        $text_required = 'required';
    }
    $text_multiple = '';
    if ($multiple) {
        $text_multiple = 'multiple=\'multiple\'';
    }
    $text_plus_minus = '';
    if ($can_be_deleted) {
        $popup .= "<div class='multipleRow' style='inline-block'>";
        $text_plus_minus = "<a href='javascript:void(0)' ><img  data-toggle='tooltip' data-placement='top' title='Přidat' class='plusBtn'  width='20px' src='img/plus_1.png'></a>"
                . "<a href='javascript:void(0)'><img data-toggle='tooltip' data-placement='top' title='Odebrat' class='removeBtn' width='30px' src='img/close.png'></a>"
        ;
    }

    $popup .= "<select name=$name style='width:auto; ' class='form-control pop custaMultipleFieldContainer' $text_required $text_multiple >";

    if (!$required) {
        $popup .= "<option selected value=''> -- nezadáno -- </option>";
    }
    //print_r("<br>");
    //print_r($sel_amount);
    $class_add = '';
    if ($in_uisbox) {
        $class_add = ' uisbox_radky_tabulka ';
    }

$counter= 0;
    foreach ($data as $key => $value) {
       
        $text_sel = '';
        $add = $addition ? $addition : 'Ks';
        
        if (in_array($key, array_values($selected))) {
            $text_sel = 'selected';
            if (isset($sel_amount)) {
                //echo 'seelcted is ' . $selected['amount'] . ' '.$sel_amount. " [$key] <br>";

                $text_amount = " &emsp;<input class=' $class_add my_input_form input_small' name='mnozstvi_$name' type='number' min='1' value='" . $sel_amount . "'/> $add";
            } else if ($amount) {
                echo 'what is the amount of this selected key in popup? [' . $key . ']';
            }

            if ($child_val) {
                $text_amount .= " &emsp;<input type='checkbox' name='detska_$name' min='1' value='" . $child_val . "'/>" .'dětská';
            }
        } else {
            if (!$text_amount) {
                $text_amount = " &emsp;<input class='$class_add my_input_form input_small' name='mnozstvi_$name' min='1' type='number' value='1'/> $add";
            }
            if ($child_active) {
                $text_amount .= " &emsp;<input type='checkbox' name='detska_$name' min='1' value=1 />" .'  dětská';
            }
        }
        // tady pak bude value['item']
        if (isset($value['item'])) {
            $popup .= "<option $text_sel value=$key>" . $value['item'] . "</option>";
        } else {
            $popup .= "<option $text_sel value=$key> $value </option>";
        }
        
    }


    $popup .= "</select>";


    if ($amount) {
        $popup .= $text_amount;
    }
    if ($can_be_deleted) {
        $popup .= $text_plus_minus;
        $popup .= " </div>";
    }

    return $popup;
}

function add_popup_menu($data = array(), $selected = array(), $required = 0, $name = 'popup', $multiple = 0, $can_be_deleted = 0, $amount = 0) {
    echo popup_menu($data, $selected, $required, $name, $multiple, $can_be_deleted, $amount = 0);
}

/**
 * 
 * @param type $data 
 * data     : array( name => 'stul', value => 2, checked => 1/0, label => 'č. 21', class => 'specialnameofclass' )
 */
function checkbox($data) {
    if (!is_array($data) || !isset($data['name']) || !isset($data['value'])) {
        echo 'Error in checkbox structure';
        return 0;
    }
    $checked = '';
    if (isset($data['checked']) && $data['checked']) {
        $checked = 'checked';
    }
    $class = ''; // specialni nazev tridy
    if (isset($data['class'])) {
        $class = data['class'];
    }
    $checkbox = '';
    $checkbox .= "<label class='checkbox_custom $class'><input type='checkbox' name=" . $data['name'] . " value=" . $data['value'] . " $checked><span class='checkmark'></span> " . $data['label'] . "</label>";
    return $checkbox;
}

function add_checkbox($data) {
    echo checkbox($data);
}

/**
 * 
 * @param type $data : array(
 * data     : array( array( name => 'č. 21', value => 2, checked => 1/0 ), ... )
 * name     : string identifikator checkboxu
 * addOk    : boolean ccheme pridat buttonOK
 * direction: [vertical|horizontal] orientace checkboxu
 * )
 */
function checkboxes($data) {
    
}

function modal_box($data) {
    ?>
    <!--Modal -->
    <div class = "modal fade" id = "<?php echo $data['id']; ?>" tabindex = "-1" role = "dialog" aria-labelledby = "exampleModalLabel" aria-hidden = "true">
        <div class = "modal-dialog" role = "document">
            <div class = "modal-content">
                <div class = "modal-header">
                    <h5 class = "modal-title" id = "exampleModalLabel"> <?php echo $data['title'] ?></h5>
                    <button type = "button" class = "close" data-dismiss = "modal" aria-label = "Close">
                        <span aria-hidden = "true">&times;
                        </span>
                    </button>
                </div>
                <div class = "modal-body">
    <?php echo $data['body'] ?>
                </div>
                <div class = "modal-footer">
                    <button type = "button" class = "btn btn-secondary" data-dismiss = "modal">Zavřít</button>
                </div>
            </div>
        </div>
    </div>
    <?php
}
?>

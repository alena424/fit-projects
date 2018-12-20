<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function vratDatum($datum = null, $cas = 1) {
    $datum = new DateTime($datum);  
    if ( $cas ){
        return $datum->format('d.m.Y H:i:s');
    }
    return $datum->format('d.m.Y');
    

}

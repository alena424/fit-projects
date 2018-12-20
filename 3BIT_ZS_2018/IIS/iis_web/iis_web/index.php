<?php

mb_internal_encoding("UTF-8");

function autoloadFunkce($trida) {
    // Končí název třídy řetězcem "Kontroler" ?
    if (preg_match('/Kontroler$/', $trida))
        require("kontrolery/" . $trida . ".php");
    else
        require("modely/" . $trida . ".php");
}

spl_autoload_register("autoloadFunkce");

Db::connect("c159um.forpsi.com", "f85392", "FN3xs4R", "f85392");

session_start();
$smerovac = new SmerovacKontroler();

// url adresu najedme v $_SERVER[ 'REQUEST_URI' ]
$smerovac->zpracuj(array($_SERVER['REQUEST_URI']));
$smerovac->vypisPohled();

?>

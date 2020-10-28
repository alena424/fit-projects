<?php

$string_ins = array("acc", "cb_", "cls", "gui", "har","vcl", "vln");
$wind_ins = array("bsn", "flt", "sax", "tbt","trp", "tub");
$perc_ins = array("per");
$others_ins = array("oth");

$low_pitch = array('tub', 'cb_', 'gui', 'har', 'per', 'tbt'); #bigger
$high_pitch = array('flt','sax', 'vln', 'bsn','cls','acc', 'trp','vcl'); # smaler

#vln 1400, 100
//$filebat = file("statistics_v05.json");

$filebat = file($argv[1]);

$myfile = fopen("excelfile.csv", "w");
$trims = " \t\n\r\0\x0B\"";
$stats = array();

foreach($filebat as $row) {
    $pieces_array = explode(":", $row);
    if (!isset($pieces_array[0]) || !isset($pieces_array[1]) ){
        continue;
    }
    $string_description = explode(".",$pieces_array[0], 2);
    $val =  $pieces_array[1];

    $name = trim($string_description[0],  $trims);
    $feature = trim($string_description[1], $trims);

    $instrument = substr($name, 0, 3);

    # categorize teh string
    $name_group = '';
    $is_str = in_array($instrument, $string_ins);

    if ($is_str){
        $name_group = "string";
    } else {
        $is_wind = in_array($instrument, $wind_ins);
        if ($is_wind){
            $name_group = "wind";
        } else{
            $is_perc = in_array($instrument, $perc_ins);
            if ($is_perc){
                $name_group = 'percussive';
            } else {
                $name_group = 'others';
            }
        }
    }
   // $is_oth = in_array($instrument, $others_ins);
    $pitch = 'high';
    if (! isset($stats[$name])){
        $stats[$name] = array();
        if ($name_group == 'percussive'){
            array_push($stats[$name], array("percussion" => 'percussive' ."," ) );
        } else {
            array_push($stats[$name], array("percussion" => 'nonpercussive' ."," ) );
        }
        array_push($stats[$name], array("name group" => $name_group ."," ) );
    }
    if ($feature == 'lowlevel.spectral_centroid.mean'){
        if ($val < 400){
            $pitch = 'low';
            print $name . "\n";
        }
    }
    array_push($stats[$name], array("pitch" => $pitch ."," ) );
    array_push($stats[$name], array($feature =>$val ));

}
$once = true;
fwrite($myfile, "name,");
foreach ($stats as $instrum_name => $features){
    $val_string = "";
    foreach ($features as $pom){
        foreach ($pom as $name => $val) {
            if ($once) {
                fwrite($myfile, trim($name) . ',');

            }
            $val_string .= trim($val) ;

        }
    }
    if ($once){
        fwrite($myfile, "\n");
    }
    $once = false;
    fwrite($myfile, trim($instrum_name) . ','.$val_string );
    fwrite($myfile,"\n");
}

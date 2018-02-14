<?php

#echo 'ahoj';
$array = array();
preg_match_all(  "/(LABEL)\s+(\w+)/","LABEL ahoj", $array );
print_r($array);

?>

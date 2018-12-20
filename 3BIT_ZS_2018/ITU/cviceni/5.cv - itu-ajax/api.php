<?php
  require('config.php');

  if (!(mysql_connect($config['dbhost'], $config['dbuser'], $config['dbpass']))) {
    throw new Exception(mysql_error());
  }
  if (!(mysql_select_db($config['dbname']))) {
    throw new Exception(mysql_error());
  }

  if (isset($_SERVER['HTTP_REFERER']) && preg_match("/\~(x\S\S\S\S\S\d\S)/i", $_SERVER['HTTP_REFERER'], $ldata)) {
    $user = $ldata[1];
  } else {
    $user = "xlogin00";
  }

  if (isset($_REQUEST['data'])) {
    $query = sprintf("INSERT INTO `itu-ajax` (`login`, `cnt`, `dts`) VALUES (
    '%s', '%s', CURRENT_TIMESTAMP )", 
    mysql_real_escape_string($user), mysql_real_escape_string(strip_tags($_REQUEST['data'])));
    

    if (!mysql_query($query)) {
      throw new Exception(mysql_error());
    }

    echo 'Your message was stored';

  } else {

    $query = "SELECT * FROM `itu-ajax` ORDER BY `id` DESC LIMIT 20";
    if (!($result = mysql_query($query))) {
      throw new Exception(mysql_error());
    }

    $response = array();
    $i = 0;
    while ($row = mysql_fetch_array($result, MYSQL_ASSOC)) {
      $row['cnt'] = htmlentities($row['cnt']);
      $response[$i++] = $row;
    }
    $response = array_reverse($response);

    if (isset($_REQUEST['debug'])) {
      echo "<pre>".print_r($response,true)."</pre>";
    } else {
      echo json_encode($response);
    }
  }
?>
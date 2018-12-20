<?php include 'functions/uisbox.php'; 
$data = 'aaa';
//uisbox($data);
?>

<h2>Denní menu</h2>
<br>
<div class="col-md-6">
    <label class="stredni_pismo">JÍDLO</label>
    <hr>
    <?php
    foreach ($jidlo as $value) {
        echo "<div class='col-md-6'>";
        echo "<a  onclick='javascript:ShowFood(".$value['id'].")'>". $value['nazev'] ."</a>";
        echo "</div>";
        echo "<div class='col-md-6'>";
        echo $value['cena'];
        echo "</div>";
    }
    
    
    ?>
    
</div>


<div class="col-md-6">
    <label class="stredni_pismo">PITÍ</label>
    <hr>
    <?php
    foreach ($piti as $value) {
        echo "<div class='col-md-6'>";
       echo "<a  onclick='javascript:ShowFood(".$value['id'].")'>". $value['nazev'] ."</a>";
        echo "</div>";
        echo "<div class='col-md-6'>";
        echo $value['cena'];
        echo "</div>";
    }
    ?>
    &nbsp;
    &nbsp;
</div>
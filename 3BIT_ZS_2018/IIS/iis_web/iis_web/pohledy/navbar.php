<?php
$zalozka = ( isset($_GET['zalozka']) ) ? $_GET['zalozka'] : 'menu';
$spravceUzivatelu = new SpravceUzivatelu();
$pravo = $spravceUzivatelu->vratUzivatelePrava();
$jmeno = ( isset($_SESSION['uzivatel']) ) ? ( $_SESSION['uzivatel']['jmeno'] . ' ' . $_SESSION['uzivatel']['prijmeni'] ) : '';
$zalozka = $this->kontroler->data['zalozka'];
?>

<script type="text/javascript">
    $(document).ready(function ($) {
        var url = <?php echo '"' . $zalozka . '"' ?>;
        $('.nav li a[href="' + url + '"]').addClass('active');
    });

</script>

<!--
<nav class="navbar">
    <div class="container_fluid menu flex-md-column">
        <div class="navbar-header">
            <a class="navbar-brand logo" href="/">
                <img src="img/logo.svg" alt="Hlavní strana" class="img_logo" >
            </a>
        </div>
        <ul class="nav navbar-nav">
<?php
/* if ($pravo == 3 || $pravo == 1) {
  echo "<li><a href='rezervace'>REZERVACE</a></li>";
  echo "<li><a href='objednavky'>OBJEDNÁVKY</a></li>";
  }
  if ($pravo == 2 || $pravo == 1) {
  echo "<li><a href='jidelni-listek'>JÍDELNÍ LÍSTEK</a></li>";
  echo "<li><a href='potraviny'>POTRAVINY</a></li>";
  }
  if ($pravo == 1) {
  echo "<li><a href='zamestnanci'>ZAMĚSTNANCI</a></li>";
  } */
?>

        </ul>

        <ul class="nav navbar-nav navbar-right ml-auto small">
<?php
if ($pravo == 0) {
    // nejsme prihlaseni
    ?>
                    <li><a href="login"><span class="glyphicon glyphicon-user"></span> přihlásit se</a></li>

    <?php
} else {
    // prihlaseni


    /* echo '<li><a href="signout"><span class="glyphicon glyphicon-user"></span>odhlásit se</a>                </li>';

      echo '<li><a href="muj-ucet">Přihlášen: ' . $jmeno . '</a></li>'; */
}
?>
        </ul>


    </div>
</nav>
-->
<nav class="navbar navbar-expand-md navbar-dark bg-dark">
    <div class="container"> <button class="navbar-toggler navbar-toggler-right border-0" type="button" data-toggle="collapse" data-target="#navbar12">
            <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbar12"> <a class="navbar-brand navbar-header d-none d-md-block" href="/"><i class="fa fa-eercast"></i>WAWE</a>
            <ul class="navbar-nav mx-auto">
                <?php
                if ($pravo == 4 || $pravo == 1) {
                    echo "<li><a class='nav-link' href='rezervace'>Rezervace</a></li>";
                    echo "<li><a class='nav-link' href='objednavky'>Objednávky</a></li>";
                }
                if ($pravo == 2 || $pravo == 1 || $pravo == 3) {
                    if ($pravo != 3) {
                        echo "<li><a class='nav-link' href='jidelni-listek'>Jídelní lístek</a></li>";
                    }

                    echo "<li><a class='nav-link' href='potraviny'>Potraviny</a></li>";
                }
                if ($pravo == 1) {
                    echo "<li><a class='nav-link' href='stoly'>Stoly</a></li>";
                    echo "<li><a class='nav-link' href='zamestnanci'>Zaměstnanci</a></li>";
                    echo "<li><a class='nav-link' href='statistiky'>Statistiky</a></li>";
                }
                ?>
            </ul>
            <ul class="navbar-nav navbar-right">
                <?php
                if ($pravo == 0) {
                    // nejsme prihlaseni
                    ?>
                    <li class="nav-item"> <a class="nav-link text-primary" href="login"><i class="fa fa-user fa-fw"></i>Přihlásit se</a> </li>

                    <?php
                } else {
                    // prihlaseni
                    ?>
                    <li class="nav-item"> <a class="nav-link" href="signout">Odhlásit se</a> </li>
                    <li class="nav-item"> <a class="nav-link text-primary" href="muj-ucet"><i class="fa fa-user fa-fw"></i><?php echo $jmeno ?> </a> </li>
                    <?php
                }
                ?>
            </ul>
        </div>
    </div>
</nav>




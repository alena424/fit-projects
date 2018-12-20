-- phpMyAdmin SQL Dump
-- version 3.5.FORPSI
-- http://www.phpmyadmin.net
--
-- Počítač: 81.2.194.159
-- Vygenerováno: Pon 03. pro 2018, 17:19
-- Verze MySQL: 5.6.33-79.0-log
-- Verze PHP: 5.2.17

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Databáze: `f85392`
--
CREATE DATABASE `f85392` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
USE `f85392`;

-- --------------------------------------------------------

--
-- Struktura tabulky `objednavka`
--

CREATE TABLE IF NOT EXISTS `objednavka` (
  `id_objednavka` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `datum` datetime DEFAULT NULL,
  `vytvoril` int(10) NOT NULL,
  `rezervace` int(10) DEFAULT NULL,
  `stul` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`id_objednavka`),
  KEY `vytvoril` (`vytvoril`),
  KEY `rezervace` (`rezervace`),
  KEY `stul` (`stul`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=15955 ;

--
-- Vypisuji data pro tabulku `objednavka`
--

INSERT INTO `objednavka` (`id_objednavka`, `datum`, `vytvoril`, `rezervace`, `stul`) VALUES
(15949, '2018-12-02 12:02:00', 2, 63, 1),
(15950, '2018-12-02 18:39:32', 2, NULL, 1),
(15951, '2018-12-02 18:39:45', 2, NULL, 4),
(15952, '2018-12-02 18:42:39', 2, NULL, 5),
(15953, '2018-12-02 18:42:44', 2, NULL, 5),
(15954, '2018-12-02 18:43:01', 2, NULL, 6);

-- --------------------------------------------------------

--
-- Struktura tabulky `objednavka_pro_stul`
--

CREATE TABLE IF NOT EXISTS `objednavka_pro_stul` (
  `stul` int(10) NOT NULL,
  `objednavka` int(10) NOT NULL,
  KEY `stul` (`stul`),
  KEY `objednavka` (`objednavka`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Struktura tabulky `obsahuje_potravina`
--

CREATE TABLE IF NOT EXISTS `obsahuje_potravina` (
  `id_obsahuje_potravina` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `potravina` int(10) NOT NULL,
  `objednavka` int(10) NOT NULL,
  `mnozstvi` varchar(10) DEFAULT NULL,
  `vlozeno` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id_obsahuje_potravina`),
  KEY `FK_obsahuje_potravina_potravina` (`potravina`),
  KEY `FK_obsahuje_potravina_objednavka` (`objednavka`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=96 ;

--
-- Vypisuji data pro tabulku `obsahuje_potravina`
--

INSERT INTO `obsahuje_potravina` (`id_obsahuje_potravina`, `potravina`, `objednavka`, `mnozstvi`, `vlozeno`) VALUES
(79, 1, 15949, '3', '2018-12-02 18:37:54'),
(80, 6, 15949, '4', '2018-12-02 18:37:54'),
(81, 5, 15949, '1', '2018-12-02 18:37:54'),
(82, 1, 15950, '3', '2018-12-02 18:39:32'),
(83, 5, 15950, '2', '2018-12-02 18:39:32'),
(84, 2, 15951, '3', '2018-12-02 18:39:45'),
(85, 5, 15951, '1', '2018-12-02 18:39:45'),
(86, 6, 15951, '1', '2018-12-02 18:39:45'),
(87, 3, 15952, '3', '2018-12-02 18:42:39'),
(88, 5, 15952, '1', '2018-12-02 18:42:39'),
(89, 6, 15952, '3', '2018-12-02 18:42:39'),
(90, 15, 15953, '3', '2018-12-02 18:42:44'),
(91, 5, 15953, '1', '2018-12-02 18:42:44'),
(92, 6, 15953, '3', '2018-12-02 18:42:44'),
(93, 15, 15954, '3', '2018-12-02 18:43:01'),
(94, 5, 15954, '1', '2018-12-02 18:43:01'),
(95, 3, 15949, '4', '2018-12-02 20:52:39');

-- --------------------------------------------------------

--
-- Struktura tabulky `obsahuje_surovina`
--

CREATE TABLE IF NOT EXISTS `obsahuje_surovina` (
  `surovina` int(10) NOT NULL,
  `potravina` int(10) NOT NULL,
  `mnozstvi` varchar(10) DEFAULT NULL,
  KEY `surovina` (`surovina`),
  KEY `potravina` (`potravina`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `obsahuje_surovina`
--

INSERT INTO `obsahuje_surovina` (`surovina`, `potravina`, `mnozstvi`) VALUES
(1, 2, '120'),
(3, 2, '80'),
(4, 3, '150'),
(5, 3, '20'),
(6, 4, '50'),
(7, 4, '30'),
(8, 4, '20'),
(9, 5, '50'),
(2, 8, '1'),
(1, 8, '1'),
(7, 8, '1'),
(3, 1, '102'),
(2, 16, '100'),
(5, 17, '25'),
(9, 6, '100'),
(3, 15, '25'),
(2, 17, '12'),
(1, 1, '100'),
(2, 15, '10'),
(2, 1, '4'),
(2, 20, '2'),
(6, 1, '1');

-- --------------------------------------------------------

--
-- Struktura tabulky `potravina`
--

CREATE TABLE IF NOT EXISTS `potravina` (
  `id_jidlo` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `nazev` varchar(50) DEFAULT NULL,
  `cena` int(10) DEFAULT NULL,
  `pocet_porci` int(4) DEFAULT NULL,
  `mnozstvi_sacharidu` int(5) DEFAULT NULL,
  `energie_hodnota` int(5) DEFAULT NULL,
  `bilkoviny` int(5) DEFAULT NULL,
  `tuky` int(5) DEFAULT NULL,
  `zrusena` int(2) DEFAULT NULL,
  `expirace` datetime DEFAULT NULL,
  `typ_sklenice` varchar(30) DEFAULT NULL,
  `pribor` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`id_jidlo`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=21 ;

--
-- Vypisuji data pro tabulku `potravina`
--

INSERT INTO `potravina` (`id_jidlo`, `nazev`, `cena`, `pocet_porci`, `mnozstvi_sacharidu`, `energie_hodnota`, `bilkoviny`, `tuky`, `zrusena`, `expirace`, `typ_sklenice`, `pribor`) VALUES
(1, 'kure s brambory a tatarkou', 32, 4, 83, 455, 21, 15, 0, '2018-03-26 18:47:05', NULL, 'normalni'),
(2, 'kure s ryzi', 113, 21, 74, 455, 20, 12, 0, '2018-03-26 18:47:05', NULL, 'normalni'),
(3, 'bolonske spagety', 87, 21, 74, 455, 20, 12, 0, '2018-03-26 18:47:05', NULL, 'vidlicka a lzicka'),
(4, 'pivo', 20, 100, 74, 455, 20, 12, 1, '2018-03-26 18:47:05', 'pullitr', NULL),
(5, 'vino', 25, 21, 74, 455, 20, 12, 0, '2018-03-26 18:47:05', 'na vino', NULL),
(6, 'vodka', 70, 22, 55, 34, 20, 12, 0, '2018-03-29 18:47:05', 'panak', NULL),
(8, 'Alenka', 10, 10, 10, 10, 10, 10, 1, '2018-11-15 00:00:00', NULL, 'gfgfg'),
(15, 'Smažený sýr', 150, 8, 20, 120, 20, 20, 0, '2019-02-07 00:00:00', NULL, 'normalni'),
(16, 'Smažený sýr', 150, 8, 20, 120, 20, 20, 1, '2019-02-07 00:00:00', NULL, 'normalni'),
(17, 'Smažená houska', 150, 8, 20, 120, 20, 20, 0, '2019-02-22 00:00:00', NULL, 'normalni'),
(20, 'kure s brambory a tatarkou', 1, 1, 3, 4, 6, 4, 0, '2019-07-02 00:00:00', NULL, 'hjggjh');

-- --------------------------------------------------------

--
-- Struktura tabulky `pozice`
--

CREATE TABLE IF NOT EXISTS `pozice` (
  `id_pozice` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `nazev` varchar(30) NOT NULL,
  `popis` varchar(100) NOT NULL,
  PRIMARY KEY (`id_pozice`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=5 ;

--
-- Vypisuji data pro tabulku `pozice`
--

INSERT INTO `pozice` (`id_pozice`, `nazev`, `popis`) VALUES
(1, 'spolumajitel', 'Ma opravneni ke vsemu zlemu.'),
(2, 'sefkuchar', 'Ma opravneni editovat potraviny i suroviny.'),
(3, 'kuchar', 'Ma opravneni editovat pouze suroviny.'),
(4, 'servirka', 'Ma opravneni vytvaret objednavky, uctenky.');

-- --------------------------------------------------------

--
-- Struktura tabulky `rezervace`
--

CREATE TABLE IF NOT EXISTS `rezervace` (
  `id_rezervace` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `datum` datetime DEFAULT NULL,
  `doba` int(10) NOT NULL,
  `jmeno_zakaznika` varchar(30) NOT NULL,
  `prijmeni_zakaznika` varchar(30) NOT NULL,
  `kontakt_zakaznika` varchar(30) DEFAULT NULL,
  `vlozil` int(10) unsigned NOT NULL,
  `identifikator` varchar(50) NOT NULL,
  PRIMARY KEY (`id_rezervace`),
  KEY `vlozil` (`vlozil`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=65 ;

--
-- Vypisuji data pro tabulku `rezervace`
--

INSERT INTO `rezervace` (`id_rezervace`, `datum`, `doba`, `jmeno_zakaznika`, `prijmeni_zakaznika`, `kontakt_zakaznika`, `vlozil`, `identifikator`) VALUES
(62, '2018-12-02 12:34:00', 60, 'Jan', 'Novák', 'novak@seznam.cz', 2, '5c04165a61708'),
(63, '2018-12-02 12:02:00', 60, 'Václav', 'Nedorost', '605405506', 2, '5c0418728dea1'),
(64, '2018-12-02 12:03:00', 60, 'Milada', 'Horáková', 'miladicka@seznam.cz', 2, '5c0418a801056');

-- --------------------------------------------------------

--
-- Struktura tabulky `rezervace_na_stul`
--

CREATE TABLE IF NOT EXISTS `rezervace_na_stul` (
  `stul` int(10) NOT NULL,
  `rezervace` int(10) NOT NULL,
  KEY `stul` (`stul`),
  KEY `rezervace` (`rezervace`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `rezervace_na_stul`
--

INSERT INTO `rezervace_na_stul` (`stul`, `rezervace`) VALUES
(1, 62),
(1, 63),
(2, 64),
(3, 64);

-- --------------------------------------------------------

--
-- Struktura tabulky `stul`
--

CREATE TABLE IF NOT EXISTS `stul` (
  `id_stul` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `umisteni` varchar(30) DEFAULT NULL,
  `pocet_osob` int(2) DEFAULT NULL,
  `zruseny` int(2) NOT NULL DEFAULT '0',
  `cislo` int(10) NOT NULL,
  PRIMARY KEY (`id_stul`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=17 ;

--
-- Vypisuji data pro tabulku `stul`
--

INSERT INTO `stul` (`id_stul`, `umisteni`, `pocet_osob`, `zruseny`, `cislo`) VALUES
(1, 'zahradka', 4, 0, 1),
(2, 'zahradka', 8, 0, 2),
(3, 'uvnitr', 12, 0, 3),
(4, 'uvnitr', 4, 0, 4),
(5, 'uvnitr', 4, 0, 5),
(6, 'uvnitr', 2, 0, 6),
(7, 'uvnitr', 3, 0, 7);

-- --------------------------------------------------------

--
-- Struktura tabulky `surovina`
--

CREATE TABLE IF NOT EXISTS `surovina` (
  `id_surovina` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `jmeno_suroviny` varchar(50) DEFAULT NULL,
  `zrusena` int(2) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_surovina`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=55 ;

--
-- Vypisuji data pro tabulku `surovina`
--

INSERT INTO `surovina` (`id_surovina`, `jmeno_suroviny`, `zrusena`) VALUES
(1, 'kure', 0),
(2, 'brambory', 0),
(3, 'ryze', 0),
(4, 'spagety', 0),
(5, 'syr', 0),
(6, 'chmel', 0),
(7, 'psenice', 0),
(8, 'voda', 0),
(9, 'hrozny', 1),
(53, 'nova', 0),
(54, 'ff', 0);

-- --------------------------------------------------------

--
-- Struktura tabulky `uctenka`
--

CREATE TABLE IF NOT EXISTS `uctenka` (
  `id_ucet` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `datum` datetime DEFAULT NULL,
  `konecna_suma` float NOT NULL,
  `objednavka` int(10) NOT NULL,
  PRIMARY KEY (`id_ucet`),
  KEY `objednavka` (`objednavka`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=34 ;

--
-- Vypisuji data pro tabulku `uctenka`
--

INSERT INTO `uctenka` (`id_ucet`, `datum`, `konecna_suma`, `objednavka`) VALUES
(30, '2018-12-02 20:52:43', 121, 15950),
(31, '2018-12-02 20:56:29', 496, 15952),
(32, '2018-12-02 20:56:35', 749, 15949),
(33, '2018-12-03 12:27:45', 685, 15953);

-- --------------------------------------------------------

--
-- Struktura tabulky `zamestnanec`
--

CREATE TABLE IF NOT EXISTS `zamestnanec` (
  `id_zam` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `jmeno` varchar(30) NOT NULL,
  `prijmeni` varchar(30) NOT NULL,
  `rodne_cislo` int(10) NOT NULL,
  `adresa` varchar(50) DEFAULT NULL,
  `pozice` int(10) DEFAULT NULL,
  `login` varchar(30) DEFAULT NULL,
  `heslo` varchar(100) DEFAULT NULL,
  `zruseny` int(2) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_zam`),
  KEY `pozice` (`pozice`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=16 ;

--
-- Vypisuji data pro tabulku `zamestnanec`
--

INSERT INTO `zamestnanec` (`id_zam`, `jmeno`, `prijmeni`, `rodne_cislo`, `adresa`, `pozice`, `login`, `heslo`, `zruseny`) VALUES
(1, 'Jan', 'Sorm', 975504, 'Tyrsova 70, Brno-Kralovo Pole', 1, 'jan', '$2y$10$bk3ZJlVm/UQBQ9zNQvNXZOEB9aqOmaWDbQZfq3pnPyJWVFKwRgDfO', 0),
(2, 'Alenka', 'Tesarova', 2147483647, 'Alencina 42, Brno-Lisen', 1, 'alena', '$2y$10$W85nj2Fr/0LZc.vexWNRA.Tm/o9so1n.qoKdQSYa2k3ANMrHxa1xi', 0),
(6, 'Arina', 'Starastsina', 2147483647, 'Jegora Pavlovice 4, Minsk', 4, 'arina', '$2y$10$3mJRmfyDbKyO2.9GBXj9buYVO1RFG2wUeSNC9Fuq1C8.4XfzSEF2i', 0),
(8, 'Radek', 'Pištělák', 1111111111, 'asdas', 4, 'radek', '$2y$10$CUbSrAZdFfOWDPVbujgzzeto9C2NYEyBCBwgzajxrVrn5OT5crq2q', 0),
(11, 'Zdeňka', 'Tesařová', 4444444, 'sss', 3, 'zdenka', '$2y$10$65ZTG8AP.4./7b/Rr9ObsOuEqQHROYQgPxa9k7K6Gs27JRlgI6Zl2', 0),
(12, 'novy', 'test', 1111111111, 'test', 4, 'test', '$2y$10$ZWtMprvMfSiC.AFtr6G7xuvnh3n3qfLXupOT1FN9xQlB/Dqkzp7ma', 1),
(14, 'Matěj', 'Brun', 454545, '4444', 2, 'matej', '$2y$10$UBWDqkMufHH4vIEMOVnJ/uT8DXipEWPunF/SWp3WgRKGdZSW/2ljy', 0),
(15, 'Jiří', 'Hynek', 2147483647, 'Božetěchova 32', 1, 'admin', '$2y$10$TUTnwtjtlzgRFWzw/W5c6O63QXt8ld85xzH6T4.nYndTmTYXan45a', 0);

--
-- Omezení pro exportované tabulky
--

--
-- Omezení pro tabulku `objednavka`
--
ALTER TABLE `objednavka`
  ADD CONSTRAINT `objednavka_ibfk_1` FOREIGN KEY (`stul`) REFERENCES `stul` (`id_stul`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

  /**
 * @file error.h
 * Reseni IJC-DU1, priklad b)
 *
 * @date 24.3.2017
 * @author Alena Tesarova, FIT
 * @gcc 5.4
 *
 * Popis:
 * Hlavickovy soubor pro error.c
 *
*/


#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void warning_msg(const char *fmt, ...);

void error_msg(const char *fmt, ...);

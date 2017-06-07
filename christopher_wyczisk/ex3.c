/**
 * @file
 * @author  Christopher Wyczisk
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Dieses Programm schreibt seinen eigenen Code in die Konsole.
 */

#include<stdio.h>

/**
 * @return 0
 */
int main() {
    FILE *f = fopen(__FILE__, "r");
    char c;
    do {
       c = getc(f);
       putchar(c);
    } while(c != EOF);
    fclose(f);
    return 0;
}

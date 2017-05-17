#include<stdio.h>
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

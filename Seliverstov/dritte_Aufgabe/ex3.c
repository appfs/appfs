#include <stdio.h>
char*T="#include <stdio.h>%cchar*T=%c%s%c;%cint main(){printf(T, 10, 34, T, 34, 10, 10);}%c";
int main(){printf(T, 10, 34, T, 34, 10, 10);}

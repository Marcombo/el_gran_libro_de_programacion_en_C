#include <stdio.h>

int main(int argc, char *argv[])
{
    int n = 0;

    printf("Argumentos recibidos\n");

    for(n = 0; n < argc; n++)
    {
        printf(" Num. %d contenido=%s\n", n, argv[n]);
    }
    return 0;
}

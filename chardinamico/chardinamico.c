#include <stdio.h>
#include <stdlib.h> //contiene funciones como malloc y free
#include <string.h> //contiene funciones como strcpy y strcat

int main(int argc, char *argv[])
{
    char * lpszBuffer = NULL;
    size_t nTamanyoMax = 1024; // size_t equivale a un número
                               // entero largo de 64 bits sin signo
                               // lo cual es mucho mayor que 
                               // un int de 32 bits

    lpszBuffer = malloc(nTamanyoMax);
    if (lpszBuffer)
    {
        memset(lpszBuffer, 0, nTamanyoMax);
        strcpy(lpszBuffer, "En un lugar de la Mancha de cuyo "); 
        strcat(lpszBuffer, "nombre no quiero acordarme ");
        strcat(lpszBuffer, "no ha mucho que vivía un hidalgo ");
        strcat(lpszBuffer, "de los de lanza en astillero, ");
        strcat(lpszBuffer, "adarga antigua, rocín flaco ");
        strcat(lpszBuffer, "y galgo corredor.");
        printf(lpszBuffer);
        free(lpszBuffer);
    }

    return 0;
}
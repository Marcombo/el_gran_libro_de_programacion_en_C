#include <stdio.h>
#include <stdlib.h> //contiene funciones como malloc y free
#include <string.h> //contiene funciones como strcpy y strcat

int main(int argc, char *argv[])
{
    char szCadena[200];
    char * pCursor = NULL;     //Cursor para recorrer la cadena

    memset(szCadena, 0, sizeof(szCadena));
    strcpy(szCadena, "EN UN LUGAR DE LA MANCHA DE CUYO "); 
    strcat(szCadena, "NOMBRE NO QUIERO ACORDARME");
    printf("Original......: %s\n", szCadena);

    pCursor = szCadena;
    while (*pCursor)
    {
         *pCursor = (*pCursor >= 'A' && *pCursor<= 'Z') ? (*pCursor+=32) : (*pCursor);
         pCursor++;
    }
    printf("Minúsculas....: %s\n", szCadena);
    return 0;
}
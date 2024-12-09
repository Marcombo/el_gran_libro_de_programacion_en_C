#include <stdio.h>
#include <stdlib.h> //contiene funciones como malloc y free
#include <string.h> //contiene funciones como strcpy y strcat
#include <wtypes.h> (//Contiene booleanos y máscaras binarias)

int BuscarPosicionCadena(char * lpszBuffer, char * lpszToFind)
{
    int nLength = 0;
    int nLengthToFind = 0;
    int nPos = -1;
    char Ch = 0x00;
    BOOL bFound = 0;
    char * lpszFound = NULL;

    if (lpszBuffer!=NULL)
    {
        nLength = strlen(lpszBuffer);
        if (lpszToFind)
        {
            nLengthToFind = strlen(lpszToFind);
            if (nLength > nLengthToFind)
            {
                lpszFound = malloc(nLengthToFind+1);
                memset(lpszFound, 0, nLengthToFind +1);
                for (nPos = 0; nPos < nLength - strlen(lpszToFind);nPos++)
                {
                    Ch = lpszBuffer[nPos];
                    memcpy(lpszFound, lpszBuffer + nPos, 5);
                    *(lpszBuffer + 6) = 0x00;
                    if (!strcmp(lpszToFind, lpszFound))
                    {
                        bFound = 1;
                        break;
                    }
                }
                free(lpszFound);
            }
        }
    }
    nPos = (bFound) ? nPos : -1;
    return nPos;
}

int main(int argc, char *argv[])
{
    char * lpszBuffer = NULL;
    size_t nTamanyoMax = 1024; // size_t equivale a un número
                               // entero largo de 64 bits sin signo
                               // lo cual es mucho mayor que 
                               // un int de 32 bits
    
    char * lpszToFind = NULL;
    int nPos = -1;
    BOOL bFound = 0;

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

        //Buscar "flaco" dentro de la cadena según indice de recorrido:
        
        lpszToFind = malloc(6);
        strcpy(lpszToFind, "flaco");

        nPos = BuscarPosicionCadena(lpszBuffer, lpszToFind);
        if (nPos > -1)
        {
            printf("Texto '%s' encontrado en la posición %d.\n", lpszToFind, nPos);
        }
        free(lpszBuffer); // Atención, mover el free más abajo 
                          // del bucle para que no nos encontremos 
                          // con la cadena ya destruida.
    }

    return 0;
}
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include"./include/saludar.h"

int main(int argc, char *argv[])
{
    char szTexto[100];
    memset(szTexto, 0, sizeof(szTexto));

    strcpy(szTexto, ObtenerTextoIdioma(INGLES));
    printf("%s\n",szTexto);

}


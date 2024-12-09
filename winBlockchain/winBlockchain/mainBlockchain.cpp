//////////////////////////////////////////////////////////////////////////////
// Proyecto: BasicBlockchain
// Archivo : mainBlockchain.cpp
// Copyright 2023, 2024- Marcombo - Boixareu Editores, Barcelona
// Copyright 2023, 2024- Alfons Gonzalez Perez
// 
//////////////////////////////////////////////////////////////////////////////

// mainBlockchain.cpp : 
// 
// Este archivo contiene la función "main". La ejecuci¢n del programa comienza y termina ahí.
//

#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include <openssl/sha.h>
#include <openssl/crypto.h>

#include "BasicBlockchain.h"

//////////////////////////////////////////////////////////////////////////////
// Debemos configurar el entorno de Visual Studio 
// para añadir el directorio donde se encuentran 
// las librerias de include de SSL a los directorios 
// que usa el compilador para encontrar los ficheros 
// de Include.
// Si no, el compilador no va a encontrar los 
// ficheros de cabecera de SSL.
// Las librerías de SSL se pueden descargar desde Internet.
// Por ejemplo, se pueden descargar desde:
// https://slproweb.com/products/Win32OpenSSL.html
// 
// Una vez descargadas las librerías, debemos escoger el modelo de las mismas
// de entre las versiones de las librerías disponibles.
// En este caso, para poder desarrollar y depurar, se aconseja usar la 
// versión MTD (multithread debug).
// 
// Tras la compilación se debe realizar el paso de enlazado (link),
// por lo cual se deben usar las librerías siguientes:
//   - libcrypto.lib
//   - libssl.lib
// 
// Y finalmente, si trabajamos desde Windows, debemos colocar en el mismo 
// directorio en el que se halle el ejecutable (o en uno de los directorios 
// incluídos en el path) las librerías de enlace dinámico (dll) 
// correspondientes:
//   - libcrypto-3-x64.dll
//   - libssl-3-x64.dll
// 
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// **** LA VARIABLE MÁS IMPORTANTE ****
// Puntero a la cabeza de la lista de bloques,
// es decir, el puntero a la cabeza del blockchain:
//////////////////////////////////////////////////////////////////////////////

LPBLOCK pBlockchainHeader = NULL;

//////////////////////////////////////////////////////////////////////////////
// PROGRAMA PRINCIPAL MAIN
//////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    int nOpcion = 0;
    int nData = 0;
    int nBloques = 0;
    int nBloqueNum = 0;
    int nRandom = 0;
    int nCamposTecleados = 0;
    int n = 0;

    while (TRUE)
    {
        printf("\n");
        printf("MENU DE OPCIONES DE PROGRAMA:\n");
        printf("-----------------------------\n");
        printf(" 1.- Dar de alta nuevo bloque\n");
        printf(" 2.- Generar N bloques aleatorios\n");
        printf(" 3.- Contar bloques\n");
        printf(" 4.- Falsificar bloque N\n");
        printf(" 5.- Listado de todos los bloques\n");
        printf(" 6.- Verificar Blockchain\n");
        printf(" 7.- Reconstruir todo el Blockchain recalculando hashes falsos\n");
        printf(" 8.- Imprimir el hash de un bloque\n");
        printf(" 9.- Imprimir solo bloques falsos\n");
        printf("10.- Salir.\n");

        printf("Escoja una opcion del 1 al 10:");
        nCamposTecleados = scanf("%d", &nOpcion);
        if (nCamposTecleados)
        {
            switch (nOpcion)
            {
            case 1:printf("Especifique el numero Data:");
                nCamposTecleados = scanf("%d", &nData);
                ConcatenarNuevoBloque(pBlockchainHeader, nData);
                break;
            case 2:
                printf("Cuantos bloques aleatorios quiere generar?");
                nCamposTecleados = scanf("%d", &nBloques);
                for (n = 0; n < nBloques; n++)
                {
                    nRandom = rand() % ((n+1) * 10);
                    printf("Generando bloque %d con el Data = %d\n", n, nRandom);
                    ConcatenarNuevoBloque(pBlockchainHeader, nRandom);
                }
                break;
            case 3:
                nBloques = ContarBloques(pBlockchainHeader);
                printf("Hay %d bloques\n\n", nBloques);
                break;
            case 4:
                printf("Número del bloque que quiere alterar :");
                nCamposTecleados = scanf("%d", &nBloqueNum);
                printf("Indique ahora qué valor nData quiere poner:");
                nCamposTecleados = scanf("%d", &nData);
                FalsificarBloque(pBlockchainHeader, nBloqueNum, nData);
                break;
            case 5:
                ListadoBloques(pBlockchainHeader);
                break;
            case 6:
                VerificarBlockchain(pBlockchainHeader);
                break;
            case 7:
                ReconstruirBlockchain(pBlockchainHeader);
                break;
            case 8:
                printf("Número del bloque cuyo hash quiere imprimir:");
                nCamposTecleados = scanf("%d", &nBloqueNum);
                ImprimirLineaBloqueHash(pBlockchainHeader, nBloqueNum);
                break;
            case 9:
                ImprimirSoloBloquesFalsos(pBlockchainHeader);
                break;
            case 10:
                nBloques = LiberarBlockchain(pBlockchainHeader);
                printf("Se han liberado %d bloques + cabecera de lista.\n", nBloques);
                printf("Se ha colocado el puntero a Blockchain de nuevo a NULL.\n\n");
                break;
            default:
                printf("Opción incorrecta.\n");
                break;
            }
        }
        if (nOpcion == 10) break;
    }
    return 0;
}

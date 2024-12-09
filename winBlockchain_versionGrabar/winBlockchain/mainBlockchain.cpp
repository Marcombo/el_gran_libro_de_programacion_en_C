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

//Nombre convenido para el fichero que almacena el Blockchain:
char szNombreFicheroBlockchain[1024];

//////////////////////////////////////////////////////////////////////////////
// PROGRAMA PRINCIPAL MAIN
//////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    int nOpcion = 0;
    int nData = 0;
    int nBloques = 0;
    int nBloquesLiberados = 0;
    int nBloqueNum = 0;
    int nRandom = 0;
    int nCamposTecleados = 0;
    int nCursorRegistros = 0;
    int nError = 0;
    int n = 0;
    char Ch = 0x00;
    BOOL bOKRead = FALSE;
    BOOL bOKWritten = FALSE;
    BLOCK* pFoundBlock = NULL; //para procesos de búsqueda en RAM de un bloque

    //Establecer el nombre del fichero de almacenamiento del Blockchain:
    memset(szNombreFicheroBlockchain, 0, sizeof(szNombreFicheroBlockchain));
    strcpy(szNombreFicheroBlockchain, "./DatosBlockchain.dat");

    while (TRUE)
    {
        printf("\n");
        printf("MENU DE OPCIONES DE PROGRAMA - VERSION CON LECTURA Y GRABACION:\n");
        printf("---------------------------------------------------------------\n");
        printf(" 1.- Dar de alta nuevo bloque\n");
        printf(" 2.- Generar N bloques aleatorios\n");
        printf(" 3.- Contar bloques\n");
        printf(" 4.- Falsificar bloque N\n");
        printf(" 5.- Listado de todos los bloques\n");
        printf(" 6.- Verificar Blockchain\n");
        printf(" 7.- Reconstruir todo el Blockchain recalculando hashes falsos\n");
        printf(" 8.- Imprimir el hash de un bloque\n");
        printf(" 9.- Imprimir solo bloques falsos\n");
        printf("10.- Leer toda la lista de bloques de blockchain desde archivo\n");
        printf("11.- Grabar toda la lista de bloques de blockchain en archivo\n");
        printf("12.- Salir.\n");

        printf("Escoja una opcion del 1 al 12:");
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
                Pause();
                break;
            case 3:
                nBloques = ContarBloques(pBlockchainHeader);
                printf("Hay %d bloques\n\n", nBloques);
                Pause();
                break;
            case 4:
                printf("Número del bloque que quiere alterar :");
                nCamposTecleados = scanf("%d", &nBloqueNum);
                printf("Indique ahora qué valor nData quiere poner:");
                nCamposTecleados = scanf("%d", &nData);
                FalsificarBloque(pBlockchainHeader, nBloqueNum, nData);
                Pause();
                break;
            case 5:
                ListadoBloques(pBlockchainHeader);
                Pause();
                break;
            case 6:
                VerificarBlockchain(pBlockchainHeader);
                Pause();
                break;
            case 7:
                ReconstruirBlockchain(pBlockchainHeader);
                Pause();
                break;
            case 8:
                printf("Número del bloque cuyo hash quiere imprimir:");
                nCamposTecleados = scanf("%d", &nBloqueNum);
                ImprimirLineaBloqueHash(pBlockchainHeader, nBloqueNum);
                Pause();
                break;
            case 9:
                ImprimirSoloBloquesFalsos(pBlockchainHeader);
                Pause();
                break;
            case 10: //Leer toda la cadena Blockchain desde fichero
                nError = LeerCadenaBloquesEnteraDesdeFichero(szNombreFicheroBlockchain, &nBloques, &pBlockchainHeader);
                if (!nError)
                {
                    // INTERESANTE
                    // Vamos a leer y reconstruir los punteros y hashes. Vamos a explicarnos:
                    // 
                    // Hay dos formas de intentar hackear blockchain: uno es modificar el valor Data y otra 
                    // que no hemos visto, y que consiste en modificar el puntero de un bloque para que apunte 
                    // a un bloque falso, insertar el bloque falso y apuntar al siguiente bloque para continuar 
                    // la cadena, como si no hubiera pasado nada.
                    // 
                    // Esto no funcionaría, porque los punteros antiguos forman parte del cálculo  del hash  
                    // y se detectaría que los hashes no cuadran con los punteros.
                    // 
                    // Esto mismo nos sucede cuando leemos los bloques desde disco, porque los bloques contenían 
                    // los punteros y el valor de los punteros forma parte del cálculo del hash.
                    // 
                    // Si hemos cambiado los punteros para que sean los resultantes de las nuevas direcciones 
                    // dinámicas que nos acaban de dar las nuevas llamadas a malloc, y dado que ests nuevos punteros
                    // deberían formar parte del bloque, los hashes de los bloques con los punteros nuevos ya no 
                    // resultan válidos, puesto que todavía tienen en cuenta los punteros antiguos leídos del fichero. 
                    // Hace falta reconstruir los hashes para que haya una seguridad de punteros que impida 
                    // que, en RAM, puedan insertarnos un bloque de forma irregular.
                    //
                    // Por esta razón de peso, después de haber leído los bloques desde fichero,
                    // debemos llamar a la función ReconstruirBlockchain()
                    // y los hashes cuadrarán con sus nuevas posiciones en memoria.
                    // //
                    // De aquí deriva una nueva funcionalidad de seguridad: la última copia grabada del blockchain 
                    // no contiene los punteros válidos hacia los bloques actuales y reales reservados dinámicamente 
                    // con posiciones (punteros) nuevas en la RAM, de modo que la lectura a bajo nivel del blockchain 
                    // no facilitará en puntero real hacia ningún bloque para que pueda ser atacado ilegalmente.

                    ReconstruirBlockchain(pBlockchainHeader); //Recalcula los hashes con los nuevos punteros
                    printf("Hemos leido desde el fichero %s %d bloques de una Blockchain nueva.\n", szNombreFicheroBlockchain, nBloques);
                }
                else
                {
                    switch (nError)
                    {
                    case 1: printf("Error %d: No se ha podido encontrar el fichero %s\n", nError, szNombreFicheroBlockchain);
                        break;
                    case 2:
                        printf("Error %d: No se ha podido leer el bloque %d del fichero %s\n", 
                               nError, nBloques, szNombreFicheroBlockchain);
                    }
                }
                Pause();
                break;
            case 11: //Escribir toda la cadena blockchain hacia fichero
                nError = EscribirCadenaBloquesEnteraHaciaFichero(szNombreFicheroBlockchain, &nBloques, &pBlockchainHeader);
                if (!nError)
                {
                    // INTERESANTE
                    // Vamos a grabar mal sabiendo que los punteros no serán válidos. Vamos a explicarnos:
                    // 
                    // Hay dos formas de intentar hackear blockchain: uno es modificar el valor Data y otra 
                    // que no hemos visto, y que consiste en modificar el puntero de un bloque para que apunte 
                    // a un bloque falso, insertar el bloque falso y apuntar al siguiente bloque para continuar 
                    // la cadena, como si no hubiera pasado nada.
                    // 
                    // Esto no funcionaría, porque los punteros antiguos forman parte del cálculo  del hash  
                    // y se detectaría que los hashes no cuadran con los punteros.
                    // 
                    // Esto mismo nos sucede cuando leemos los bloques desde disco, porque los bloques contenían 
                    // los punteros y el valor de los punteros forma parte del cálculo del hash.
                    // 
                    // Si hemos cambiado los punteros para que sean los resultantes de las nuevas direcciones 
                    // dinámicas que nos acaban de dar las nuevas llamadas a malloc, y dado que ests nuevos punteros
                    // deberían formar parte del bloque, los hashes de los bloques con los punteros nuevos ya no 
                    // resultan válidos, puesto que todavía tienen en cuenta los punteros antiguos leídos del fichero. 
                    // Hace falta reconstruir los hashes para que haya una seguridad de punteros que impida 
                    // que, en RAM, puedan insertarnos un bloque de forma irregular.
                    //
                    // Por esta razón de peso, después de haber leído los bloques desde fichero,
                    // debemos llamar a la función ReconstruirBlockchain()
                    // y los hashes cuadrarán con sus nuevas posiciones en memoria.
                    // //
                    // De aquí deriva una nueva funcionalidad de seguridad: la última copia grabada del blockchain 
                    // no contiene los punteros válidos hacia los bloques actuales y reales reservados dinámicamente 
                    // con posiciones (punteros) nuevas en la RAM, de modo que la lectura a bajo nivel del blockchain 
                    // no facilitará en puntero real hacia ningún bloque para que pueda ser atacado ilegalmente.

                    printf("Hemos escrito desde RAM hacia el fichero %s %d bloques de una Blockchain nueva.\n", szNombreFicheroBlockchain, nBloques);
                }
                else
                {
                    switch (nError)
                    {
                    case 1: printf("Error %d: No se ha podido crear el fichero %s", nError, szNombreFicheroBlockchain);
                        break;
                    case 2:
                        printf("Error %d: No se ha podido escibir el bloque %d del fichero %s",
                            nError, nBloques, szNombreFicheroBlockchain);
                    }
                }
                Pause();
                break;
            case 12:
                nBloquesLiberados = LiberarBlockchain(pBlockchainHeader);
                printf("Se han liberado %d bloques + cabecera de lista.\n", nBloques);
                printf("Se ha colocado el puntero a Blockchain de nuevo a NULL.\n\n");
                break;
            default:
                printf("Opción incorrecta.\n");
                break;
            }
        }
        if (nOpcion == 12)
        {
            printf("Cerramos la aplicación.\n\n");
            break;
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Proyecto: BasicBlockchain
// Archivo : mainBlockchain.cpp
// Copyright 2023, 2024- Marcombo - Boixareu Editores, Barcelona
// Copyright 2023, 2024- Alfons Gonzalez Perez
// 
//////////////////////////////////////////////////////////////////////////////

// mainBlockchain.cpp : 
// 
// Este archivo contiene la funci�n "main". La ejecuci�n del programa comienza y termina ah�.
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
// para a�adir el directorio donde se encuentran 
// las librerias de include de SSL a los directorios 
// que usa el compilador para encontrar los ficheros 
// de Include.
// Si no, el compilador no va a encontrar los 
// ficheros de cabecera de SSL.
// Las librer�as de SSL se pueden descargar desde Internet.
// Por ejemplo, se pueden descargar desde:
// https://slproweb.com/products/Win32OpenSSL.html
// 
// Una vez descargadas las librer�as, debemos escoger el modelo de las mismas
// de entre las versiones de las librer�as disponibles.
// En este caso, para poder desarrollar y depurar, se aconseja usar la 
// versi�n MTD (multithread debug).
// 
// Tras la compilaci�n se debe realizar el paso de enlazado (link),
// por lo cual se deben usar las librer�as siguientes:
//   - libcrypto.lib
//   - libssl.lib
// 
// Y finalmente, si trabajamos desde Windows, debemos colocar en el mismo 
// directorio en el que se halle el ejecutable (o en uno de los directorios 
// inclu�dos en el path) las librer�as de enlace din�mico (dll) 
// correspondientes:
//   - libcrypto-3-x64.dll
//   - libssl-3-x64.dll
// 
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// **** LA VARIABLE M�S IMPORTANTE ****
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
    BLOCK* pFoundBlock = NULL; //para procesos de b�squeda en RAM de un bloque

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
                printf("N�mero del bloque que quiere alterar :");
                nCamposTecleados = scanf("%d", &nBloqueNum);
                printf("Indique ahora qu� valor nData quiere poner:");
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
                printf("N�mero del bloque cuyo hash quiere imprimir:");
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
                    // Esto no funcionar�a, porque los punteros antiguos forman parte del c�lculo  del hash  
                    // y se detectar�a que los hashes no cuadran con los punteros.
                    // 
                    // Esto mismo nos sucede cuando leemos los bloques desde disco, porque los bloques conten�an 
                    // los punteros y el valor de los punteros forma parte del c�lculo del hash.
                    // 
                    // Si hemos cambiado los punteros para que sean los resultantes de las nuevas direcciones 
                    // din�micas que nos acaban de dar las nuevas llamadas a malloc, y dado que ests nuevos punteros
                    // deber�an formar parte del bloque, los hashes de los bloques con los punteros nuevos ya no 
                    // resultan v�lidos, puesto que todav�a tienen en cuenta los punteros antiguos le�dos del fichero. 
                    // Hace falta reconstruir los hashes para que haya una seguridad de punteros que impida 
                    // que, en RAM, puedan insertarnos un bloque de forma irregular.
                    //
                    // Por esta raz�n de peso, despu�s de haber le�do los bloques desde fichero,
                    // debemos llamar a la funci�n ReconstruirBlockchain()
                    // y los hashes cuadrar�n con sus nuevas posiciones en memoria.
                    // //
                    // De aqu� deriva una nueva funcionalidad de seguridad: la �ltima copia grabada del blockchain 
                    // no contiene los punteros v�lidos hacia los bloques actuales y reales reservados din�micamente 
                    // con posiciones (punteros) nuevas en la RAM, de modo que la lectura a bajo nivel del blockchain 
                    // no facilitar� en puntero real hacia ning�n bloque para que pueda ser atacado ilegalmente.

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
                    // Vamos a grabar mal sabiendo que los punteros no ser�n v�lidos. Vamos a explicarnos:
                    // 
                    // Hay dos formas de intentar hackear blockchain: uno es modificar el valor Data y otra 
                    // que no hemos visto, y que consiste en modificar el puntero de un bloque para que apunte 
                    // a un bloque falso, insertar el bloque falso y apuntar al siguiente bloque para continuar 
                    // la cadena, como si no hubiera pasado nada.
                    // 
                    // Esto no funcionar�a, porque los punteros antiguos forman parte del c�lculo  del hash  
                    // y se detectar�a que los hashes no cuadran con los punteros.
                    // 
                    // Esto mismo nos sucede cuando leemos los bloques desde disco, porque los bloques conten�an 
                    // los punteros y el valor de los punteros forma parte del c�lculo del hash.
                    // 
                    // Si hemos cambiado los punteros para que sean los resultantes de las nuevas direcciones 
                    // din�micas que nos acaban de dar las nuevas llamadas a malloc, y dado que ests nuevos punteros
                    // deber�an formar parte del bloque, los hashes de los bloques con los punteros nuevos ya no 
                    // resultan v�lidos, puesto que todav�a tienen en cuenta los punteros antiguos le�dos del fichero. 
                    // Hace falta reconstruir los hashes para que haya una seguridad de punteros que impida 
                    // que, en RAM, puedan insertarnos un bloque de forma irregular.
                    //
                    // Por esta raz�n de peso, despu�s de haber le�do los bloques desde fichero,
                    // debemos llamar a la funci�n ReconstruirBlockchain()
                    // y los hashes cuadrar�n con sus nuevas posiciones en memoria.
                    // //
                    // De aqu� deriva una nueva funcionalidad de seguridad: la �ltima copia grabada del blockchain 
                    // no contiene los punteros v�lidos hacia los bloques actuales y reales reservados din�micamente 
                    // con posiciones (punteros) nuevas en la RAM, de modo que la lectura a bajo nivel del blockchain 
                    // no facilitar� en puntero real hacia ning�n bloque para que pueda ser atacado ilegalmente.

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
                printf("Opci�n incorrecta.\n");
                break;
            }
        }
        if (nOpcion == 12)
        {
            printf("Cerramos la aplicaci�n.\n\n");
            break;
        }
    }
    return 0;
}

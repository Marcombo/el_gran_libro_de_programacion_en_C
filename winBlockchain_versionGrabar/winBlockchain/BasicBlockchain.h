//////////////////////////////////////////////////////////////////////////////
// Proyecto: BasicBlockchain
// Archivo : BasicBlockchain.h
// Copyright 2023, 2024- Marcombo - Boixareu Editores, Barcelona
// Copyright 2023, 2024- Alfons Gonzalez Perez
// 
//////////////////////////////////////////////////////////////////////////////

#pragma once

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

#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <conio.h>


#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false;
#endif

#ifndef BOOL
#define BOOL bool
#endif

//////////////////////////////////////////////////////////////////////////////
// Para detectar MS Visual C++, para que no de warnings por usar 
// funciones clásicas de tipo string:
//////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER 
#ifndef __llvm__
#ifndef __INTEL_COMPILER
#pragma warning(disable:4996)
#endif
#endif
#endif

//////////////////////////////////////////////////////////////////////////////
// Declaraciones de tipos BLOCK y LPBLOCK (puntero a BLOCK)
//////////////////////////////////////////////////////////////////////////////
typedef struct block
{
    unsigned char prevHash[SHA256_DIGEST_LENGTH];
    int blockData;
    struct block* pSiguienteBloque;
}BLOCK;

typedef BLOCK* LPBLOCK;

//////////////////////////////////////////////////////////////////////////////
// Declaraciones forward de las funciones para trabajar con blockchain:
//////////////////////////////////////////////////////////////////////////////

//Funciones para trabajar con Blockchain en memoria RAM dinámica (heap):
unsigned char* toString(BLOCK b);
void Pause();
void ConcatenarNuevoBloque(LPBLOCK& pHead, int nData);
void ImprimirHash(unsigned char hash[], size_t length);
BOOL VerificarBlockchain(LPBLOCK pHead);
BOOL FalsificarBloque(LPBLOCK pHead, int nOrdinal, int newData);
void ImprimirHash(unsigned char hash[], size_t length);
BOOL CompararHash(unsigned char* str1, unsigned char* str2);
void ImprimirLineaBloque(BLOCK* pBloque);
void ListadoBloques(LPBLOCK pHead);
int  ContarBloques(LPBLOCK pHead);
BOOL ImprimirLineaBloqueHash(LPBLOCK pHead, int nBlockNumber);
void ImprimirSoloBloquesFalsos(LPBLOCK pHead);
void ReconstruirBlockchain(LPBLOCK pHead);
void LiberarUltimoBloque(LPBLOCK& pHead);
int  LiberarBlockchain(LPBLOCK& pHead);

//Rutinas de movimiento para recorrer la cadena:
BLOCK * MoveFirst(BLOCK * pHead);
BLOCK * MoveLast(BLOCK* pHead);
BLOCK * MoveToIndex(BLOCK* pHear, int nIndex);

//Funciones para serializar (escribir y leer) la Blockchain en fichero:
BOOL ExistsFile(char* lpszFileName);
BOOL CrearFicheroBlockchain(char* lpszFileName);
int ContarBloquesDesdeFichero(char* lpszFileName);
BOOL LeerBloque(char* lpszFileName, int nRegistro, LPBLOCK pBloque);
int LeerCadenaBloquesEnteraDesdeFichero(char* lpszFileName, //Nombre del fichero que vamos a leer
    int* pnNumBloques, //Número de bloques leídos al final (el último leído)
    LPBLOCK* ppHeaderBlock); //puntero a puntero a cabecera de cadena Blockchain
BOOL EscribirBloque(char* lpszFileName, LPBLOCK pBloque);
int EscribirCadenaBloquesEnteraHaciaFichero(char* lpszFileName, //Nombre del fichero que vamos a leer
    int* pnNumBloques, //Número de bloques escritos al final (el último bloque escrito)
    LPBLOCK* ppHeaderBlock); //puntero a puntero a cabecera de cadena Blockchain





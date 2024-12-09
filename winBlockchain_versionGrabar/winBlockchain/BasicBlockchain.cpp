//////////////////////////////////////////////////////////////////////////////
// Proyecto: BasicBlockchain
// Archivo : BasicBlockchain.cpp
// Copyright 2023, 2024- Marcombo - Boixareu Editores, Barcelona
// Copyright 2023, 2024- Alfons Gonzalez Perez
// 
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

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

#include "BasicBlockchain.h"

//////////////////////////////////////////////////////////////////////////////
// Función: toString
// 
// Objetivo:
// Convertie una masa de memoria binaria en una tira de caracteres 
// alfanuméricos de 8 bits declarados como unsigned char *
// 
// Sintaxis:
// unsigned char* toString(BLOCK b)
// 
// Parámetros:
// Usa el parámetro b de tipo BLOCK que contiene un struct con la definición 
// de un bloque básico de Blockchain.
// 
// Valor de retorno:
// Un puntero a cadena de caracteres sin signo (unsigned char *) cuyo 
// contenido ha sido reservado en el heap mediante malloc.
// Por tanto, todo aquello que se reserva con la función malloc, se debe 
// liberar con free para evitar memory leakings. Dicha liberación posterior 
// debería realizarse desde la función que llame a toString().
// 
// Comentarios:
// Usamos el truco de no declarar variables estáticas para crear variables 
// de tipo carácter dentro de la función, sino declarar punteros a una 
// masa de memoria que no se encuentra dentro de la función y por tanto, 
// no se destruye al salir de ella.
// Se dice que el compilador se "asusta" cuando se declara una variable 
// de tipo puntero para ser retornada por la función.
// En este caso, para devolver un puntero a unsigned char * que ha sido 
// cargado mediante un malloc en el heap, con lo cual podemos devolver 
// el puntero al final de la función sin tener que liberar el contenido 
// una variable local propia.
// 
// Los compiladores como MinGW generan código assembler que, directamente, 
// retorna un valor 0 en el registro EAX cuando se le dice que retorne 
// un char[], ya que asume que al destruirse la función se va a destruir 
// implícitamente el string declarado en la función y, por tanto, el 
// puntero resultará inútil.
// 
// Al devolver el 0, MinGW pretende "avisar" de que el puntero es nulo 
// en vez de suministrar un puntero a una posición arbitraria de memoria 
// que, una vez destruída, no puede garantizar su contenido.
// 
// En cambio, si declaramos un puntero a "algo", el compilador se "asusta"
// y lo coloca en el registro EAX del procesador para ser devuelto a la 
// función llamadora.
// Garantizamos el contenido de la cadena al ser ésta reservada mediante 
// la función malloc en el heap, fuera de la función, pero pagamos el precio 
// de que tenemos que liberar dicha memroia con free(pPuntero) por nuestra 
// cuenta desde la función llamadora.
// 
//////////////////////////////////////////////////////////////////////////////

unsigned char* toString(BLOCK b)
{
    // Usamos el truco de asustar al compilador para devolver 
    // un puntero a unsigned char * que ha sido cargado 
    // mediante un malloc en el heap, con lo cual podemos 
    // devolver el puntero al final de la función sin tener que
    // liberar el contenido una variable local propia.
    unsigned char* lpszBuffer = NULL;
    int nLength = 0;

    nLength = (sizeof(b) + 1) * sizeof(unsigned char);
    lpszBuffer = (unsigned char*)malloc(nLength);
    if (lpszBuffer != NULL)
    {
        memset(lpszBuffer, 0, sizeof(b));
        memcpy(lpszBuffer, &b, sizeof(b));
    }
    return lpszBuffer;
}

//////////////////////////////////////////////////////////////////////////////
//
// La función Pause es la típica de "Pulse intro para continuar".
//
//////////////////////////////////////////////////////////////////////////////

void Pause()
{
    char Ch;
    
    printf("Pulse Intro para continuar...");
    while (!kbhit())
    {
        //Do nothing
    }
    Ch = getchar();
}

//////////////////////////////////////////////////////////////////////////////
// Función: ImprimirHash
// 
// Objetivo:
// Imprimir por pantalla un hash como una cadena visible
// 
// Sintaxis:
// void ImprimirHash(unsigned char hash[], size_t length)
// 
// Parámetros:
// 1.- unsigned char hash[]: matiz de caracteres sin signo.
// 2.- size_t length: longitud del hash
// 
// Valor de retorno: 
// No hay. Retorna void.
// 
// Comentarios:
// Para hacer visibles los caracteres, los toma como enteros sin signo y 
// los saca por pantalla, byte a byte, en formato hexadecimal.
// 
//////////////////////////////////////////////////////////////////////////////

void ImprimirHash(unsigned char hash[], size_t length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%02x", hash[i]);
    }
}

//////////////////////////////////////////////////////////////////////////////
// Función: CompararHash
// 
// Objetivo: 
// Comparar dos hashes byte a byte. Si el resultado es idéntico retorna TRUE.
// En caso de no ser iguales, retorna FALSE.
// 
// Sintaxis:
// BOOL CompararHash(unsigned char* lpszHash1, unsigned char* lpszHash2)
// 
// Parámetros:
// 1.- unsigned char* lpszHash1: puntero al hash1
// 2.- unsigned char* lpszHash2: puntero al hash2
// 
// Valor de retorno:
//   - TRUE si ambos hashes son iguales.
//   - FALSE si son diferentes.
// 
// Comentarios:
// Se sobreentiende que los hashes van a ser siempre de 
// tipo SHA256_DIGEST_LENGTH
// 
//////////////////////////////////////////////////////////////////////////////

BOOL CompararHash(unsigned char* lpszHash1, unsigned char* lpszHash2)
{
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        if (lpszHash1[i] != lpszHash2[i])
        {
            return FALSE;
        }
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// Función: ImprimirLineaBloque
// 
// Objetivo:
// Imprimir de forma visible un bloque de Blockchain. Se trata de una función 
// auxiliar de la función ListadoBloques.
// 
// Sintaxis:
// void ImprimirLineaBloque(BLOCK * pBloque)
// 
// Parámetros:
// - BLOCK * pBloque: es un puntero al bloque que hay que imprimir en una 
//    línea.
// 
// Valor de retorno:
// No tiene. Retorna void.
// 
// Comentarios:
// Esta función llama a su vez a la función ImprimirHash, de forma que el 
// hash se vuelve visible al ser impreso byte a byte en hexadecimal.
// 
//////////////////////////////////////////////////////////////////////////////

void ImprimirLineaBloque(BLOCK* pBloque)
{
    printf("%p\t", pBloque);
    ImprimirHash(pBloque->prevHash, sizeof(pBloque->prevHash));
    printf("\t[%d]\t", pBloque->blockData);
    printf("%p\n", pBloque->pSiguienteBloque);
}

//////////////////////////////////////////////////////////////////////////////
// Función: ListadoBloques
// 
// Objetivo:
// Listar todos los bloques que componen un Blockchain de forma legible.
// 
// Sintaxis:
// void ListadoBloques(LPBLOCK pHead)
// 
// Parámetros:
//   1.- LPBLOCK pHead: puntero hacia el Blockchain representado por la cabeza
//       de la lista dinámica de bloques.
// 
// Comentarios:
// La cadena de bloques o Blockchain es, de hecho, una lista dinámica en la 
// cual cada bloque apunta al siguiente mediante un puntero, hasta encontrar 
// el último elemento, que es el que apunta a NULL.
// Cuando el Blockchain (la lista dinámica) no contiene bloque alguno, el 
// puntero pHead es NULL.
// 
//////////////////////////////////////////////////////////////////////////////

void ListadoBloques(LPBLOCK pHead)
{
    LPBLOCK curr = pHead;
    if (pHead == NULL)
    {
        printf("El Blockchain está vacio. Necesita dar de alta nuevos bloques...\n");
        return;
    }

    printf("Puntero                 Bloque encriptado                                                       [Dato]  Puntero al sig. \n");
    printf("----------------------  ----------------------------------------------------------------        ------- ----------------\n");
    while (curr)
    {
        ImprimirLineaBloque(curr);
        curr = curr->pSiguienteBloque;
    }
}

//////////////////////////////////////////////////////////////////////////////
// Función: ContarBloques
// 
// Objetivo:
// Contar el número de bloques existente dentro del Blockchain.
// 
// Sintaxis:
// int ContarBloques(LPBLOCK pHead)
// 
// Parámetros:
//   1.- LPBLOCK pHead: puntero hacia el Blockchain representado por la cabeza
//       de la lista dinámica de bloques.
// 
// Valor de retorno:
// Un entero con el número de bloques existente en el Blockchain.
// 
// Comentarios:
// La cadena de bloques o Blockchain es, de hecho, una lista dinámica en la 
// cual cada bloque apunta al siguiente mediante un puntero, hasta encontrar 
// el último elemento, que es el que apunta a NULL.
// Cuando el Blockchain (la lista dinámica) no contiene bloque alguno, el 
// puntero pHead es NULL.
// 
//////////////////////////////////////////////////////////////////////////////

int ContarBloques(LPBLOCK pHead)
{
    LPBLOCK curr = pHead;
    int nCount = 0;
    while (curr)
    {
        nCount++;
        curr = curr->pSiguienteBloque;
    }
    return nCount;
}

//////////////////////////////////////////////////////////////////////////////
// Función: ConcatenarNuevoBloque
// 
// Objetivo:
// Añade un nuevo bloque al final del Blockchain
// 
// Sintaxis:
// void ConcatenarNuevoBloque(LPBLOCK& pHead, int Data)
// 
// Parámetros:
//   1.- LPBLOCK& pHead: puntero a la cabeza de la lista del Blockchain
//   2.- int nData: Dato numérico que se guardará en el bloque
// 
// Comentarios:
// Añade un nuevo bloque a un blockchain.
//   - Si el blockchain no tiene elementos, esto es, si el puntero a la 
//     cabecera de la lista es NULL, entonces:
//       - Crea un elemento sub cero como cabecera de lista nueva. 
//       - Calcula el hash del elemento sub cero de cabecera.
//         
//   - Tanto si el blockchain no tenía elemento inicial como si el blockchain ya tiene al menos un elemento:
//       - Empieza desde el principio del Blockchain hasta llegar al bloque final.
//       - A este bloque final lo llama pCurrentBlock.
//       - Crea un nuevo bloque  al que llama pNewBlock (si es el primero, será el bloque sub 1 y si no será el bloque sub N).
//       - Usa el puntero devuelto por el malloc y lo coloca como puntero desde el elemento current->pSiguienteBloque para enlazar con el nuevo bloque.
//       - Coloca el valor Data dentro del bloque del nuevo bloque pNewBlock.
//       - Calcula el HASH anterior del pCurrentBlock y lo coloca dentro 
//         del bloque pNewBlock, dentro de la variable pNewBlock->prevHash 
//         de modo que el nuevo bloque "se encabalga" sobre el anterior 
//         mediante el hash del anterior.
//       - Pone el puntero hacia el futuro elemento siguiente a NULL, 
//         indicando que aquí se acaba la lista del Blockchain.
//  
//////////////////////////////////////////////////////////////////////////////

void ConcatenarNuevoBloque(LPBLOCK& pHead, int Data)
{
    unsigned char* pToString = NULL;
    if (pHead == NULL)
    {
        pHead = (BLOCK*)malloc(sizeof(struct block));
        SHA256((unsigned char*)"", sizeof(""), pHead->prevHash);
        pHead->blockData = Data;
        pHead->pSiguienteBloque = NULL;
        return;
    }
    BLOCK* currentBlock = pHead;
    while (currentBlock->pSiguienteBloque)
    {
        currentBlock = currentBlock->pSiguienteBloque;
    }
    LPBLOCK pNewBlock = (LPBLOCK)malloc(sizeof(BLOCK));
    currentBlock->pSiguienteBloque = pNewBlock;
    pNewBlock->blockData = Data;
    pToString = toString(*currentBlock);
    SHA256(pToString, sizeof(*currentBlock), pNewBlock->prevHash);
    pNewBlock->pSiguienteBloque = NULL;
    free(pToString);
}

//////////////////////////////////////////////////////////////////////////////
// Función: VerificarBlockchain
// 
// Objetivo:
// Verifica la coherencia de todos los eslabones del Blockchain, verificando 
// que coinciden el hash previo con el hash guardado como prevHash del 
// siguiente bloque.
// Imprime un listado en el que se ve en cada línea correspondiente de bloque:
//   1.- El ordinal del bloque (1=primero, 2=segundo...).
//   2.- El valor del dato almacenado en el bloque.
//   3.- El hash del bloque previo.
//   4.- Un guión de separación.
//   5.- El hash (almacenado en el bloque actual) que correspondería con el 
//       hash del bloque previo.
//   6.- El estado de verificado OK o de ** no encaja **
// 
// En caso de no encajar, el descuadre se debe a la falsificación del bloque 
// previo al que aparece el error.
// 
// Sintaxis:
// BOOL VerificarBlockchain(LPBLOCK pHead)
// 
// Parámetros:
//   1.- LPBLOCK pHead es el puntero a la cabecera del Blockchain.
// 
// Valor devuelto:
//   - TRUE si el blockchain es completamente coherente.
//   - FALSE si ha fallado.
// 
// Comentarios:
// Si los hashes no coinciden, ese error se detecta en el bloque siguiente 
// del que ha sido falsificado.
// 
//////////////////////////////////////////////////////////////////////////////

BOOL VerificarBlockchain(LPBLOCK pHead)
{
    BOOL bOK = TRUE;
    unsigned char* pToString = NULL;

    if (pHead == NULL)
    {
        printf("El Blockchain está vacio. Necesita dar de alta nuevos bloques...\n");
        return 0;
    }

    LPBLOCK pBloqueEnCurso = pHead->pSiguienteBloque;
    LPBLOCK pBloqueAnterior = pHead;

    printf("Ordinal [Dato]  Hash del bloque anterior                   Hash del bloque actual                   Estado           \n");
    printf("------- ------  ----------------------------------------   ---------------------------------------- -----------------\n");

    int count = 1;
    while (pBloqueEnCurso)
    {
        printf("%d\t[%d]\t", count++, pBloqueEnCurso->blockData);

        pToString = toString(*pBloqueAnterior);
        ImprimirHash(SHA256(pToString, sizeof(*pBloqueAnterior), NULL), SHA_DIGEST_LENGTH);
        free(pToString);

        printf(" - ");
        ImprimirHash(pBloqueEnCurso->prevHash, SHA_DIGEST_LENGTH);
        pToString = toString(*pBloqueAnterior);
        if (CompararHash(SHA256(pToString, sizeof(*pBloqueAnterior), NULL), pBloqueEnCurso->prevHash))
        {
            printf(" Verificado OK\n");
        }
        else
        {
            printf(" ** no encaja **\n");
            bOK = FALSE;
        }
        free(pToString);

        pBloqueAnterior = pBloqueEnCurso;
        pBloqueEnCurso = pBloqueEnCurso->pSiguienteBloque;
    }
    return bOK;
}

//////////////////////////////////////////////////////////////////////////////
// Función: FalsificarBloque
// 
// Objetivo:
// Proporcionar una "función malware" que sirve para falsificar un bloque y 
// ver qué sucede con el blockchain. Evidentemente, esta función no está 
// disponible para los usuarios externos.
// 
// Sintaxis:
// BOOL FalsificarBloque(LPBLOCK pHead, int nOrdinal, int nDatoFalso)
// 
// Parámetros:
//   1.- LPBLOCK pHead: puntero a la cabecera del Blockchain.
//   2.- int nOrdinal: número de índice (ordinal) basado en 1 que se quiere 
//       seleccionar para modificar.
//   3.- int Data: valor falso que machaca el valor auténtico anterior del 
//       bloque seleccionado.
// 
// Valor de retorno:
//   - TRUE si ha ido bien la falsificación.
//   - FALSE si el Blockchain está vacío o si se ha intentado falsificar un bloque que no existe.
// 
// Comentarios:
// Imprime en pantalla el valor Data anterior y el valor Data falsificado.
// Evidentemente, esta función no está disponible para los usuarios externos.
// 
//////////////////////////////////////////////////////////////////////////////

BOOL FalsificarBloque(LPBLOCK pHead, int nOrdinal, int nDatoFalso)
{
    int count = 1;
    LPBLOCK pBloqueEnCurso = nullptr;

    pBloqueEnCurso = pHead;

    if (pBloqueEnCurso == NULL)
    {
        printf("El bloque %d no existe!\n", nOrdinal);
        return 0;
    }
    while (count != nOrdinal)
    {
        if (pBloqueEnCurso->pSiguienteBloque == NULL && count != nOrdinal)
        {
            printf("El bloque %d no existe!\n", nOrdinal);
            return 0;
        }
        else
        {
            if (count == nOrdinal)
            {
                break;
            }
            pBloqueEnCurso = pBloqueEnCurso->pSiguienteBloque;
            count++;
        }
    }
    printf("Antes:\n");
    ImprimirLineaBloque(pBloqueEnCurso);
    printf("\n");

    pBloqueEnCurso->blockData = nDatoFalso;

    printf("\nAhora:\n");
    ImprimirLineaBloque(pBloqueEnCurso);
    printf("\n");

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// Función: ImprimirLineaBloqueHash
// 
// Objetivo:
// Función auxiliar que imprime cada bloque mediante la función 
// ImprimirSoloBloquesFalsos.
// 
// Sintaxis:
// BOOL ImprimirLineaBloqueHash(LPBLOCK pHead, int nBlockNumber)
// 
// Parámetros:
//   1.- LPBLOCK pHead: Puntero a la cabecera del blockchain.
//   2.- int nBlockNumber: número de bloque que se desea imprimir.
// 
// Valor devuelto:
//   - TRUE si el registro existe.
//   - FALSE si el blockchain está vacío o si el número de bloque (ordinal) 
//     no existe dentro del hash. Por ejemplo, si tenemos 10 bloques y nos 
//     piden que imprimamoe el bloque 15.
// 
// Comentarios:
// 
//////////////////////////////////////////////////////////////////////////////

BOOL ImprimirLineaBloqueHash(LPBLOCK pHead, int nBlockNumber)
{
    int count = 1;
    unsigned char* pToString = NULL;
    unsigned char szHash[SHA256_DIGEST_LENGTH];
    LPBLOCK pBloqueEnCurso = nullptr;

    pBloqueEnCurso = pHead;

    memset(szHash, 0, sizeof(szHash));

    if (pBloqueEnCurso == NULL)
    {
        printf("El bloque %d no existe!\n", nBlockNumber);
        return 0;
    }
    while (count != nBlockNumber)
    {
        if (pBloqueEnCurso->pSiguienteBloque == NULL && count != nBlockNumber)
        {
            printf("El bloque %d no existe!\n", nBlockNumber);
            return 0;
        }
        else
        {
            if (count == nBlockNumber)
            {
                break;
            }
            pBloqueEnCurso = pBloqueEnCurso->pSiguienteBloque;
            count++;
        }
    }
    printf("Hash del bloque num. %d:", nBlockNumber);
    pToString = toString(*pBloqueEnCurso);
    SHA256(pToString, sizeof(*pBloqueEnCurso), szHash);
    free(pToString);
    ImprimirHash(szHash, SHA256_DIGEST_LENGTH);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// Función: ImprimirSoloBloquesFalsos
// 
// Objetivo:
// Proporciona un listado solamente de los hashes falsificados.
// 
// Sintaxis:
// void ImprimirSoloBloquesFalsos(LPBLOCK pHead)
// 
// Parámetros:
//   1.- LPBLOCK pHead: puntero a la cabecera del blockchain.
// 
// Valor de retorno:
//   Ninguno. Retorna void.
// 
// Comentarios:
// Esta función utiliza a su vez a la función auxiliar ImprimirLineaBloqueHash
// para verificar los hashes.
//////////////////////////////////////////////////////////////////////////////

void ImprimirSoloBloquesFalsos(LPBLOCK pHead)
{
    BOOL bCoinciden = FALSE;
    int nFalsos = 0;
    unsigned char* pToString = NULL;
    if (pHead == NULL)
    {
        printf("Blockchain is empty. Try again after adding some blocks...\n");
        return;
    }
    LPBLOCK pBloqueEnCurso = pHead->pSiguienteBloque;
    LPBLOCK pBloqueAnterior = pHead;

    printf("Ordinal [Dato]  Hash falsificado                           Estado            \n");
    printf("------- ------  ----------------------------------------   ----------------- \n");

    int count = 1;
    while (pBloqueEnCurso)
    {
        pToString = toString(*pBloqueAnterior);
        bCoinciden = CompararHash(SHA256(pToString, sizeof(*pBloqueAnterior), NULL), pBloqueEnCurso->prevHash);
        free(pToString);
        if (!bCoinciden)
        {
            printf("%d\t[%d]\t", count, pBloqueAnterior->blockData);

            pToString = toString(*pBloqueAnterior);
            ImprimirHash(SHA256(pToString, sizeof(*pBloqueAnterior), NULL), SHA_DIGEST_LENGTH);
            free(pToString);

            printf(" - Hash falso\n");
            nFalsos++;
        }
        count++;
        pBloqueAnterior = pBloqueEnCurso;
        pBloqueEnCurso = pBloqueEnCurso->pSiguienteBloque;
    }
    if (!nFalsos)
    {
        printf("No hay bloques falsos.\n");
    }
    else
    {
        printf("Hay %d bloques falsos.\n", nFalsos);
    }
}

//////////////////////////////////////////////////////////////////////////////
// Función: ReconstruirBlockchain
// 
// Objetivo: Ver cómo sería una función hipotética de hackeo que recalculara 
// los hashes de toda la cadena de bloques, sean estos coherentes o 
// falsificados y los daría todos por buenos.
// 
// Sintaxis:
// void ReconstruirBlockchain(LPBLOCK pHead)
// 
// Parámetros:
//   1.- LPBLOCK pHead: puntero a la cabecera del blockchain.
// 
// Valor de retorno:
//   Ninguno. Retorna void.
// 
// Comentarios:
// Evidentemente, esta función no está disponible para los usuarios externos.
// 
//////////////////////////////////////////////////////////////////////////////

void ReconstruirBlockchain(LPBLOCK pHead)
{
    unsigned char* pToString = NULL;
    BOOL bCoinciden = FALSE;

    LPBLOCK pBloqueEnCurso = pHead;
    LPBLOCK pBloqueAnterior;

    if (pBloqueEnCurso == NULL)
    {
        printf("El Blockchain está vacío!");
        return;
    }
    while (TRUE)
    {
        pBloqueAnterior = pBloqueEnCurso;
        pBloqueEnCurso = pBloqueEnCurso->pSiguienteBloque;
        if (pBloqueEnCurso == NULL)
        {
            return;
        }

        pToString = toString(*pBloqueAnterior);
        bCoinciden = CompararHash(SHA256(toString(*pBloqueAnterior), sizeof(pBloqueAnterior), NULL), pBloqueEnCurso->prevHash);
        free(pToString);

        if (!bCoinciden)
        {
            pToString = toString(*pBloqueAnterior);
            ImprimirHash(SHA256(pToString, sizeof(*pBloqueAnterior), pBloqueEnCurso->prevHash), SHA256_DIGEST_LENGTH);
            free(pToString);
            printf("\n");
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// Función: LiberarUltimoBloque
// 
// Objetivo:
// Saca y libera la memoria del último elemento de la cola de bloques del 
// blockchain. Se utiliza como función auxiliar de la función 
// LiberarBlockchain, que se encarga de limpiar la memoria de la lista de 
// los elementos del Blockchain después de haber trabajado con él, al salir 
// de programa. Esta es, pues, una función auxiliar de eliminación de bloques
// individuales llamada desde una función de eliminación total en memoria,
// peldaño a peldaño.
// 
// Sintaxis:
// void LiberarUltimoBloque(LPBLOCK& pHead)
// 
// Parámetros:
//   1.- LPBLOCK& pHead: Puntero a la cabecera del Blockchain.
// 
// Valor devuelto:
// Ninguno. Retorna void.
// 
// Comentarios:
// Cuando el último bloque liberado da 0 elementos en la lista, 
// se pone el valor del puntero de pHead a NULL y se pasa por referencia
// eliminando todo el Blockchain.
// Por esa razón el parámetro se pasa como LPBLOCK& pHead, 
// porque se quiere afectar al puntero a blockchain 
// que está fuera de la función.
// 
//////////////////////////////////////////////////////////////////////////////

void LiberarUltimoBloque(LPBLOCK& pHead)
{
    LPBLOCK pBloqueEnCurso = pHead;
    LPBLOCK pBloqueAnterior;

    if (pBloqueEnCurso == NULL)
    {
        return;
    }
    while (TRUE)
    {
        pBloqueAnterior = pBloqueEnCurso;
        pBloqueEnCurso = pBloqueEnCurso->pSiguienteBloque;
        if (pBloqueEnCurso != NULL)
        {
            if (pBloqueEnCurso->pSiguienteBloque == NULL)
            {
                //Hemos encontrado el último elemento:
                //Borramos el bloque en curso:
                free(pBloqueEnCurso);

                //El bloque anterior debe apuntar a NULL:
                pBloqueAnterior->pSiguienteBloque = NULL;
                return;
            }
        }
        else
        {
            // Sólo queda el bloque 0, o sea el pBloqueAnterior, cuya
            // dirección coincide con el valor de la dirección de pHead 
            // Lo liberamos y ponemos el pHead igual a NULL.
            free(pBloqueAnterior);
            pHead = NULL;
            return;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// Función: LiberarBlockchain
// 
// Objetivo:
// Liberar toda la memoria usada por la totalidad de bloques del Blockchain.
// 
// Sintaxis:
// int LiberarBlockchain(LPBLOCK& pHead)
// 
// Parámetros:
//   1.- LPBLOCK& pHead
// 
// Comentarios:
// Llama a la función auxiliar LiberarUltimoBloque para ir peldando a peldaño,
// quitando desde el último bloque hasta el primero.
// 
// Valor devuelto:
// Ninguno. Retorna void.
// 
// Comentarios:
// Cuando el último bloque liberado da 0 elementos en la lista, 
// se pone el valor del puntero de pHead a NULL y se pasa por referencia
// eliminando todo el Blockchain.
// 
// Por esa razón el parámetro se pasa como LPBLOCK& pHead, 
// porque se quiere afectar al puntero a blockchain 
// que está fuera de la función.
// 
// El algoritmo de esta función podría haberse hecho recursivo,
// pero en este punto del libro aún no hemos explicado las 
// funciones recursivas ni las estructuras de datos recursivas, 
// como lo son las listas.
// 
//////////////////////////////////////////////////////////////////////////////

int LiberarBlockchain(LPBLOCK& pHead)
{
    int nCount = 0;

    if (pHead == NULL)
    {
        return 0;
    }

    while (TRUE)
    {
        nCount = ContarBloques(pHead);
        if (nCount == 0)
        {
            break;
        }
        LiberarUltimoBloque(pHead);
    }
    // Comprobar que la cabeza de lista haya sido liberada.
    // Si no ha sido liberada, la liberamos y después
    // ponemos el puntero a pHead a NULL, 
    // afectando al valor del puntero fuera de la función:
    if (pHead)
    {
        free(pHead);
        pHead = NULL;
    }
    return TRUE;
}

BLOCK* MoveFirst(BLOCK* pHead)
{
    BLOCK* pFoundBlock = NULL;
    if (pHead)
    {
        if (pHead->pSiguienteBloque!=NULL)
        {
            pFoundBlock = pHead->pSiguienteBloque;
        }
    }
    return pFoundBlock;
}

BLOCK* MoveLast(BLOCK* pHead)
{
    LPBLOCK pCurr = pHead;
    int nCount = 0;
    while (pCurr->pSiguienteBloque!=NULL)
    {
        nCount++;
        pCurr = pCurr->pSiguienteBloque;
    }
    //Si no hay elementos, el primero y el último se toman como NULL
    if (nCount == 0)
    {
        pCurr = NULL;
    }
    return pCurr;
}

// Se le suministra como parámetro no el pBlockchainHeader (la cabecera absoluta)
// sino el puntero al bloque desde el cual se parte.
// Se trata del concelto recursivo que afirma que:
// 1.- Una lista es una lista de listas:
// 2.- Toda lista se compone de una cabeza (primer elemento) y una cola (la tira de bloques restantes)

BLOCK* MoveNext(BLOCK* pHeadCursor)
{
    BLOCK* pCurrentBlock = NULL;

    if (pHeadCursor)
    {
        if (pHeadCursor->pSiguienteBloque)
        {
            pCurrentBlock = pHeadCursor->pSiguienteBloque;
        }
    }
    return pCurrentBlock;
}

BLOCK* MoveToIndex(BLOCK* pHead, int nIndex)
{
    int nElementos = 0;
    int nContador = 0;
    BLOCK * pCurrentBlock = NULL;

    if (nIndex > 0)
    {
        if (pHead)
        {
            if (pHead->pSiguienteBloque)
            {
                nElementos = ContarBloques(pHead);
            }
            //No se admiten númerods de índice negativos
            if (nIndex > 0) 
            {
                //Si el índice es igual o menor al numero máximo de elementos
                if (nIndex <= nElementos) 
                {
                    pCurrentBlock = pHead;
                    while (pCurrentBlock->pSiguienteBloque != NULL && nContador <= nIndex)
                    {
                        nContador++;
                    }
                }
            }
            return pCurrentBlock;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// Comprueba si existe un fichero determinado.
// El sistema consiste en intentar abrirlo en modo lectura.
// Si consigue abrirlo, devuelde un TRUE y cierra el fichero.
// Si no puede abrirlo, devuelve un FALSE
//
BOOL ExistsFile(char* lpszFileName)
{
    BOOL bExists = FALSE;
    FILE* pFile = NULL;

    pFile = fopen(lpszFileName, "r");
    if (pFile)
    {
        bExists = TRUE;
        fclose(pFile);
    }
    return bExists;
}

//////////////////////////////////////////////////////////////////////////////
// Crea un fichero blockchain comenzando por un bloque 0.
//
BOOL CrearFicheroBlockchain(char* lpszFileName)
{
    BOOL bOKCreado = FALSE;
    FILE* pFile = NULL;
    BLOCK zeroBlock;

    pFile = fopen(lpszFileName, "wb+");
    if (pFile)
    {
        memset(&zeroBlock, 0, sizeof(BLOCK));
        fwrite(&zeroBlock, sizeof(BLOCK), 1, pFile);
        fclose(pFile);
        bOKCreado = TRUE;
    }
    return bOKCreado;
}

//////////////////////////////////////////////////////////////////////////////
// Cuenta el número de bloques que hay en el fichero de blockchain
// El mecanismo de conteo consiste en posicionarse al final del fichero
// y divide el número de bytes por la longitud en bytes de un bloque.
// Finalmente le resta 1 para descontar el bloque 0.
//
int ContarBloquesDesdeFichero(char* lpszFileName)
{
    int nBloques = 0;
    FILE* pFile = NULL;
    size_t nTamFichero = 0;
    int nNumBloques = 0;
    int nError = 0;

    pFile = fopen(lpszFileName, "r+");
    if (pFile)
    {
        nError = fseek(pFile, 0, SEEK_END);
        if (!nError)
        {
            nTamFichero = ftell(pFile);
            nNumBloques = (int)(nTamFichero / sizeof(BLOCK)) - 1;
        }
        fclose(pFile);
    }
    return nNumBloques;
}

//////////////////////////////////////////////////////////////////////////////
// LeerBloque lee un bloque desde el fichero Blockchain especificado.
// Crea dinámicamente un nuevo bloque, que devuelve un puntero a puntero de bloque
// Si el registro es el número 1, libera previamente la blockchain existente 
// en RAM y luego pasa el puntero obtenido de la lectura hacia la variable 
// global pBlockChainHeader que señala la cabecera del Blockchain en RAM.
//
BOOL LeerBloque(char* lpszFileName, int nRegistro, LPBLOCK pBloque)
{
    BOOL bOK = FALSE;
    FILE* pFile = NULL;
    int nBlocksRead = 0;
    int nBloquesLiberados = 0;
    int nPos = 0;
    int nError = 0;

    pFile = fopen(lpszFileName, "r+");
    if (pFile)
    {
        memset(pBloque, 0, sizeof(BLOCK));
        nError = fseek(pFile, nRegistro * sizeof(BLOCK), SEEK_SET);
        if (!nError)
        {
            nPos = ftell(pFile);
            nBlocksRead = (int)fread(pBloque, sizeof(BLOCK), 1, pFile);
            bOK = (nBlocksRead > 0) ? TRUE : FALSE;
        }
        fclose(pFile);
    }
    return bOK;
}

//////////////////////////////////////////////////////////////////////////////
// LeerCadenaBloquesEnteraDesdeFichero lee toda una cadena blockchain desde
// el fichero que se le proporciona.
// Parámetros:
//  1.- Nombre, incluyendo ruta, del fichero blockchain (param. de entrada).
//  2.- Número de bloques leídos al final de la cadena (param. de salida).
//  3.- Puntero hacia puntero  apuntando a la cabecera del blockchain 
//      (tanto de entrada como de salida).
// El tercer parámetro recibe el puntero a la antigua cadena de blockchain 
// (si la hubiera), la libera entera y crea el puntero a la cabecera de la 
// nueva cadena para poder devolver dicho puntero mediante la técnica (o truco) 
// del puntero al puntero de la cabecera.
// De este modo, el puntero suministrado a la cabecera de la antigua Blockchain 
// queda sustituido por el puntero a la cabecera de la nueva Blockchain.
// Valor de retorno:
// ----------------
// Número de error:
// 0 si todo va bien:
// 1 No existe el fichero especificado
// 2 No se ha podido leer un bloque:
//

int LeerCadenaBloquesEnteraDesdeFichero(char* lpszFileName, //Nombre del fichero que vamos a leer
    int* pnNumBloques, //Número de bloques leídos al final (el último leído)
    LPBLOCK* ppHeaderBlock) //puntero a puntero a cabecera de cadena Blockchain
{
    BLOCK* pOldBlockchainHeader = NULL;
    BLOCK* pNewBlockchainHeader = NULL;
    BLOCK* pGenericBlock = NULL;
    BLOCK* pPreviousBlock = NULL;
    int nNumBloques = 0;
    int nCursorRegistros = 0;
    BOOL bExisteFichero = FALSE;
    BOOL bOKRead = FALSE;
    int nError = 0;

    // Antes de nada, asegurémonos de que existe el fichero.
    // Así evitaremos liberar una cadena blockchain sin poder leer la siguiente.
    bExisteFichero = ExistsFile(lpszFileName);
    if (bExisteFichero)
    {
        // Continuando con las medidas de seguridad, asegurémonos de que 
        // hay bloques dentro del Blockchain antes de liberar la cadena anterior
        nNumBloques = ContarBloquesDesdeFichero(lpszFileName);
        if (nNumBloques > 0)
        {
            // Ahora sí que podemos liberar toda la cadena Blockchain actual, 
            // si la hubiera, a partir del puntero al bloque de la cabecera:
            pOldBlockchainHeader = *ppHeaderBlock;
            LiberarBlockchain(pOldBlockchainHeader);

            //Bucle de lectura de bloques:
            for (nCursorRegistros = 1; nCursorRegistros <= nNumBloques; nCursorRegistros++)
            {
                //Crear blockchain nueva en RAM:
                pGenericBlock = (LPBLOCK)malloc(sizeof(BLOCK));
                memset(pGenericBlock, 0, sizeof(BLOCK));

                bOKRead = LeerBloque(lpszFileName, nCursorRegistros, pGenericBlock);
                if (bOKRead)
                {
                    if (nCursorRegistros == 1)
                    {
                        pNewBlockchainHeader = pGenericBlock;
                        pPreviousBlock = pGenericBlock;
                    }
                    else
                    {
                        if (nCursorRegistros > 1)
                        {
                            // Hacemos que el bloque anterior apunte al bloque recién leído
                            // porque los punteros almacenados dentro del fichero no tienen
                            // sentido, ya que apuntan a posiciones de RAM que ya no son 
                            // las mismas:
                            pPreviousBlock->pSiguienteBloque = pGenericBlock;
                            pPreviousBlock = pGenericBlock;
                        }
                    }
                }
                else
                {
                    // Error: no se ha posido leer bloque:
                    nError = 2;
                    // Salimos del bucle, guardando el número del 
                    // último bloque intentado en nCursorRegistros.
                    // Una vez fuera del bucle for, el valor de nCursorRegistros 
                    // pasará a *pnNumBloques como parámetro de salida (por referencia).
                    break;
                }
            } // cierre del bucle for

            // Ponemos que el puntero al último elemento de la cadena es NULL, indicando que la cadena ha llegado a su cola final.
            // Ese sí que podría coincidir con el NULL grabado en el fichero, pero lo forzamos a NULL ara mayor seguridad:
            pPreviousBlock->pSiguienteBloque = NULL;

            // Devolvemos parámetros de salida, que son los dos finales:
            // Número de registros encontrados:
            *pnNumBloques = (nCursorRegistros <= nNumBloques) ? nCursorRegistros : nNumBloques;

            // Puntero a la cabecera de la nueva Blockchain:
            *ppHeaderBlock = pNewBlockchainHeader;
        }
    }
    else
    {
        // Error 1: el fichero especificado no existe:
        nError = 1;
    }
    return nError;
}

//////////////////////////////////////////////////////////////////////////////
// Escribe un bloque en el fichero Blockchain especificado.
// El sistema consiste en posicionarse en el último registro y añadir el 
// nuevo bloque al final.
//

BOOL EscribirBloque(char* lpszFileName, LPBLOCK pBloque)
{
    BOOL bOK = FALSE;
    BOOL bOKCreado = FALSE;
    FILE* pFile = NULL;
    int nError = 0;
    size_t nPosition = 0;
    int nNumBlocksWritten = 0;
    int nBloquesExistentes = 0;
    int nCursorRegistros = 0;

    // El nombre del fichero para almacenar el Blockchain se establece 
    // al principio de la función main (principal)
    bOKCreado = ExistsFile(lpszFileName);
    if (!bOKCreado)
    {
        bOKCreado = CrearFicheroBlockchain(lpszFileName);
    }
    if (bOKCreado)
    {
        nBloquesExistentes = ContarBloquesDesdeFichero(lpszFileName);
        pFile = fopen(lpszFileName, "r+");
        if (pFile)
        {
            nError = fseek(pFile, (nBloquesExistentes + 1) * sizeof(BLOCK), SEEK_SET);
            if (!nError)
            {
                nNumBlocksWritten = (int)fwrite(pBloque, sizeof(BLOCK), 1, pFile);
                bOK = (nNumBlocksWritten == 1) ? TRUE : FALSE;
            }
            fclose(pFile);
        }
    }
    return bOK;
}

//////////////////////////////////////////////////////////////////////////////
// La función EscribirCadenaBloquesEnteraHaciaFichero escribe la cadena 
// entera de bloques en un fichero.
//
// Sus parámetros son:
// - char* lpszFileName:     Nombre del fichero que vamos a escribir
// - int* pnNumBloques:      Número de bloques escritos al final (el 
//                           último bloque escrito)
// - LPBLOCK* ppHeaderBlock: Puntero a puntero a cabecera de cadena 
//                           de Blockchain. Se trata de un parámetro 
//                           de entrada que se envía de forma bastante 
//                           parecida al caso de la función 
//                           LeerCadenaBloquesEnteraDesdeFichero, 
//                           pero renunciando a modificar el puntero 
//                           como parámetro de salida. Sólo sirve como 
//                           parámetro de entrada para facilitar el 
//                           puntero hacia la cadena de bloques.
//

int EscribirCadenaBloquesEnteraHaciaFichero(char* lpszFileName, 
                                          int* pnNumBloques, 
                                          LPBLOCK* ppHeaderBlock) 
{
    int nError = 0;
    int nMaxBloques = 0;
    int nCursorBloques = 0;
    int nBlocksWritten = 0;
    BOOL bOKCreado = FALSE;
    BOOL bOKWritten = FALSE;
    LPBLOCK pHead = NULL;
    LPBLOCK pCurrentBlock;
    FILE* pFile = NULL;

    if (*ppHeaderBlock)
    {
        pHead = *ppHeaderBlock;
        bOKCreado = CrearFicheroBlockchain(lpszFileName);
        if (bOKCreado)
        {
            nMaxBloques = ContarBloques(pHead);
            if (nMaxBloques > 0)
            {
                pCurrentBlock = pHead;

                for (nCursorBloques = 1; nCursorBloques <= nMaxBloques && pCurrentBlock != NULL; nCursorBloques++)
                {
                    bOKWritten = EscribirBloque(lpszFileName, pCurrentBlock);
                    if (bOKWritten)
                    {
                        pCurrentBlock = pCurrentBlock->pSiguienteBloque;
                        nBlocksWritten++;
                    }
                    else
                    {
                        //No se ha podido escribir algún bloque
                        nError = 2; 
                        break;
                    }
                }
            }
        }
        else
        {
            //No se ha podido crear el fichero
            nError = 1;
        }
    }
    *pnNumBloques = nBlocksWritten;
    return nError;
}



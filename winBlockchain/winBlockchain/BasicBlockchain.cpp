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
        printf("El Blockchain está vacío. Necesita dar de alta nuevos bloques...\n");
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
        printf("El Blockchain está vacío. Necesita dar de alta nuevos bloques...\n");
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

BOOL LiberarUltimoBloque(LPBLOCK& pHead)
{
    LPBLOCK pBloqueEnCurso = pHead;
    LPBLOCK pBloqueAnterior;
    BOOL bBorrado = FALSE;

    if (pBloqueEnCurso == NULL)
    {
        printf("El Blockchain está vacío!");
        return bBorrado;
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
                bBorrado = TRUE;
                break;
            }
        }
        else
        {
            // Sólo queda el bloque 0, o sea el pBloqueAnterior, cuya
            // dirección coincide con el valor de la dirección de pHead 
            // Lo liberamos y ponemos el pHead igual a NULL.
            free(pBloqueAnterior);
            pHead = NULL;
            bBorrado = TRUE;
            break;
        }
    }//final de bucle while
    return bBorrado;
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
    int nBloquesLiberados = 0;
    BOOL bOKLiberado = FALSE;

    if (pHead == NULL)
    {
        printf("El Blockchain está vacío!");
        return FALSE;
    }

    while (TRUE)
    {
        nCount = ContarBloques(pHead);
        if (nCount == 0)
        {
            break;
        }
        bOKLiberado = LiberarUltimoBloque(pHead);
        if (bOKLiberado)
        {
            nBloquesLiberados++;
        }
    }
    // Comprobar que la cabeza de lista haya sido liberada.
    // Si no ha sido liberada, la liberamos y después
    // ponemos el puntero a pHead a NULL, 
    // afectando al valor del puntero fuera de la función:
    if (pHead)
    {
        free(pHead);
        pHead = NULL;
        nBloquesLiberados++;
    }
    return nBloquesLiberados;
}

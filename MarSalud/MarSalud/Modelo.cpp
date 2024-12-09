#include <iostream>
#include "MarSalud.h"

CModeloPtr BuscarModelo(CModeloPtr pListaModelo, char* lpszClassName)
{
    CModeloPtr pCursor = pListaModelo;
    CModeloPtr pFoundModelo = NULL;
    int nComp = 0;

    if (pListaModelo)
    {
        while (!pFoundModelo)
        {
            nComp = strcmp(pCursor->m_szRuntimeClassName, lpszClassName);
            if (!nComp)
            {
                //Encontrado: son iguales:
                pFoundModelo = pCursor;
                break;
            }
            pCursor = pCursor->m_pNext;
            if (!pCursor)
            {
                //Final de lista
                break;
            }
        }
    }
    return pFoundModelo;
}

CModeloPtr FindLastModelo(CModeloPtr pInicioModelo)
{
    CModeloPtr pCursor = pInicioModelo;

    while (pCursor->m_pNext)
    {
        pCursor = pCursor->m_pNext;
    }
    return pCursor;
}

void ListarModelos(void* pInicio)
{
    CModeloPtr pCursor = (CModeloPtr)pInicio;
    int nModelos = 0;
    printf("\nNOMBRE DE MODELO\t INSTANCIAS\n");
    printf("-----------------------------------\n");
    if (!pCursor)
    {
        printf("No hay modelos ni instancias.\n");
    }
    while (pCursor)
    {
        printf("%s\t\t %6d\n", pCursor->m_szRuntimeClassName, pCursor->m_nContador);
        pCursor = pCursor->m_pNext;
    }
    printf("Fin de listado.\n");
}

bool InsertarModelo(CModeloPtr pListaModelos, CModeloPtr pModeloAInsertar)
{
    bool bOK = false;
    CModeloPtr pFoundModelo = NULL;
    CModeloPtr pUltimoModelo = NULL;

    pFoundModelo = (CModeloPtr)BuscarModelo(pListaModelos, pModeloAInsertar->m_szRuntimeClassName);
    if (!pFoundModelo)
    {
        //Si no ha existido nunca ningún modelo:
        if (pListaModelos == NULL)
        {
            pListaModelos = pModeloAInsertar;
            pListaModelos->m_pNext = NULL;
            pListaModelos->m_pPrev = NULL;
            _pListaModelos = pListaModelos;
            bOK = true;
        }
        else
        {
            //Si hay modelos, colocar el nuevo modelo en la última posición de la lista:
            pUltimoModelo = (CModeloPtr)FindLastModelo(pListaModelos);
            if (pUltimoModelo)
            {
                pUltimoModelo->m_pNext = pModeloAInsertar;
                pModeloAInsertar->m_pPrev = pUltimoModelo;
                _pListaModelos = pListaModelos;
                bOK = true;
            }
        }
    }
    return bOK;
}
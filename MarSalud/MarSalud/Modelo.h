#pragma once

#include <iostream>
#include "TiposForward.h"
#include "Persona.h"

struct CModelo
{
    char m_szRuntimeClassName[100];
    int m_nContador = 0;

    CPersona m_persona;

    CModeloPtr m_pNext = NULL;
    CModeloPtr m_pPrev = NULL;
};

CModeloPtr BuscarModelo(CModeloPtr pListaModelo, char* lpszClassName);
CModeloPtr FindLastModelo(CModeloPtr pInicioModelo);
bool InsertarModelo(CModeloPtr pListaModelos, CModeloPtr pModeloAInsertar);
void ListarModelos(void* pInicio);

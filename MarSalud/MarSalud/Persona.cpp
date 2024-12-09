#include <iostream>
#include "TiposForward.h"
#include "Persona.h"
#include "Modelo.h"

#pragma warning(disable:4996)

CPersonaPtr PersonaDefaultConstructor()
{
    CPersonaPtr pNuevaInstancia;
    CModelo* pNewModelo = NULL;
    CModelo* pFoundModelo = NULL;
    CModelo* pLastModelo = NULL;
    char szTypeID[100];

    // Dar de alta la instancia:
    pNuevaInstancia = (CPersonaPtr)malloc(sizeof(CPersona));
    if (pNuevaInstancia)
    {
        // Obtener el tipo del objeto:
        memset(szTypeID, 0, sizeof(szTypeID));
        strcpy(szTypeID, typeid(*pNuevaInstancia).name());

        // Poner a cero todo el objeto:
        memset(pNuevaInstancia, 0, sizeof(CPersona));

        //Apuntarse a sí mismo:
        pNuevaInstancia->m_this = pNuevaInstancia;

        //Obtener punteros a función:
        pNuevaInstancia->m_lpfnPersonaDefaultConstructor = PersonaDefaultConstructor;
        pNuevaInstancia->m_lpfnPersonaConstructorBebe = PersonaConstructorBebe;
        pNuevaInstancia->m_lpfnPersonaConstructorAdulto = PersonaConstructorAdulto;
        pNuevaInstancia->m_lpfnDestructor = PersonaDestructor;
        pNuevaInstancia->m_lpfnImprimirFicha = ImprimirFicha;
    }
    return pNuevaInstancia;
}

CPersonaPtr PersonaConstructorBebe(char* lpszNombre,
    char* lpszApellidos)
{
    CPersonaPtr pNuevaInstancia = NULL;

    //Llamar al constructor por defecto:
    pNuevaInstancia = PersonaDefaultConstructor();
    if (pNuevaInstancia)
    {
        //Cargar valores de nombre y apellidos:
        strcpy(pNuevaInstancia->m_szNombre, lpszNombre);
        strcpy(pNuevaInstancia->m_szApellidos, lpszApellidos);
    }
    return pNuevaInstancia;
}

CPersonaPtr PersonaConstructorAdulto(char* lpszNIF,
                                     char* lpszNombre,
                                     char* lpszApellidos)
{
    CPersonaPtr pNuevaInstancia = NULL;

    //Llamar al constructor por defecto:
    pNuevaInstancia = PersonaDefaultConstructor();
    if (pNuevaInstancia)
    {
        //Cargar valores de NIF, nombre y apellidos:
        strcpy(pNuevaInstancia->m_szNIF, lpszNIF);
        strcpy(pNuevaInstancia->m_szNombre, lpszNombre);
        strcpy(pNuevaInstancia->m_szApellidos, lpszApellidos);
    }
    return pNuevaInstancia;
}

//Destructor:
void PersonaDestructor(CPersonaPtr pObjeto)
{
    // Otras tareas de liberación y restauración
    // de recursos...

    //Destruir el propio objeto
    if (pObjeto)
    {
        free(pObjeto);
    }
}

//Simulador de operador "new":
void* _new(CPersona* pInstancia)
{
    CModeloPtr pFoundModelo = NULL;
    CModeloPtr pNewModelo = NULL;
    char szTypeID[100];
    bool bOK = false;

    memset(szTypeID, 0, sizeof(szTypeID));
    strcpy(szTypeID, typeid(*pInstancia).name());

    if (pInstancia)
    {
        pFoundModelo = BuscarModelo((CModeloPtr)_pListaModelos, szTypeID); //Puntero de la instancia hacia el modelo
        if (pFoundModelo)
        {
            //Vincular la instancia con el modelo:
            pInstancia->m_pModelo = pFoundModelo;

            //Incrementar el contador de instancias del modelo
            pFoundModelo->m_nContador++; 
        }
        else
        {
            //Crear dinámicamente el modelo:
            pNewModelo = (CModeloPtr)malloc(sizeof(CModelo));

            //Poner a cero el modelo:
            memset(pNewModelo, 0, sizeof(CModelo));
            memcpy(&pNewModelo->m_persona, pInstancia, sizeof(CPersona));

            //Vincular la instancia con el modelo:
            pInstancia->m_pModelo = pNewModelo;

            //Copiar el nombre del modelo a partir del typeid
            strcpy(pNewModelo->m_szRuntimeClassName, szTypeID);

            //Inicializar el contador de instancias de este modelo a 1
            pNewModelo->m_nContador = 1;  

            bOK = InsertarModelo((CModeloPtr)_pListaModelos, pNewModelo);
        }
    }
    return pInstancia;
}

//Simulador de operador "delete"
void _delete(CPersonaPtr pObjeto)
{
    char szTypeID[100];
    CModeloPtr pFoundModelo = NULL;
    CModeloPtr pPreviousModelo = NULL;

    // Obtener el tipo del objeto:
    memset(szTypeID, 0, sizeof(szTypeID));
    strcpy(szTypeID, typeid(*pObjeto).name());

    if (_pListaModelos)
    {
        pFoundModelo = BuscarModelo((CModeloPtr)_pListaModelos, szTypeID);
        if (pFoundModelo)
        {
            if (pFoundModelo == pObjeto->m_pModelo)
            {
                if (pObjeto->m_pModelo)
                {
                    if (pFoundModelo->m_nContador > 0)
                    {
                        pFoundModelo->m_nContador--;
                    }
                    if (pFoundModelo->m_nContador == 0)
                    {
                        pObjeto->m_pModelo = NULL; //Borrado en la instancia

                        //Ahora hay que borrarlo de la lista de modelos:
                        //Primero puenteamos a pFoundModelo:
                        pPreviousModelo = pFoundModelo->m_pPrev;
                        if (pPreviousModelo)
                        {
                            pPreviousModelo->m_pNext = pFoundModelo->m_pNext;
                        }
                        else
                        {
                            // Si no hay modelo anterior, ponemos de primero al siguiente 
                            // del que vamos a borrar, es decir, shiftamos a la izquierda la lista
                            // (que también puede ser NULL cuando no quede ningún modelo)

                            _pListaModelos = pFoundModelo->m_pNext;
                            if (_pListaModelos)
                            {
                                ((CModeloPtr)_pListaModelos)->m_pPrev = NULL; //El primero no tiene nunca un anterior
                            }
                        }
                        //Y luego liberamos pFoundModelo:
                        free(pFoundModelo);
                    }
                }
            }
        }
    }
    if (pObjeto->m_lpfnDestructor)
    {
        (pObjeto->m_lpfnDestructor)(pObjeto);
    }
    else
    {
        free(pObjeto);
    }
    pObjeto = NULL;
}

void ImprimirFicha(CPersonaPtr pPersona)
{
    printf("\n");
    printf("FICHA DE PERSONA:\n");
    printf("-----------------\n");
    printf("Direccion de puntero base:%016zx\n", (unsigned __int64)pPersona->m_this); //64 bits
    printf("CIF/DNI/NIE..............:%s\n", pPersona->m_szNIF);
    printf("Nombre...................:%s\n", pPersona->m_szNombre);
    printf("Apellidos................:%s\n", pPersona->m_szApellidos);
    printf("Direccion................:%s\n", pPersona->m_szDireccion);
    printf("Cod.Postal...............:%s\n", pPersona->m_szCodPostal);
    printf("Poblacion................:%s\n", pPersona->m_szPoblacion);
    printf("Telefono 1...............:%s\n", pPersona->m_szTelefono1);
    printf("Telefono 2...............:%s\n", pPersona->m_szTelefono2);
    printf("Email....................:%s\n", pPersona->m_szEmail);
}
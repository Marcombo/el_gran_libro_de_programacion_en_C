#include <iostream>
#include "Doctor.h"

#pragma warning(disable:4996) //Para ignorar warnings por las  funciones clásicas de strings

CDoctorPtr DoctorDefaultConstructor() 
{
    CDoctorPtr pNuevaInstancia;
    CModelo* pNewModelo = NULL;
    CModelo* pFoundModelo = NULL;
    CModelo* pLastModelo = NULL;
    char szTypeID[100];

    // Dar de alta la instancia:
    pNuevaInstancia = (CDoctorPtr)malloc(sizeof(CDoctor));
    if (pNuevaInstancia)
    {
        // Poner a cero todo el objeto:
        memset(pNuevaInstancia, 0, sizeof(CDoctor));

        //Apuntarse a sí mismo:
        pNuevaInstancia->m_this = pNuevaInstancia;

        //Cargar punteros a funciones propias de CDoctor:
        pNuevaInstancia->m_lpfnDefaultDoctorConstructor = DoctorDefaultConstructor;
        pNuevaInstancia->m_lpfnDefaultDoctorDestructor = DoctorDestructor;
        pNuevaInstancia->m_lpfnImprimirFicha = DoctorImprimirFicha;

        //Crear la cápsula del objeto padre del cual hereda:
        pNuevaInstancia->m_pPersona = (CPersonaPtr)_new(PersonaDefaultConstructor());

        //Sólo a efectos didácticos de comprobación:
        //Obtener el tipo de objeto de la cápsula:
        memset(szTypeID, 0, sizeof(szTypeID));
        strcpy(szTypeID, typeid(*pNuevaInstancia->m_pPersona).name());

        // Obtener el tipo del objeto doctor:
        memset(szTypeID, 0, sizeof(szTypeID));
        strcpy(szTypeID, typeid(*pNuevaInstancia).name());
    }
    //La vinculación con el modelo se hace desde el new
    return pNuevaInstancia;
}

CDoctorPtr DoctorConstructor(int nNumColegiado, //Añade un constructor y bloquea los demás constructores heredados
    char* lpszNIF, char* lpszNombre,
    char* lpszApellidos,
    char* lpszEspecialidad)
{
    CDoctorPtr pNuevaInstancia = NULL;

    //Llamar al constructor por defecto:
    pNuevaInstancia = DoctorDefaultConstructor();
    if (pNuevaInstancia)
    {
        //Valores que van a la estructura principal:
        pNuevaInstancia->m_nNumColegiado, nNumColegiado;
        strcpy(pNuevaInstancia->m_szEspecialidad, lpszEspecialidad);

        //Valores que van a la estructura heredada CPersona:
        strcpy(pNuevaInstancia->m_pPersona->m_szNIF, lpszNIF);
        strcpy(pNuevaInstancia->m_pPersona->m_szNombre, lpszNombre);
        strcpy(pNuevaInstancia->m_pPersona->m_szApellidos, lpszApellidos);
    }
    //La vinculación con el modelo se hace desde el new
    return pNuevaInstancia;
}

CDoctorPtr DoctorConstructorCopia(CPersonaPtr pPersonaOriginal) //Construye un doctor a partir de una persona
{
    CDoctorPtr pNuevaInstancia = NULL;

    //Llamar al constructor por defecto:
    pNuevaInstancia = DoctorDefaultConstructor();
    if (pNuevaInstancia)
    {
        //Cargar valores todos los campos de golpe en una sola operación:
        memcpy(pNuevaInstancia->m_pPersona, pPersonaOriginal, sizeof(CPersona));

        // Corregimos tras em memcpy para que el m_this de la persona original 
        // no nos machaque el m_this de la persona duplicada dentro de CDoctor:
        pNuevaInstancia->m_pPersona->m_this = pNuevaInstancia->m_pPersona;
    }
    //La vinculación con el modelo se hace desde el new

    return pNuevaInstancia;
}

void DoctorDestructor(CDoctorPtr pObjeto)
{
    // Otras tareas de liberación y restauración
    // de recursos...

    //Destruir el propio objeto
    if (pObjeto)
    {
        if (pObjeto->m_pPersona)
        {
            // El _delete de Doctor ya ha llamado al destructor 
            // de CPersona heredada, no hace falta llamarlo dos veces
            pObjeto->m_pPersona = NULL;
        }
        free(pObjeto);
        pObjeto = NULL;
    }
}

void* _new(CDoctor* pInstancia)
{
    CModeloPtr pFoundModelo = NULL;
    CModeloPtr pNewModelo = NULL;
    char szTypeID[100];
    bool bOK = false;

    // Obtener el tipo del objeto:
    memset(szTypeID, 0, sizeof(szTypeID));
    strcpy(szTypeID, typeid(*pInstancia).name());

    if (pInstancia)
    {
        pInstancia->m_this = pInstancia; //Puntero de la instancia de doctor hacia sí misma
        pFoundModelo = BuscarModelo((CModeloPtr)_pListaModelos, szTypeID); //Puntero de la instancia hacia el modelo
        if (pInstancia->m_pModelo)
        {
            //Vincular instancia con modelo:
            pInstancia->m_pModelo = pFoundModelo;
            
            //Datos de sistema propios del modelo:
            strcpy(pInstancia->m_pModelo->m_szRuntimeClassName, szTypeID);

            pInstancia->m_pModelo->m_nContador++; //Incrementar el contador de instancias de modelo

            //Punteros propios personalizados para el tipo:
            pInstancia->m_lpfnDefaultDoctorConstructor = DoctorDefaultConstructor;
            pInstancia->m_lpfnDefaultDoctorDestructor = DoctorDestructor;
            pInstancia->m_lpfnImprimirFicha;
        }
        else
        {
            //Crear dinámicamente el modelo:
            pNewModelo = (CModeloPtr)malloc(sizeof(CModelo));

            //Poner a cero el modelo:
            memset(pNewModelo, 0, sizeof(CModelo));
            memcpy(&pNewModelo->m_persona, pInstancia, sizeof(CDoctor));

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

void _delete(CDoctorPtr pObjeto)
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
                        //pObjeto->m_pModelo = NULL; //Borrado en la instancia

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
    if (pObjeto->m_lpfnDefaultDoctorDestructor)
    {
        //Eliminar el modelo de CPersona de la cápsula heredada también
        _delete(pObjeto->m_pPersona);

        //Liberar objeto CDoctor
        (pObjeto->m_lpfnDefaultDoctorDestructor)(pObjeto);

    }
    else
    {
        free(pObjeto);
    }
    pObjeto = NULL;
}

void DoctorImprimirFicha(CDoctorPtr pDoctor, int nTipoFicha)
{
    CPersonaPtr pPersona = pDoctor->m_pPersona;

    switch (nTipoFicha)
    {
    case 0:
        printf("\n");
        printf("FICHA DE DOCTOR: DATOS PROFESIONALES:\n");
        printf("-------------------------------------\n");
        printf("Direccion de puntero base:%016zx\n", (unsigned __int64)pDoctor->m_this); //64 bits
        printf("Numero de colegiado......:%d\n", pDoctor->m_nNumColegiado);
        printf("Universidad..............:%s\n", pDoctor->m_szUniversidad);
        printf("Especialidad.............:%s\n", pDoctor->m_szEspecialidad);
        //El break se omite intencionadamente
        //para que continúe imprimiendo los datos personales:
    case 1:
        //Llamar a la versión de la cápsula heredada CPersona:
        (*pDoctor->m_pPersona->m_lpfnImprimirFicha)(pDoctor->m_pPersona);   
        break;
    }
}

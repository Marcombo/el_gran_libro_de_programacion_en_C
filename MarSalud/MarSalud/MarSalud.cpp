// MarSalud.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>

extern void * _pListaModelos = NULL;

#include "MarSalud.h"

#pragma warning(disable:4996) //Para ignorar warnings por las  funciones clásicas de strings

int main()
{
    char szNIF[50];
    char szNombre[60];
    char szApellidos[100];
    int nOpcionPolimorfismo = 0;

    strcpy(szNIF, "NIF1234567");
    strcpy(szNombre, "Francesc");
    strcpy(szApellidos, "Casamitjana");

    CPersonaPtr pPersonaDinamica = NULL;

    pPersonaDinamica = (CPersonaPtr)_new(PersonaConstructorAdulto(szNIF, szNombre, szApellidos));
    if (pPersonaDinamica)
    {
        strcpy(pPersonaDinamica->m_szDireccion, "C/Voramar, 37");
        strcpy(pPersonaDinamica->m_szCodPostal, "08000");
        strcpy(pPersonaDinamica->m_szPoblacion, "Barcelona");

        //Imprimir la ficha de la persona:
        (pPersonaDinamica->m_lpfnImprimirFicha)(pPersonaDinamica->m_this);
    }
    ListarModelos(_pListaModelos);

    CDoctor* pDoctorDinamico = NULL;

    pDoctorDinamico = (CDoctorPtr)_new(DoctorConstructorCopia(pPersonaDinamica));
    if (pDoctorDinamico)
    {
        pDoctorDinamico->m_nNumColegiado = 77777777;
        strcpy(pDoctorDinamico->m_szUniversidad, "Universitat de Barcelona (UB)");
        strcpy(pDoctorDinamico->m_szEspecialidad, "Pediatria");

        //Imprimir la ficha con 2 opciones posibles de polimorfismo:
        nOpcionPolimorfismo = 1;
        if (nOpcionPolimorfismo == 1)
        {
            // Opción 1 de polimorfismo:
            // Polimorfismo por llamada a la función primitiva de CPersona 
            // perteneciente al objeto derivado CDoctor:
            // Con la opcion nTipoFicha = 0, imprime datos del doctor y al final datos de la persona.
            // Con la opción nTipoFicha = 1, sólo imprime lo datos de persona.
            printf("\n*** Opcion 1 de polimorfismo: ***\n");
            printf("Polimorfismo por herencia:\n");
            int nTipoFicha = 0;
            (pDoctorDinamico->m_lpfnImprimirFicha)(pDoctorDinamico->m_this, nTipoFicha);
        }

        if (nOpcionPolimorfismo == 2)
        {
            //Opción 2 de polimorfismo:
            //Sustituir  valor del puntero a función de ImprimirFicha de CDoctor
            //por el valor del puntero a función de ImprimirFicha de CPersona:
            printf("\n**Opcion 2 de polimorfismo : ***\n");
            printf("Polimorfismo por sustitución de punteros a función:\n");
            pDoctorDinamico->m_lpfnImprimirFicha = (CDoctor::LPFNIMPRIMIRFICHA)pDoctorDinamico->m_pPersona->m_lpfnImprimirFicha;
            ((CPersona::LPFNIMPRIMIRFICHA)(pDoctorDinamico->m_lpfnImprimirFicha))(pDoctorDinamico->m_pPersona);
        }
    }
    ListarModelos(_pListaModelos);

    _delete(pPersonaDinamica);
    ListarModelos(_pListaModelos);

    _delete(pDoctorDinamico);
    ListarModelos(_pListaModelos);
}


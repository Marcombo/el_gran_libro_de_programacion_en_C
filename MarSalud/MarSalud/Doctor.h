#pragma once

#include <iostream>
#include "TiposForward.h"
#include "Persona.h"
#include "Modelo.h"

struct CDoctor
{
    //Heredado:
    CDoctorPtr m_this;
    CPersonaPtr m_pPersona; //Entidad heredada, creación dinámica
    CModeloPtr m_pModelo;

    //Propio:
    int m_nNumColegiado;
    char m_szUniversidad[100];
    char m_szEspecialidad[100];

    //Personalizado:
    //Constructores y destructores:
    typedef CDoctorPtr(*LPFNDEFAULTDOCTORCONSTRUCTOR)();
    typedef CDoctorPtr(*LPFNDOCTORCONSTRUCTOR)(int nNumColegiado, char* lpszNIF, char* lpszNombre, char* lpszApellidos, char* lpszEspecialidad);
    typedef CDoctorPtr(*LPFNDOCTORCONSTRUCTORCOPIA)(CPersonaPtr pPersonaOriginal);
    typedef void (*LPFNDEFAULTDOCTORDESTRUCTOR)(CDoctorPtr pObjeto); //Destructor

    LPFNDEFAULTDOCTORCONSTRUCTOR m_lpfnDefaultDoctorConstructor = NULL;
    LPFNDEFAULTDOCTORDESTRUCTOR m_lpfnDefaultDoctorDestructor = NULL;

    //Funciones miembro:
    typedef void (*LPFNIMPRIMIRFICHA)(CDoctorPtr pObjeto, int nTipoFicha); //Destructor
    LPFNIMPRIMIRFICHA m_lpfnImprimirFicha = NULL;
};

//Para CDoctor:
CDoctorPtr DoctorDefaultConstructor(); //Sustituye al constructor por defecto de persona
CDoctorPtr DoctorConstructor(int nNumColegiado, //Añade un constructor y bloquea los demás constructores heredados
    char* lpszNIF, char* lpszNombre,
    char* lpszApellidos,
    char* lpszEspecialidad);
CDoctorPtr DoctorConstructorCopia(CPersonaPtr pPersonaOriginal); //Construye un doctor a partir de una persona
void DoctorDestructor(CDoctorPtr p); //Llamará al destructor de la clase padre
//void DoctorImprimirFicha(int nTipoListado); //Polimórfica
void* _new(CDoctor* pInstancia);
void _delete(CDoctorPtr pObjeto);
void DoctorImprimirFicha(CDoctorPtr pDoctor, int nTipoFicha);


#pragma once

#include <iostream>
#include "TiposForward.h"

#pragma warning(disable:4091) //Para ignorar warnings por no delarar variable tras el typedef

extern void* _pListaModelos;

struct CPersona
{
    //Variables miembro:
    CPersonaPtr m_this;   //Puntero a sí mismo
    CModeloPtr m_pModelo; //Puntero a su modelo
    char m_szID[80];      //Identif. único, fecha alta + contador + etc.
    char m_szNIF[50];
    char m_szSSID[50];
    char m_szNombre[60];
    char m_szApellidos[100];
    time_t m_dtfechaNacim;
    time_t m_dtfechaAlta;
    char m_szDireccion[200];
    char m_szCodPostal[10]; //Código portal o ZipCode
    char m_szPoblacion[100];
    char m_szPais[60];
    char m_szTelefono1[50];
    char m_szTelefono2[50];
    char m_szEmail[100];

    //Constructores y destructores:
    typedef CPersonaPtr(*LPFNPERSONADEFAULTCONSTRUCTOR)();
    typedef CPersonaPtr(*LPFNPERSONACONSTRUCTORBEBE)(char* lpszNombre, char* lpszApellidos);
    typedef CPersonaPtr(*LPFNPERSONACONSTRUCTORADULTO)(char* lpszNIF, char* lpszNombre, char* lpszApellidos);
    typedef void (*LPFNPERSONADESTRUCTOR)(CPersonaPtr pObjeto); //Destructor

    LPFNPERSONADEFAULTCONSTRUCTOR m_lpfnPersonaDefaultConstructor;
    LPFNPERSONACONSTRUCTORBEBE m_lpfnPersonaConstructorBebe;
    LPFNPERSONACONSTRUCTORADULTO m_lpfnPersonaConstructorAdulto;
    LPFNPERSONADESTRUCTOR m_lpfnDestructor;

    //Funciones miembro:
    typedef void (*LPFNIMPRIMIRFICHA)(CPersonaPtr pPersona);
    LPFNIMPRIMIRFICHA m_lpfnImprimirFicha;
};

//Declaraciones prototipos forward:
//Para CPersona:
CPersonaPtr PersonaDefaultConstructor();
CPersonaPtr PersonaConstructorBebe(char* lpszNombre, char* lpszApellidos);
CPersonaPtr PersonaConstructorAdulto(char* lpszNIF, char* lpszNombre, char* lpszApellidos);
void PersonaDestructor(CPersonaPtr p);
//void PersonaImprimirFicha();
void* _new(CPersona* pInstancia);
void _delete(CPersonaPtr pObjeto);
void ImprimirFicha(CPersonaPtr pPersona);
void PersonaImprimirFicha(CPersonaPtr pPersona);

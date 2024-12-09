#pragma once

#pragma warning(disable:4091) //Para ignorar warnings por no delarar variable tras el typedef

typedef struct CPersona;
typedef CPersona* CPersonaPtr;
typedef struct CModelo;
typedef CModelo* CModeloPtr;
typedef struct CDoctor;
typedef CDoctor* CDoctorPtr;

typedef void* (*LPFNDEFAULTCONSTRUCTOR)();
typedef void* (*LPFNDEFAULTDESTRUCTOR)(CPersonaPtr pObjeto);

//Lista de modelos como variable global:

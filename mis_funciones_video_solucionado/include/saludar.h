#include <stdio.h>
#include <memory.h>
#include <wtypes.h>

#ifndef _SALUDAR_
#define _SALUDAR_

#define CASTELLANO 0
#define FRANCES    1
#define INGLES     2

char * ObtenerTextoIdioma(int nIdioma); //Función peligrosa
void SaludarIdioma(int nIdioma);
void SaludarCastellano();
void SaludarFrances();
void SaludarIngles();
BOOL ObtenerTextoIdiomaExt(int nIdioma, 
					char * lpszTextoObtenido, 
					DWORD * pdwMaxSize);

#endif

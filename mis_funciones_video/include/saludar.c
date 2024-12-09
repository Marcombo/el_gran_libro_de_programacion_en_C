#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "./include/saludar.h"

void SaludarIdioma(int nIdioma)
{
    switch (nIdioma)
    {
        case CASTELLANO: 
          SaludarCastellano();
          break;
        case FRANCES: 
          SaludarFrances();
          break;
        case INGLES: 
          SaludarIngles();
          break;
        default:
          printf("No hay caso correcto de idioma\n");
          break;
    }
}

char * ObtenerTextoIdioma(int nIdioma) //Función peligrosa
{
  char szSaludo[100];
  memset(szSaludo, 0, sizeof(szSaludo));
  
  switch (nIdioma)
  {
    case CASTELLANO: 
      strcpy(szSaludo, "¡Hola!");
      break;
    case FRANCES: 
      strcpy(szSaludo, "Salut!");
      break;
    case INGLES: 
      strcpy(szSaludo, "Hello!");
      break;
    default:
      printf("No hay caso correcto de idioma\n");
      break;
  }
  return szSaludo;
}

int ObtenerTextoIdiomaExt(int nIdioma, 
					char * lpszTextoObtenido, 
					DWORD * pdwMaxSize)
{
  int nError = 0;
  int nLengthSaludo = 0;
  char szSaludo[100];
  
  memset(szSaludo, 0, sizeof(szSaludo));

  if (lpszTextoObtenido == NULL)
  {
    nError = 101;
    *pdwMaxSize = 0;
    return nError;
  }

  if (nIdioma < 0 || nIdioma > 2)
  {
    nError = 102;
    strcpy(lpszTextoObtenido, "");
    *pdwMaxSize = 0;
  }

  switch (nIdioma)
  {
    case CASTELLANO: 
      strcpy(szSaludo, "¡Hola!");
      break;
    case FRANCES: 
      strcpy(szSaludo, "Salut!");
      break;
    case INGLES: 
      strcpy(szSaludo, "Hello!");
      break;
    default: //No hay caso correcto de idioma
      //Dejamos el valor por defecto de szSaludo (cadena vacía)
      break;
  }
  nLengthSaludo = strlen(szSaludo);

  if (nLengthSaludo > *pdwMaxSize)
  {
    nError = 103;
    szSaludo[*pdwMaxSize]=0x00; //Cortamos la cadena por la longitud menos 1 (índice en base 0)
    memcpy(lpszTextoObtenido, szSaludo, *pdwMaxSize);
    *pdwMaxSize = nLengthSaludo;
    return nError;
  }

  //Si no, es que todo ha ido bien y el error es 0:
  strcpy(lpszTextoObtenido, szSaludo);
  *pdwMaxSize = nLengthSaludo;

  return nError;
}

void SaludarCastellano()
{
	printf("¡Hola!");
}

void SaludarFrances()
{
	printf("Salut!");
}

void SaludarIngles()
{
	printf("Hello!");
}


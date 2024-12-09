#include <stdio.h>
#include <string.h>
#include <memory.h>
#include"./include/saludar.h"

int main(int argc, char *argv[])
{
    char szTexto[100];
    int nError = 0;
    //Cambiar este número para simular textos más cortos
    DWORD dwTamMaximo = 10; 
    
    memset(szTexto, 0, sizeof(szTexto));
    
    //Se usa para la depuración de la función que falla:
    char * pCharRetornado = 0x0F0F0F0F0F0F0F0F;
    
    //La llamada que falla:
    //pCharRetornado = ObtenerTextoIdioma(INGLES); 

    //Llamada a la función arreglada:
    nError = ObtenerTextoIdiomaExt(INGLES, szTexto, &dwTamMaximo);
    
    //Gestión de errores:
    if (nError)
    {
        switch(nError)
        {
        case 100: 
            printf("Error desconocido.\n");
            break;
        case 101:
            printf("El parémetro 2 (lpszTextoObtenido) no puede ser igual a NULL.\n");
            break;
        case 102:
            printf("El identificador de idioma es desconocido. Use valores de 0 a 2.\n");
            break;
        case 103: //Esto es realmente un aviso (warning) y no un error:
            printf("La longitud para la variable de texto no es suficiente. Debería ser almenos de %d caracteres.\n", dwTamanoMaximo);
            break;
        default:
            printf("Error no previsto.\n");
            break;
        }
        return -1;
    }

    //Esta instrucción era para depurar con debugger:
    //strcpy(szTexto, pCharRetornado);

    if (nError == 0 || nError == 103)
    {
        printf("%s\n", szTexto);
    }
}


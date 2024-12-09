#include <stdio.h>
#include <conio.h>

int EsperarTeclaFuncion()
{
    unsigned char Ch = 0;
    unsigned char Ch2 = 0;
    unsigned int nIDFuncion = 0;
    
    while (Ch!=27)
    {
        Ch = getch();
        if (Ch!=27)
        {
            if (Ch == 0 || Ch == 224)
            {
                // Es una tecla de función de dos fases:
                // Tomar el segundo carácter de la segunda fase 
                // que está en el buffer de teclado
                Ch2 = getch(); 
                if (Ch == 0)
                {
                    if (Ch2 > 58 && Ch2 < 69)
                    {
                        nIDFuncion = Ch2 - 58;
                    }
                }
                if (Ch = 224)
                {
                    if (Ch2 > 132 && Ch2 < 135)
                    {
                        nIDFuncion = Ch2 - 132 + 10;
                    }
                }
                break;
            }
            else
            {
                // No hacer nada, es un carácter normal 
                // y nosotros esperamos una tecla de función
            }
        }
    }
    return nIDFuncion;
}

int main(int argc, char *argv[])
{
    unsigned int nIDFuncion = 0;
    do
    {
        printf("Pulse una tecla, o la tecla ESC para salir.:");
        nIDFuncion = EsperarTeclaFuncion();
        printf("La tecla de funcion es F%u\n", nIDFuncion);

    } while (nIDFuncion > 0);

    printf("\nFin de programa.\n");

    return 0;
}
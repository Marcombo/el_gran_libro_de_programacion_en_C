#include <stdio.h>
#include <stdlib.h> //contiene funciones como malloc y free
#include <wtypes.h> //contiene máscaras binarias

/************************************************************/
/* Las macros para la obtención de máscaras parciales están */
/* definidas así en la librería wtypes.h                    */
/*                                                          */
/* #define LOWORD(l) ((WORD)(l))                            */
/* #define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))  */
/* #define LOBYTE(w) ((BYTE)(w))                            */
/* #define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))      */
/************************************************************/

int main(int argc, char *argv[])
{
    WORD wNumBin16bits = 255; //Suponemos que los bytes alto y bajo serán respectivamente 0x00 y 0xFF (little endian)
    BYTE byteAlto = 0;
    BYTE byteBajo = 0;

    byteAlto = HIBYTE(wNumBin16bits);
    byteBajo = LOBYTE(wNumBin16bits);
    
    printf("Word = %d (%04x), Byte alto = %02x Byte bajo = %02x \n", wNumBin16bits, wNumBin16bits, byteAlto, byteBajo);
    
    return 0;
}
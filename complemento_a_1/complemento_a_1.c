#include <stdio.h>
#include <stdlib.h> //contiene funciones como malloc y free
#include <wtypes.h> //contiene máscaras binarias

BOOL GetSigno(short nNumero)
{
    WORD bMascaraSigno = 0xA000;
    WORD wNumero;
    memcpy(&wNumero, &nNumero, sizeof(nNumero));
    WORD wSigno = (wNumero & bMascaraSigno);
    wSigno = wSigno >> 15;
    
    BOOL bSigno = (BOOL)wSigno;

    return bSigno;
}

short invertir(short nOriginal)
{
    //Truco conocido: lo más rápido para invertir 
    //es aplicar una operación de tipo XOR de 
    //una máscara contra otra con todos los bits a 1:

    short nInverted;

    nInverted = nOriginal ^ 0xFFFF;

    return nInverted;
}

short Complemento_a_1(short nOriginal)
{
    short nDestino;
    
    /* Paso 1: */
    nDestino = invertir(nOriginal);

    /* Paso 2: Sumar 1: */
    nDestino++;

    return nDestino;
}

int main(int argc, char *argv[])
{
    short nMinuendo;
    short nSustraendoOriginal;
    short nSustraendoComplementado;
    BOOL bSignoMinuendo;
    BOOL bSignoOriginal;
    BOOL bSignoComplementado;
    BOOL bSignoResta;

    //Programa de resta binaria paso a paso:
    printf("PROGRAMA DE RESTA BINARIA COMO SUMA DE COMPLEMENTARIO:\n");
    printf("EMPLEA NUMEROS DE TIPO ENTERO short de %d bits DE LONGITUD:\n", sizeof(short)*8);

    nMinuendo = 255; 
    nSustraendoOriginal = 2;

    //Queremos restar minuendo - Sustraendo:
    printf("Queremos restar %d - %d :\n", nMinuendo, nSustraendoOriginal);

    bSignoOriginal = GetSigno(nSustraendoOriginal);
    printf("Minuendo                 = %6d (Hexa = %04x), Signo original = %d (%s)\n", 
           nMinuendo, nMinuendo, 
           bSignoOriginal, (bSignoOriginal) ? "negativo" : "positivo");

    //Hallamos el complementado a 1 del sustraendo:
    nSustraendoComplementado = Complemento_a_1(nSustraendoOriginal);
    bSignoComplementado =  GetSigno(nSustraendoComplementado);

    //Imprimir el original:
    printf("Sustraendo original      = %6d (Hexa = %04x), Signo original = %d (%s)\n", 
           nSustraendoOriginal, nSustraendoOriginal, 
           bSignoOriginal, (bSignoOriginal) ? "negativo" : "positivo");
    
    //Imprimir el complementado:
    printf("Sustraendo complementado = %6d (Hexa = %04x), Signo complementado = %d (%s)\n", 
           nSustraendoComplementado, nSustraendoComplementado, 
           bSignoComplementado, (bSignoComplementado) ? "negativo" : "positivo");

    //Restar como suma del minuendo más un número complementado del sustraendo;
    short nResta = nMinuendo + nSustraendoComplementado;
    bSignoResta = GetSigno(nResta);

    //Imprimir resta como suma de un negativo:
    printf("RESTA                    = %6d (Hexa = %04x), Signo resta = %d (%s)\n", 
           nResta, nResta, 
           bSignoResta, (bSignoResta) ? "negativo" : "positivo");

    
    return 0;
}
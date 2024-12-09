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

short RestaBinaria(short a, short b)
{
    short nResultado = 0;
    short nTemp = a;
    BOOL bSignoMinuendo = 0; //Valor por defecto: positivo

    bSignoMinuendo = GetSigno(a);
    
    // Si el signo del minuendo fuese negativo (valor true) no haría 
    // falta complementarlo en nuestro caso concreto, porque 
    // el lenguaje C ya lo complementa aantes de colocarlo en el 
    // acumulador.
    // En cambio, si la entrada fuera de número natural (positivo) 
    // y luego entrada de signo (positivo o negativo),
    // habría que complementarlo,
    // es decir, quitar los comentarios de estas líneas:

    /*****************************************
    if (bSignoMinuendo)
    {
        nTemp = Complemento_a_1(a);
        a = nTemp;
    }
    *****************************************/
    
    // El sustraendo siempre se tiene que cambiar
    // de signo y por tanto complementarlo:
    nTemp = Complemento_a_1(b);
    b = nTemp;

    //Sumamos ambos números:
    nResultado = a + b;

    return nResultado;
}

void ImprimirResultado(short nMinuendo, short nSustraendo, short nResta)
{
    short nMinuendoComplementado        = 0;
    short nSustraendoComplementado      = 0;
    BOOL  bSignoMinuendoOriginal        = 0; //Por defecto, positivo
    BOOL  bSignoSustraendoOriginal      = 0; //Por defecto, positivo
    BOOL  bSignoMinuendoComplementado   = 0; //Idem
    BOOL  bSignoSustraendoComplementado = 0; //Idem
    BOOL  bSignoResta                   = 0; //Idem

    printf("PROGRAMA DE RESTA BINARIA COMO SUMA DE COMPLEMENTARIO:\n");
    printf("EMPLEA NUMEROS DE TIPO ENTERO short de %d bits DE LONGITUD:\n", sizeof(short)*8);
    // Queremos restar minuendo - Sustraendo:
    printf("Queremos restar %d - %d :\n", nMinuendo, nSustraendo);

    bSignoMinuendoOriginal = GetSigno(nMinuendo);
    printf("Minuendo original        = %6d (Hexa = %04x), Signo original = %d (%s)\n", 
           nMinuendo, nMinuendo, 
           bSignoMinuendoOriginal, (bSignoMinuendoOriginal) ? "negativo" : "positivo");

    bSignoSustraendoOriginal = GetSigno(nSustraendo);
    printf("Sustraendo original      = %6d (Hexa = %04x), Signo original = %d (%s)\n", 
           nSustraendo, nSustraendo, 
           bSignoSustraendoOriginal, (bSignoSustraendoOriginal) ? "negativo" : "positivo");
    
    // Saber si debemos complementar el Minuendo original:
    if (bSignoMinuendoOriginal) //Si el signo es negativo
    {
        nMinuendoComplementado = Complemento_a_1(nMinuendo);
        //Imprimir el complementado:
        printf("Minuendo complementado = %6d (Hexa = %04x), Signo complementado = %d (%s)\n", 
        nMinuendoComplementado, nMinuendoComplementado, 
        bSignoMinuendoComplementado, (bSignoMinuendoComplementado) ? "negativo" : "positivo");
    }

    // Como el sustraendo siempre tiene que cambiarse de signo,
    // se complementa siempre:
    nSustraendoComplementado = Complemento_a_1(nSustraendo);
    bSignoSustraendoComplementado = GetSigno(nSustraendoComplementado);
    //Imprimir el complementado:
    printf("Sustraendo complementado = %6d (Hexa = %04x), Signo complementado = %d (%s)\n", 
           nSustraendoComplementado, nSustraendoComplementado, 
           bSignoSustraendoComplementado, (bSignoSustraendoComplementado) ? "negativo" : "positivo");
    
    // El resultado de la resta binaria (en este caso suma binaria)
    // ya nos viene calculado desde el programa principal
    // en el tercer parámetro de esta función.
    //Imprimir resta como suma de un negativo:
    bSignoResta = GetSigno(nResta);
    printf("RESTA                    = %6d (Hexa = %04x), Signo resta = %d (%s)\n", 
           nResta, nResta, 
           bSignoResta, (bSignoResta) ? "negativo" : "positivo");
}

int main(int argc, char *argv[])
{
    short nMinuendo;
    short nSustraendo;
    short nResta = 0;

    //Programa de resta binaria paso a paso:

    nMinuendo = 255; 
    nSustraendo = 2;

    nResta = RestaBinaria(nMinuendo, nSustraendo);

    ImprimirResultado(nMinuendo, nSustraendo, nResta);
    
    return 0;
}
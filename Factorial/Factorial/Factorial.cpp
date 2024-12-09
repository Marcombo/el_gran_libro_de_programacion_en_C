// Factorial.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <math.h>


#pragma warning(disable:4996)

#define ENTERO_32_BITS_CON_SIGNO 1
#define ENTERO_32_BITS_SIN_SIGNO 2
#define ENTERO_64_BITS_CON_SIGNO 3
#define COMA_FLOTANTE_32_BITS_CON_SIGNO 4 //Doble precisión
#define COMA_FLOTANTE_64_BITS_CON_SIGNO 5 //Cuádruple precisión

//Entero 32 bits con signo:
int factorial_int(int Num)
{
    int fact = 1;
    if (Num > 1)
    {
        fact = Num * factorial_int(Num - 1);
        if (fact <= 0 || fact >= INT_MAX)
        {
            printf("Error de desbordamiento aritmetico (math overflow) al calcular factorial de %d.\n", Num);
            //abort();  //opcional
        }
    }
    return fact;
}

//Entero de 32 bits sin signo:
unsigned int factorial_uint(unsigned int Num)
{
    unsigned int fact = 1;
    if (Num > 1)
    {
        fact = Num * factorial_uint(Num - 1);
        if (fact <= 0 || fact >= UINT_MAX)
        {
            printf("Error de desbordamiento aritmetico (math overflow) al calcular factorial de %d.\n", Num);
            //abort();  //opcional
        }
    }
    return fact;
}

//Enteros de 64 bits (long long = __int64):
__int64  factorial_int64(__int64 Num)
{
    __int64 fact = 1;
    if (Num > 1)
    {
        fact = Num * factorial_int64(Num - 1);
        if (fact <= 0 || fact >= LLONG_MAX)
        {
            printf("Error de desbordamiento aritmetico (math overflow) al calcular factorial de %I64d.\n", Num);
            //abort();  //opcional
        }
    }
    return fact;
}

//doble precisión (coma flotante, 32 bits):
double factorial_double(double Num)
{
    double fact = 1;
    if (Num > 1)
    {
        fact = Num * factorial_double(Num - 1);
        if (fact <= 0 || fact >= DBL_MAX)
        {
            printf("Error de desbordamiento aritmetico (math overflow) al calcular factorial de %f.\n", Num);
            //abort();  //opcional
        }
    }
    return fact;
}

//Cuádruple precisión, coma flotante 64 bits
//Mismos límites de cálculo que el double, pero mayor precisión de mantisa
long double factorial_long_double(long double Num)
{
    long double fact = 1;
    if (Num > 1)
    {
        fact = Num * factorial_long_double(Num - 1);
        if (fact <= 0 || fact >= LDBL_MAX)
        {
            printf("Error de desbordamiento aritmetico (math overflow) al calcular factorial de %f.\n", Num);
            //abort();  //opcional
        }
    }
    return fact;
}

void FactorialParaEnteros32bitsConSigno()
{
    printf("ARITMETICA ENTERA DE 32 BITS CON SIGNO\n");
    printf("--------------------------------------\n");
    int Num = 1;

    do
    {
        printf("Averiguar la factorial de :");
        scanf("%d", &Num);
        if (Num < 0)
        {
            printf("Factorial de %ld no existe.\n", Num);
        }
        else
        {
            int fact = factorial_int(Num);
            printf("Factorial de %d = %d\n", (int)Num, fact);
        }
    } while (Num >= 0);
}

void FactorialParaEnteros32bitsSinSigno()
{
    printf("ARITMETICA ENTERA DE 32 BITS SIN SIGNO\n");
    printf("--------------------------------------\n");
    unsigned int Num = 1;

    do
    {
        printf("Averiguar la factorial de :");
        scanf("%d", &Num);
        if (Num < 0)
        {
            printf("Factorial de %ld no existe.\n", Num);
        }
        else
        {
            int fact = factorial_int(Num);
            printf("Factorial de %d = %d\n", (int)Num, fact);
        }
    } while (Num >= 0);
}

void FactorialParaEnteros64bitsConSigno()
{
    printf("ARITMETICA ENTERA DE 64 BITS CON SIGNO\n");
    printf("--------------------------------------\n");
    __int64 Num = 1;
    do
    {
        printf("Averiguar la factorial de :");
        scanf("%lld", &Num);
        if (Num < 0)
        {
            printf("Factorial de %lld no existe.\n", Num);
        }
        else
        {
            __int64 fact = factorial_int64(Num);
            printf("Factorial de %d = %lld\n", (int)Num, fact);
        }
    } while (Num >= 0);
}

void FactorialParaComaFlotante32bitsConSigno()
{
    printf("ARITMETICA COMA FLOTANTE DE 32 BITS DOBLE PRECISION CON SIGNO\n");
    printf("-------------------------------------------------------------\n");
    double Num = 1;
    int nNum = 1;
    do
    {
        printf("Averiguar la factorial de :");
        scanf("%d", &nNum);
        Num = nNum; //Pasar de int a double.
        if (Num < 0)
        {
            printf("Factorial de %d no existe.\n", nNum);
        }
        else
        {
            double fact = factorial_double(Num);
            printf("Factorial de %d = %f\n (notacion cientifica: %e)\n", (int)nNum, fact, fact);
        }
    } while (Num >= 0);
}

void FactorialParaComaFlotante64bitsConSigno()
{
    printf("ARITMETICA COMA FLOTANTE DE 64 BITS DOBLE PRECISION CON SIGNO\n");
    printf("-------------------------------------------------------------\n");
    long double Num = 1;
    int nNum = 1;
    do
    {
        printf("Averiguar la factorial de :");
        scanf("%d", &nNum);
        Num = nNum; //Pasar de int a double.
        if (Num < 0)
        {
            printf("Factorial de %d no existe.\n", nNum);
        }
        else
        {
            long double fact = factorial_long_double(Num);
            printf("Factorial de %d = %f (notacion cientifica: %e)\n", (int)nNum, fact, fact);
        }
    } while (Num >= 0);
}

////////////////////////////////////////////////////////////
// Programa principal:
// ---------------------------------------------------------
// El programa deja de preguntar para calcular 
// factoriales cuando le entras un número negativo 
// como, por ejemplo -1.
////////////////////////////////////////////////////////////

int main()
{
    int nTipoAritmetica = -1;

    while (nTipoAritmetica != 0)
    {
        printf("\n\n");
        printf("ESCOJA LA ARIMETICA QUE DESEE USAR:\n");
        printf("-----------------------------------\n");
        printf("1.- Entero 32 bits sin signo (int).\n");
        printf("2.- Entero 32 bits con signo (unsigned int).\n");
        printf("3.- Entero 64 bits sin signo (__int64 o long long).\n");
        printf("4.- Coma flotante 32 bits doble precision con signo (double)\n");
        printf("5.- Coma flotante, cuadruple precision 64 bits con signo (long double)\n");

        printf("0.------- Salir de programa.\n\n");

        printf("Tipo de aritmetica que desea usar:");
        scanf("%d", &nTipoAritmetica);

        switch (nTipoAritmetica)
        {
        case ENTERO_32_BITS_CON_SIGNO:
            FactorialParaEnteros32bitsConSigno();
            break;
        case ENTERO_32_BITS_SIN_SIGNO:
            FactorialParaEnteros32bitsSinSigno();
            break;
        case ENTERO_64_BITS_CON_SIGNO:
            FactorialParaEnteros64bitsConSigno();
            break;
        case COMA_FLOTANTE_32_BITS_CON_SIGNO:
            FactorialParaComaFlotante32bitsConSigno();
            break;
        case COMA_FLOTANTE_64_BITS_CON_SIGNO:
                FactorialParaComaFlotante64bitsConSigno();
                break;
        case 0:
            break;
        }
    }
    
}


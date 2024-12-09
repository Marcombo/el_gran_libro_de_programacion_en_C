#include <stdio.h>

int main(int argc, char *argv[])
{
    char szNombre[80];
    int nEdad = 0;

    printf("Dime tu nombre:");
    scanf("%80s", szNombre);

    printf("Dime cuantos años tienes:");
    scanf("%d", &nEdad);

    printf("\nVeo que te llamas %s y que tienes %d años.\n", szNombre, nEdad);

    return 0;
}
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    /*Declaración de variables: */
    int nEdad;
    char szNombre[40];
    int nNumEmpleado;
    double fSueldo;
    float fPorcentImpuestos;

    /*Asignación de variables: */
    nEdad = 21;
    nNumEmpleado = 317;
    fSueldo = 2700.0;
    fPorcentImpuestos = 21.5;
    strcpy(szNombre, "José Carlos García");

    printf("Nombre: %s\nEdad: %d\nNum. empleado:%08d\n", 
           szNombre, nEdad, nNumEmpleado);
    printf("Sueldo:%7.2f €\n", fSueldo);
    printf("Porcentaje impuestos: %1.2f%%\n", fPorcentImpuestos);
    return 0;
}
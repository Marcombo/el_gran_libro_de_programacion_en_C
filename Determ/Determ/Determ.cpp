// Determ.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

////////////////////////////////////////////////////////////
// Función GetValor de una celda de una matriz
// ---------------------------------------------------------
// Parte de que el buffer donde se encuentra la mariz de 
// doubles es lineal y no bidimensional.
// A partir de las dimensiones cuadradas de la matriz, 
// recibe un parámerto de columna y otro de fila y, 
// gracias a la dimensión de dicha matriz, sabemos cuál es 
// la posición lineal de la celda. 
// Dicho de otra manera, pasa de coordenadas 2D a 
// coordenadas 1D.
// La función retorna el valor numérico double almacenado en 
// esa posición.
// 
// Parámetros:
// -----------
//   - double* pMatriz: es el puntero al buffer de la matriz
//   - int nCol: coordenada X (columna) de la celda cuyo 
//               valor se desea obtener.
//   - int nRow: coordenada Y (fila) de la celda cuyo
//               valor de desea obtener.
//   - int nDimension: dimensión del lado de la matriz cuadrada.
// Valor de retorno:
// -----------------
// Un double conteniendo el valor de la posición coordenada 
// mencionada.

double GetValor(double* pMatriz, int nCol, int nRow, int nDimension)
{
    int nPos = nRow * nDimension + nCol;
    double dblValor = pMatriz[nPos];
    return dblValor;
}

////////////////////////////////////////////////////////////
// Función SetValor
// ---------------------------------------------------------
// 
// Parámetros:
// -----------
//   - double* pMatriz: es el puntero al buffer de la matriz
//   - int nCol: coordenada X (columna) de la celda cuyo 
//               valor se desea obtener.
//   - int nRow: coordenada Y (fila) de la celda cuyo
//               valor de desea obtener.
//   - int nDimension: dimensión del lado de la matriz cuadrada.
//   - double dblValor: el valor que se desea colocar 
//     (escribir) en la celda mencionada.
// Valor de retorno:
// -----------------
// Ninguno.
//
void SetValor(double * pMatriz, int nCol, int nRow, int nDimension, double dblValor)
{
    int nPos = nRow * nDimension + nCol;
    pMatriz[nPos] = dblValor;
}

////////////////////////////////////////////////////////////
// Función Determinante de una matriz cuadrada
// ---------------------------------------------------------
// Parámetros:
// -----------
//   1. pMatriz es un puntero a un buffer dinámico montado
//      como una matriz (array) de números double (coma 
//      flotante en doble precisión).
//   2. nDimensiones es el número de celdas de longitud de 
//      cualquier lado de la matriz. Por ejemplo, si la 
//      matriz es de 5 x 5, su dimensión es 5.
// Valor de retorno:
// -----------------
// Retorna el resultado del determinante en formato de un 
// numérico de tipo double (coma flotante en doble precisión)
// 
double Determinante(double* pMatriz, int nDimensiones)
{
    double det = 0;
    double* pSubMatriz = NULL;

    //Subíndices y contadores de bucles:
    int x = 0;     //columna de matriz de origen.
    int y = 0;     //fila de matriz de origen.
    int nFila = 0;
    int y2 = 0;    //posición vertical dentro de la 
                   //sub-matriz de destino de la copia.                   

    if (nDimensiones < 1)
    {
        // No deberían darse nunca dimensiones negativas
    }
    else
    {
        if (nDimensiones == 1)
        {   
            // Una sola celda no tendría sentido,
            // pero si así fuera, el determinante sería la celda única:
            det = GetValor(pMatriz, 0, 0, nDimensiones);
        }
        else
        {
            if (nDimensiones == 2)
            {   
                // Cálculo del determinante mínimo de 2 x 2 como resta de diagonales
                det = GetValor(pMatriz, 0, 0, nDimensiones) * GetValor(pMatriz, 1, 1, nDimensiones) - GetValor(pMatriz, 1, 0, nDimensiones) * GetValor(pMatriz, 0, 1, nDimensiones);
            }
            else
            {
                // Más de 2 dimensiones:
                // Cuerpo de función recursiva:

                // Para cada fila de la matriz principal seleccionamos para obtener:
                //   - Por un lado la primera celda de la fila (columna 0 de la matriz)
                //   - Y por otro lado la sub-matriz opuesta a esa celda.
                for (nFila = 0; nFila < nDimensiones; nFila++)
                {
                    // Reservar memoria para la sub-matriz, sabiendo que la sub-matriz 
                    // tiene una dimensión inferior en 1 unidad respecto a la matriz madre.
                    pSubMatriz = (double*)malloc((nDimensiones - 1) * (nDimensiones - 1) * sizeof(double));

                    // Rellenar la SubMatriz:
                    for (x = 1; x < nDimensiones; x++)  //Barrido por las columnas de la matriz de origen
                                                        //empieza en la posición 1 porque ya se sabe que 
                                                        //la posición 0 no se va a usar.
                    {
                        y2 = 0;  // establecer de inicio la primera posición vertical dentro de la columna de la sub-matriz
                        // Hacer bucle para pasar por todas las columnas 
                        for (y = 0; y < nDimensiones; y++) //Barrido por las filas de la matriz de origen
                        {
                            //Pasa por las filas, pero se salta una, que corresponde con el "gap" de la proipia fila nFila:
                            if (y == nFila) continue; // Saltarse esta fila porque es un "gap" entre filas

                            // copiar el elemento de la matriz de origen hacia la sub-matriz
                            SetValor(pSubMatriz, x - 1, y2, nDimensiones-1, GetValor(pMatriz, x, y, nDimensiones));  
                            
                            y2++;  // incrementar cursor vertical en la sub-matriz
                        }
                    }

                    // Cálculo del determinante de la submatriz y acumulación en la variable de retorno
                    // Llama la atención el uso de la función pow  (potencia o "elevado a"). 
                    // Es la que se encarga de cambiar el signo +/- según sea par/impar nFila, 
                    // cambiando el signo del determinante.
                    // Esta es la fórmula de cálculo que acumula los sub-determinantes 
                    // parciales dentro de la variable det. 
                    det += pow(-1.0, 1.0 + nFila + 1.0) * GetValor(pMatriz, 0, nFila, nDimensiones) * Determinante(pSubMatriz, nDimensiones - 1);

                    //Eliminar la sub-matriz
                    free(pSubMatriz); 
                }
            }
        }
    }
    return det;
}

int main()
{
    int nDimensiones = 4;

    double* pMatriz = NULL;
    
    //Crear el buffer para la matriz:
    pMatriz = (double*)malloc(nDimensiones * nDimensiones * sizeof(double));

    //Rellenar los datos de la matriz:

    //Fila 1:
    SetValor(pMatriz, 0, 0, nDimensiones, 4.0);
    SetValor(pMatriz, 1, 0, nDimensiones, 7.0);
    SetValor(pMatriz, 2, 0, nDimensiones, 3.0);
    SetValor(pMatriz, 3, 0, nDimensiones, 8.0);

    //Fila 2:
    SetValor(pMatriz, 0, 1, nDimensiones, 1.0);
    SetValor(pMatriz, 1, 1, nDimensiones, 6.0);
    SetValor(pMatriz, 2, 1, nDimensiones, 11.0);
    SetValor(pMatriz, 3, 1, nDimensiones, 5.0);

    //Fila 3:
    SetValor(pMatriz, 0, 2, nDimensiones, 5.0);
    SetValor(pMatriz, 1, 2, nDimensiones, 3.0);
    SetValor(pMatriz, 2, 2, nDimensiones, 9.0);
    SetValor(pMatriz, 3, 2, nDimensiones, 4.0);

    //Fila 4:
    SetValor(pMatriz, 0, 3, nDimensiones, 8.0);
    SetValor(pMatriz, 1, 3, nDimensiones, 11.0);
    SetValor(pMatriz, 2, 3, nDimensiones, 7.0);
    SetValor(pMatriz, 3, 3, nDimensiones, 3.0);

    //Si la haces de 3x3 con estas cifras, pero 
    // poniendo nDimensiones a 3 (o sea 3x3), 
    // el resultado debería darte 325
    // 
    //Si pones estas cifras con dimensiones=4 
    // (o sea 4x4), el resultado debería darte -3265

    double dblResultado = Determinante(pMatriz, nDimensiones);

    printf("Determinante = %lf\n", dblResultado);

    free(pMatriz); 
}



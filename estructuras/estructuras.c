#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <memory.h>
#include <wtypes.h>
#include <wchar.h>
#include <stdlib.h> 
#include <string.h>

/********************************************************************************/
/* Ejemplo de ordenación de una matriz de structs declarada como typedef struct */
/* usando el algoritmo de la burbuja.                                           */
/********************************************************************************/
#define NUMCLIENTES 10

typedef struct 
{
    int nID;
    char szNIF[50];
    char szEmpresa[100];
    char szNombre[80];
    char szApellidos[256];
    char szDireccion[100];
    char szPoblacion[100];
    char szCodPostal[20];
    wchar_t * lpwszComentarios;
}tagCliente;

tagCliente cliente[NUMCLIENTES];
tagCliente swapCliente;

BOOL SetComentarios(tagCliente * pCliente, wchar_t * lpwszTextoComentarios)
{
    BOOL bOK = FALSE; //Asumimos que sólo va bien si ha pasado por la asignación de variable.
    int nLongitud = 0; //Para valorar la longitud del string
    int nMargenSeguridad = 2; //Daremos dos caracteres adicionales para colocar el terminador '\0' y un cojín adidional de 1 caracter más.
    int nTamBuffer = 0; //Tamaño de buffer: Será el resultado de sumar la longitud de la cadena de texto + margen de seguridad.

    //Medidas de previsión de seguridad:
    //Para evitar que nos pasen un puntero nulo hacia la nada absoluta:
    if (lpwszTextoComentarios!=NULL) 
    {
        nLongitud = wcslen(lpwszTextoComentarios);
        //Para evitar que nos pasen una cadena de texto vacía
        if (nLongitud > 0) 
        {
            //Reservar memoria con un margen de seguridad de 2 caracteres de 16 bits por cada caracter:
            nTamBuffer = nLongitud + nMargenSeguridad;
            pCliente->lpwszComentarios = malloc(nTamBuffer * sizeof(wchar_t));

            //Valoramos que la reserva de memoria se haya realizado si el
            //puntero devuelto por el malloc es diferente de NULL:
            if (pCliente->lpwszComentarios!=NULL) 
            {
                //Inicializamos a cero todo el buffer apuntado por lpwszComentarios:
                memset(pCliente->lpwszComentarios, 0, nTamBuffer);
                
                //Colocamos el texto sobre el buffer:
                //Contendrá dicho texto y 4 bytes más (2 wchar_t) puestos a terminador cero.
                wcscpy(pCliente->lpwszComentarios, lpwszTextoComentarios); 
                bOK = TRUE;
            }
        }
    }
    // Si no se ha conseguido asignar valor al string,
    // asegurarse de que el puntero al string dinámico sea NULL.
    // Así lo dejaremos marcado dentro del struct.
    if (!bOK)
    {
        pCliente->lpwszComentarios = NULL;
    }
    return bOK;
}

// Para liberar la memoria dinámica usada por los comentarios:
void LiberarMemoriaComentarios()
{
    int n = 0;
    
    for (n = 0; n < NUMCLIENTES; n++)
    {
        if (cliente[n].lpwszComentarios!=NULL) //Si se ha reservado alguna vez memoria para comentarios sobre ese cliente
        {
            free(cliente[n].lpwszComentarios);
            cliente[n].lpwszComentarios = NULL;
        }
    }
}

void Inicializar()
{
    memset(&cliente, 0, sizeof(cliente)); //Toda la matriz puesta a cero
    memset(&swapCliente, 0, sizeof(tagCliente)); //Elemento de swap o intercambio

    // Para cambiar el contexto de página de códigos y cultura:
    wchar_t wszCountry[40]; 
    wchar_t * lpwszCountry = NULL;
    memset(wszCountry, 0, sizeof(wszCountry));

    lpwszCountry =  _wsetlocale(LC_ALL, L".ACP"); //Equivale a página de códigos del sistema operativo
    if (lpwszCountry) //Si el locale ha entendido correctamente, el puntero será diferente de NULL
    {
        wcscpy(wszCountry, lpwszCountry);
    }
}

void EntrarDatos()
{
    BOOL bComentarioAsignado = FALSE;

    cliente[0].nID = 1;
    strcpy(cliente[0].szNIF, "11111111Q");
    strcpy(cliente[0].szEmpresa, "Librería HispanoAmericana");
    strcpy(cliente[0].szNombre, "Jeroni");
    strcpy(cliente[0].szApellidos, "Boixareu");
    strcpy(cliente[0].szDireccion, "Gran Vía, 594");
    strcpy(cliente[0].szPoblacion, "Barcelona");
    strcpy(cliente[0].szCodPostal, "08000");
    bComentarioAsignado = SetComentarios(&cliente[0], L"La librería de mi editor Jeroni");

    cliente[1].nID = 2;
    strcpy(cliente[1].szNIF, "22222222Q");
    strcpy(cliente[1].szEmpresa, "Fotógrafo Josep García");
    strcpy(cliente[1].szNombre, "Josep");
    strcpy(cliente[1].szApellidos, "Garcia");
    strcpy(cliente[1].szDireccion, "Barrio de Cerdanyola");
    strcpy(cliente[1].szPoblacion, "Mataró");
    strcpy(cliente[1].szCodPostal, "08303");
    bComentarioAsignado = SetComentarios(&cliente[1], L"Gran actor, artista y fotógrafo");

    cliente[2].nID = 3;
    strcpy(cliente[2].szNIF, "33333333Q");
    strcpy(cliente[2].szEmpresa, "Kaj Tiel Plu");
    strcpy(cliente[2].szNombre, "Txema");
    strcpy(cliente[2].szApellidos, "Milla");
    strcpy(cliente[2].szDireccion, "C/San Jorge");
    strcpy(cliente[2].szPoblacion, "Mataró");
    strcpy(cliente[2].szCodPostal, "08303");
    bComentarioAsignado = SetComentarios(&cliente[2], L"Poeta y músico esperantista");

    cliente[3].nID = 4;
    strcpy(cliente[3].szNIF, "44444444Q");
    strcpy(cliente[3].szEmpresa, "Xavi Fabregas");
    strcpy(cliente[3].szNombre, "Xavi");
    strcpy(cliente[3].szApellidos, "Fábregas");
    strcpy(cliente[3].szDireccion, "Plaza Granollers");
    strcpy(cliente[3].szPoblacion, "Mataró");
    strcpy(cliente[3].szCodPostal, "08303");
    bComentarioAsignado = SetComentarios(&cliente[3], L"Base del equipo de basket.");

    cliente[4].nID = 5;
    strcpy(cliente[4].szNIF, "55555555Q");
    strcpy(cliente[4].szEmpresa, "Flor Hispania");
    strcpy(cliente[4].szNombre, "Cesc");
    strcpy(cliente[4].szApellidos, "Casamitjana");
    strcpy(cliente[4].szDireccion, "C/Recoder");
    strcpy(cliente[4].szPoblacion, "Mataró");
    strcpy(cliente[4].szCodPostal, "08304");
    bComentarioAsignado = SetComentarios(&cliente[4], L"Guitarrista y jugador de balonmano.");

    cliente[5].nID = 6;
    strcpy(cliente[5].szNIF, "66666666Q");
    strcpy(cliente[5].szEmpresa, "MorpheusMaster");
    strcpy(cliente[5].szNombre, "Montse");
    strcpy(cliente[5].szApellidos, "Yuste");
    strcpy(cliente[5].szDireccion, "C/Luis Moret");
    strcpy(cliente[5].szPoblacion, "Mataró");
    strcpy(cliente[5].szCodPostal, "08304");
    bComentarioAsignado = SetComentarios(&cliente[5], L"La empollona más molona del grupo.");

    cliente[6].nID = 7;
    strcpy(cliente[6].szNIF, "77777777Q");
    strcpy(cliente[6].szEmpresa, "APDH");
    strcpy(cliente[6].szNombre, "Montse");
    strcpy(cliente[6].szApellidos, "Gonzalez");
    strcpy(cliente[6].szDireccion, "C/Santa Ana");
    strcpy(cliente[6].szPoblacion, "Barcelona");
    strcpy(cliente[6].szCodPostal, "08004");
    bComentarioAsignado = SetComentarios(&cliente[6], L"Brigadista, profesora y melómana.");

    cliente[7].nID = 8;
    strcpy(cliente[7].szNIF, "88888888Q");
    strcpy(cliente[7].szEmpresa, "PureDevelopment, Inc.");
    strcpy(cliente[7].szNombre, "Edgar");
    strcpy(cliente[7].szApellidos, "Baldelomar");
    strcpy(cliente[7].szDireccion, "Plaza del Reloj");
    strcpy(cliente[7].szPoblacion, "Santa Coloma");
    strcpy(cliente[7].szCodPostal, "08006");
    bComentarioAsignado = SetComentarios(&cliente[7], L"Joven aventurero y programador pura sangre.");

    cliente[8].nID = 9;
    strcpy(cliente[8].szNIF, "99999999Q");
    strcpy(cliente[8].szEmpresa, "Pizzart APC");
    strcpy(cliente[8].szNombre, "Pierre");
    strcpy(cliente[8].szApellidos, "Eichenberger");
    strcpy(cliente[8].szDireccion, "C/Pierre Aéby");
    strcpy(cliente[8].szPoblacion, "Fribourg");
    strcpy(cliente[8].szCodPostal, "08006");
    bComentarioAsignado = SetComentarios(&cliente[8], L"Galerista creativo genial. Siempre te recordaremos.");

    cliente[9].nID = 10;
    strcpy(cliente[9].szNIF, "10101010Q");
    strcpy(cliente[9].szEmpresa, "Arcadi Oliveres");
    strcpy(cliente[9].szNombre, "Arcadi");
    strcpy(cliente[9].szApellidos, "Oliveres");
    strcpy(cliente[9].szDireccion, "Universitat Autònoma Barcelona");
    strcpy(cliente[9].szPoblacion, "Cerdanyola");
    strcpy(cliente[9].szCodPostal, "08193");
    bComentarioAsignado = SetComentarios(&cliente[9], L"Profesor y amigo. Te echamos de menos.");
}

void Swap(tagCliente * pClienteX, tagCliente * pClienteY)
{
    // Pasamos todos los campos de una sola vez
    // con una operación de bajo nivel llamada
    // memcpy que mueve masas de memoria de una
    // posición de puntero a la otra especificando 
    // la cantidad de memoria igual al tamaño del 
    // registro cliente

    unsigned int nTamCliente = 0;

    nTamCliente = sizeof(tagCliente);

    //Pasar el cliente X hacia swapCliente:
    memcpy(&swapCliente, pClienteX, nTamCliente);

    //Pasar el clienteY hacia el clienteX
    memcpy(pClienteX, pClienteY, nTamCliente);

    //Pasar swapCliente hacia clienteY:
    memcpy(pClienteY, &swapCliente, nTamCliente);
}

void OrdenarPorApellidos()
{
    int x = 0;
    int y = 0;
    int nComparacion = 0;
    char szNombreCompletoPosterior[400];
    char szNombreCompletoAnterior[400];
    
    for (x = 0; x <= NUMCLIENTES; x++)
    {
        for (y = 0; y < NUMCLIENTES-1;y++)
        {
            strcpy(szNombreCompletoAnterior, cliente[y].szApellidos);
            strcat(szNombreCompletoAnterior, ", ");
            strcat(szNombreCompletoAnterior, cliente[y].szNombre);

            strcpy(szNombreCompletoPosterior, cliente[y+1].szApellidos);
            strcat(szNombreCompletoPosterior, ", ");
            strcat(szNombreCompletoPosterior, cliente[y+1].szNombre);

            nComparacion = strcmp(szNombreCompletoAnterior, szNombreCompletoPosterior);
            if (nComparacion == 1) //Si es mayor el anterior que el posterior, intercambiar
            {
                Swap(&cliente[y+1], &cliente[y]);
            }
        }
    }
}

void ImprimirListado(char * lpszTitulo)
{
    int n = 0;
    int nLength = 0;
    char szNombreCompleto[400];

    nLength = strlen(lpszTitulo);

    printf("\n");
    printf("%s\n", lpszTitulo);
    for (n=0; n <=nLength;n++)
    {
        printf("-");
    }
    printf("\n\n");

    for (n = 0; n < NUMCLIENTES; n++)
    {
        memset(szNombreCompleto, 0, sizeof(szNombreCompleto));
        sprintf(szNombreCompleto, "%s, %s", cliente[n].szApellidos, cliente[n].szNombre);
        wprintf(L"%04d %40s --> %S\n", cliente[n].nID, szNombreCompleto, cliente[n].lpwszComentarios);
    }
}

int main(int argc, char *argv[])
{
    Inicializar();

    EntrarDatos();

    ImprimirListado("LISTADO SIN ORDENAR (ORDEN DE ENTRADA):");

    OrdenarPorApellidos();

    ImprimirListado("LISTADO ORDENADO POR APELLIDOS Y NOMBRE:");

    //Liberar la memoria RAM de los comentarios
    LiberarMemoriaComentarios();

    return 0;
}
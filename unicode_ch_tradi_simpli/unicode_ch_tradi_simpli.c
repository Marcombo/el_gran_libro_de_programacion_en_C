#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h> //contiene funciones como malloc y free
#include <string.h> //contiene funciones como strcpy y strcat

int main(int argc, char *argv[])
{
    //Trabajando en UTF-16:
    wchar_t wszCountry[60];
    wchar_t wszChinoSimplificado[200];
    wchar_t wszChinoTradicional[200];
    wchar_t wszMask[256];  //para formatear texto con comodidad
    wchar_t * lpwszCountry = NULL;
    int nMaxLen = 0;       //para saber la longitud máxima de la cadena Unicode
    int n = 0;             //para iterar por la cadena
    wchar_t chUnicode = 0; //Para recoger el carácter de cada posición
    
    //Inicializar a ceros:
    memset(wszCountry, 0, sizeof(wszCountry));
    memset(wszChinoSimplificado, 0, sizeof(wszChinoSimplificado));
    memset(wszChinoTradicional, 0, sizeof(wszChinoTradicional));

    //Pegamos aquí las traducciones:
    wcscpy(wszChinoSimplificado, L"我有一匹马");
    wcscpy(wszChinoTradicional,  L"我有一匹馬");
    
    //Para cambiar el contexto de página de códigos y cultura:
    lpwszCountry =  _wsetlocale(LC_ALL, L"chinese-simplified"); //chcp 936
    if (lpwszCountry)
    {
        wcscpy(wszCountry, lpwszCountry);
        //Si ha ido bien, tendremos el texto
        //"Chinese (Traditional)" dentro 
        //de la variable wszCountry
    }

    //Imprimimos en simplificado:    
    wprintf(L"Simplificado en UTF-16: '%ls'\n", wszChinoSimplificado);
    
    //Revisamos carácter por carácter:
    nMaxLen = wcslen(wszChinoSimplificado);
    for (n = 0;n<nMaxLen;n++)
    {
        chUnicode = wszChinoSimplificado[n];
        wcscpy(wszMask, L"Carácter %d de %d: '%lc' ");
        wcscat(wszMask, L"-->núm Unicode = %d ");
        wcscat(wszMask, L"(en hexadecimal: %04x)\n");
        wprintf(wszMask, n, nMaxLen, chUnicode, chUnicode, chUnicode);
    }

    wprintf(L"\n\n"); //Separamos dos líneas

    //Imprimimos en tradicional:
    wprintf(L"Tradicional  en UTF-16: '%ls'\n", wszChinoTradicional);
    
    //Revisamos carácter por carácter:
    nMaxLen = wcslen(wszChinoTradicional);
    for (n = 0;n<nMaxLen;n++)
    {
        chUnicode = wszChinoTradicional[n];
        wcscpy(wszMask, L"Carácter %d de %d: '%lc' ");
        wcscat(wszMask, L"-->núm Unicode = %d ");
        wcscat(wszMask, L"(en hexadecimal: %04x)\n");
        wprintf(wszMask, n, nMaxLen, chUnicode, chUnicode, chUnicode);
    }
    
    return 0;
}
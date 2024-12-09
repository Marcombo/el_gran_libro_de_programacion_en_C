#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h> //contiene funciones como malloc y free
#include <string.h> //contiene funciones como strcpy y strcat
#include <windows.h>

int main(int argc, char *argv[])
{
    //Trabajando en UTF-16:
    wchar_t wszFrase1[200];
    
    //Para cambiar el contexto de página de códigos y cultura:
    wchar_t wszCountry[40]; 
    wchar_t * lpwszCountry = NULL;
    memset(wszCountry, 0, sizeof(wszCountry));

    lpwszCountry =  _wsetlocale(LC_ALL, L"chinese-traditional"); //Equivale a página de códigos 936
    if (lpwszCountry) //Si el locale ha entendido correctamente, el puntero será diferente de NULL
    {
        wcscpy(wszCountry, lpwszCountry);
    }

    memset(wszFrase1, 0, sizeof(wszFrase1));
    wcscpy(wszFrase1, L"我是中国人。 我有一匹马和两只猫。");
    wprintf(L"Cadena en UTF-16: '%ls'\n", wszFrase1);
    
    return 0;
}
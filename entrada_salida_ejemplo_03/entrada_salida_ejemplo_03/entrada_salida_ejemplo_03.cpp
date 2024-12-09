// entrada_salida_ejemplo_03.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <memory.h>
#include <string.h>
#include <windows.h>
#include <ConsoleApi.h>
#include <ConsoleApi2.h>

#pragma warning(disable:4996)

void GoToXY(int column, int line)
{
    COORD coord;
    coord.X = column;
    coord.Y = line;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleCursorPosition(hConsole, coord);
}

void ClrScr()
{
    system("cls");
}

void clrscr()
{
    ClrScr();
}

COORD GetCursorPosition()
{
    COORD coord;
    HANDLE hConsole = NULL;
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;

    memset(&consoleBufferInfo, 0, sizeof(CONSOLE_SCREEN_BUFFER_INFO));

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &consoleBufferInfo);
    coord.X = consoleBufferInfo.dwCursorPosition.X;
    coord.Y = consoleBufferInfo.dwCursorPosition.Y;

    return coord;
}

COORD GetConsoleSize()
{
    COORD coord;
    HANDLE hConsole = NULL;
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
    CONSOLE_FONT_INFOEX fontInfoEx;
    BOOL bOK = FALSE;

    memset(&consoleBufferInfo, 0, sizeof(CONSOLE_SCREEN_BUFFER_INFO));

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &consoleBufferInfo);
    coord.X = consoleBufferInfo.dwSize.X;
    coord.Y = consoleBufferInfo.dwSize.Y;

    bOK = SetConsoleOutputCP(850); //Old was 437
    SetConsoleCP(1252);
    fontInfoEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    bOK = GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfoEx);
    wcscpy(fontInfoEx.FaceName, L"Terminal");
    bOK = SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfoEx);

    return coord;
}

void SetFGColor(WORD nColor)
{
    HANDLE hConsole = NULL;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, nColor);
}

WORD GetTextAttributes()
{
    HANDLE hConsole = NULL;
    CONSOLE_SCREEN_BUFFER_INFOEX consoleInfoEx;
    BOOL bOK = FALSE;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    consoleInfoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    bOK = GetConsoleScreenBufferInfoEx(hConsole, &consoleInfoEx);
    return consoleInfoEx.wAttributes;
}

void SetTextAttributes(WORD nColor)
{
    HANDLE hConsole = NULL;
    CONSOLE_SCREEN_BUFFER_INFOEX consoleInfoEx;
    BOOL bOK = FALSE;

    consoleInfoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    bOK = GetConsoleScreenBufferInfoEx(hConsole, &consoleInfoEx);
    consoleInfoEx.wAttributes = nColor;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void PintarRecuadroTitulo()
{
    GoToXY(0, 1);
    putwchar(0xC9);
    COORD consoleSize = GetConsoleSize();
    for (int x = 1; x < consoleSize.X - 1; x++)
    {
        putwchar(0xCD);
    }
    putwchar(0xBB);
    //
    putwchar(0x00BA);
    for (int x = 1; x < consoleSize.X - 1; x++)
    {
        putwchar(0x0020);
    }
    putwchar(0x00BA);
    //
    putwchar(0x00CC);
    for (int x = 1; x < consoleSize.X - 1; x++)
    {
        putwchar(0x00CD);
    }
    putwchar(0x00B9);
    //
    putwchar(0x00BA);
    for (int x = 1; x < consoleSize.X - 1; x++)
    {
        putwchar(0x0020);
    }
    putwchar(0x00BA);
    //
    putwchar(0x00C8);
    for (int x = 1; x < consoleSize.X - 1; x++)
    {
        putwchar(0x00CD);
    }
    putwchar(0x00BC);

    GoToXY(50, 2);

    SetFGColor(FOREGROUND_GREEN);

    printf("Ejemplo usando EntraString");
}

char EntraString(int x, int y, unsigned char* lpszString, size_t nMaxChars)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    int n = 0;
    size_t nLength = 0;
    int nStart = 0;
    unsigned char Ch = 0x00;
    unsigned char Ch2 = 0x00;
    unsigned char* lpszBuffer = NULL;
    unsigned char* pCursor = NULL;

    SetFGColor(FOREGROUND_RED | FOREGROUND_GREEN);

    //Recortar string original al tamaño máximo nMaxChars si nos dan uno con valor:
    if (lpszString)
    {
        nLength = strlen((char*)lpszString);
        if (nLength > 0)
        {
            if (nLength > nMaxChars)
            {
                //Cortar el string:
                (*(lpszString + nMaxChars) = 0);
                nLength = nMaxChars;
            }
            pCursor = lpszString;
            n = 0;
            GoToXY(x, y);
            while (n < nMaxChars && n < strlen((char*)lpszString))
            {
                putchar(*pCursor);
                n++;
                pCursor++;
            }
        }
        nStart = n;
    }

    //Reservar el b8uffer de tecleos:
    lpszBuffer = (unsigned char*)malloc((size_t)nMaxChars+4); //4 = margen de seguridad contra buffer overrun
    if (lpszBuffer)
    {
        memset(lpszBuffer, 0, nMaxChars+4);
        pCursor = lpszBuffer;
    }
    //Si hay valor previo de la variable, copiarlo en el buffer:
    if (lpszString)
    {
        nLength = strlen((char*)lpszString);
        if (nLength)
        {
            strcpy((char*)lpszBuffer, (char*)lpszString);
            pCursor = pCursor + nLength;
        }
    }

    //Rellenar hasta la longitud máxima con ceracteres de subrayado:
    GoToXY(x + nStart, y);
    for (n = nStart; n < nMaxChars; n++)
    {
        putch('_');
    }
    GoToXY(x + nStart, y);
    while (Ch != 27 && Ch != 13 && Ch!=10 && Ch!='\t')
    {
        Ch = getch();
        switch (Ch)
        {
        case 13:
            goto quit_entra_string;
        case 8:
            coord = GetCursorPosition();
            if (coord.X > x)
            {
                coord.X = coord.X - 1;
                GoToXY(coord.X, coord.Y);
                putch('_');
                pCursor--;
                *pCursor = 0;
                GoToXY(coord.X, coord.Y);
            }
            else
            {
                GoToXY(x, y);
                putch('_');
            }
            continue;
        case 224: //Scan code compuesto:
            Ch = getch();
            if (Ch == 80) //Flecha abajo
            {
                goto quit_entra_string;
            }
            if (Ch == 72) //flecha arriba
            {
                goto quit_entra_string;
            }
            if (Ch == 75) //flecha izquierda
            {
                goto quit_entra_string;
            }
            if (Ch == 77) //flecha izquierda
            {
                goto quit_entra_string;
            }
            if ((Ch > 59 && Ch < 61) || Ch == 0)
            break;
        default:
            break;
        }
     
        if (::iscntrl(Ch) || Ch == 0 || (Ch >= 133 && Ch <= 134))
        {
            if (Ch == 0) //Tecla de función F1..F10
            {
                //59 = F1, 60 = F2, 61 = F3, 62 = F4, 63 = F5, 64 = F6, 65 = F7, 66 = F8, 67 = F9, 60 = F10
                Ch2 = getch();
                Ch = Ch2;
            }
            else
            {
                if (Ch == 27) //Tecla de Escape
                {
                    //No hacer nada; ya saldrá por el break
                }
                else
                {
                    if (Ch == 224) //Teclas de flechas:
                    {
                        Ch2 = getch();
                        Ch = Ch2;
                    }
                    else
                    {
                        if (Ch > 132 && Ch < 135) //Teclas de función F11 = 133..F12 = 134
                        {
                            Ch2 = getch();
                            Ch = Ch2;
                        }
                    }
                }
            }
            break;
        }

        if (::isalpha(Ch) || ::isdigit(Ch) || Ch == 32 || ::isgraph(Ch))
        {
            coord = GetCursorPosition();
            if (coord.X < x + nMaxChars)
            {
                *pCursor = Ch;
                pCursor++;
                putch(Ch);
            }
            else
            {
                GoToXY(coord.X, coord.Y);
            }
            continue;
        }
    }//fin del while
quit_entra_string:
    //Borrar caracteres de subrayado hasta el final:
    nLength = strlen((char*)lpszBuffer);

    SetFGColor(7);
    GoToXY(x, y);
    printf("%s", lpszBuffer);

    for (n = nLength; n <= nMaxChars; n++)
    {
        GoToXY(x + n, y);
        putch(' ');
    }
    strcpy((char*)lpszString, (char*)lpszBuffer);
    free(lpszBuffer);

    return Ch;
}

int main(int argc, char* argv[])
{
    unsigned char szNombre[41];
    unsigned char szDireccion[61];
    unsigned char szCodPostal[15];
    unsigned char szPoblacion[61];
    unsigned char szPais[61];
    unsigned char szTelMovil[21];
    unsigned char szTelFijo[21];
    unsigned char szEmail[61];
    unsigned char * lpszPais = NULL;
    HANDLE hConsole = NULL;
    CONSOLE_FONT_INFOEX fontInfoEx;
    char Ch;
    BOOL bOK = FALSE;
    WORD wDefaultBlanco = GetTextAttributes(); //Default blanco = 7

    //Inicializar variables:
    memset(szNombre, 0, sizeof(szNombre));
    memset(szCodPostal, 0, sizeof(szCodPostal));
    memset(szDireccion, 0, sizeof(szDireccion));
    memset(szPoblacion, 0, sizeof(szPoblacion));
    memset(szPais, 0, sizeof(szPais));
    memset(szTelMovil, 0, sizeof(szTelMovil));
    memset(szTelFijo, 0, sizeof(szTelFijo));
    memset(szEmail, 0, sizeof(szEmail));
    
    Ch = 0x00;

    //Recuadro:
    PintarRecuadroTitulo();

    GoToXY(1, 1);

    lpszPais = (unsigned char*)setlocale(LC_ALL, "Spanish_Spain");//1252

    bOK = GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfoEx);
    wcscpy(fontInfoEx.FaceName, L"Arial");
    bOK = SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfoEx);


    SetFGColor(wDefaultBlanco);
    GoToXY(5, 12); printf("Nombre........:");
    GoToXY(5, 13); printf("Dirección.....:");
    GoToXY(5, 14); printf("Cod.Postal....:");
    GoToXY(5, 15); printf("Población.....:");
    GoToXY(5, 16); printf("País..........:");
    GoToXY(5, 17); printf("Tel. móvil....:");
    GoToXY(5, 18); printf("Tel. fijo.....:");
    GoToXY(5, 19); printf("Email.........:");
    
entra_nombre:
    Ch = EntraString(20, 12, szNombre, 40);
    if (Ch == 27) //Tecla de escape, salir
    {
        ClrScr();
        return 0;
    }
    if (Ch == 13 || Ch == 10 || Ch == '\t') goto entra_direccion;
    if (Ch == 'P') //flecha abajo
    {
        goto entra_direccion;
    }
    if (Ch == 'H') //flecha arriba
    {
        goto entra_email;
    }
    if (Ch == 'K') //flecha izquierda
    {
        goto entra_email;
    }
    if (Ch == 'M') //flecha derecha
    {
        goto entra_direccion;
    }
entra_direccion:
    Ch = EntraString(20, 13, szDireccion, 60);
    if (Ch == 27) //Tecla de escape, salir
    {
        ClrScr();
        return 0;
    }
    if (Ch == 13 || Ch == 10 || Ch == '\t')
    {
        goto entra_codpostal;
    }
    if (Ch == 'P') //flecha abajo
    {
        goto entra_codpostal;
    }
    if (Ch == 'H') //flecha arriba
    {
        goto entra_nombre;
    }
    if (Ch == 'K') //flecha izquierda
    {
        goto entra_nombre;
    }
    if (Ch == 'M') //flecha derecha
    {
        goto entra_codpostal;
    }
entra_codpostal:
    Ch = EntraString(20, 14, szCodPostal, 5);
    if (Ch == 27) //Tecla de escape, salir
    {
        ClrScr();
        return 0;
    }
    if (Ch == 13 || Ch == 10 || Ch == '\t')
    {
        goto entra_poblacion;
    }
    if (Ch == 'P') //flecha abajo
    {
        goto entra_poblacion;
    }
    if (Ch == 'H') //flecha arriba
    {
        goto entra_direccion;
    }
    if (Ch == 'K') //flecha izquierda
    {
        goto entra_direccion;
    }
    if (Ch == 'M') //flecha derecha
    {
        goto entra_poblacion;
    }

entra_poblacion:
    Ch = EntraString(20, 15, szPoblacion, 60);
    if (Ch == 27) //Tecla de escape, salir
    {
        ClrScr();
        return 0;
    }
    if (Ch == 13 || Ch == 10 || Ch == '\t') goto entra_pais;
    if (Ch == 'P') //flecha abajo
    {
        goto entra_pais;
    }
    if (Ch == 'H') //flecha arriba
    {
        goto entra_codpostal;
    }
    if (Ch == 'K') //flecha izquierda
    {
        goto entra_codpostal;
    }
    if (Ch == 'M') //flecha derecha
    {
        goto entra_pais;
    }
entra_pais:
    Ch = EntraString(20, 16, szPais, 60);
    if (Ch == 27) //Tecla de escape, salir
    {
        ClrScr();
        return 0;
    }
    if (Ch == 13 || Ch == 10 || Ch == '\t')
    {
        goto entra_telmovil;
    }
    if (Ch == 'P') //flecha abajo
    {
        goto entra_telmovil;
    }
    if (Ch == 'H') //flecha arriba
    {
        goto entra_poblacion;
    }
    if (Ch == 'K') //flecha izquierda
    {
        goto entra_poblacion;
    }
    if (Ch == 'M') //flecha derecha
    {
        goto entra_telmovil;
    }
entra_telmovil:
    Ch = EntraString(20, 17, szTelMovil, 20);
    if (Ch == 27) //Tecla de escape, salir
    {
        ClrScr();
        return 0;
    }
    if (Ch == 13 || Ch == 10 || Ch == '\t')
    {
        goto entra_telfijo;
    }
    if (Ch == 'P') //flecha abajo
    {
        goto entra_telfijo;
    }
    if (Ch == 'H') //flecha arriba
    {
        goto entra_pais;
    }
    if (Ch == 'K') //flecha izquierda
    {
        goto entra_pais;
    }
    if (Ch == 'M') //flecha derecha
    {
        goto entra_telfijo;
    }
entra_telfijo:
    if (Ch == 27) //Tecla de escape, salir
    {
        ClrScr();
        return 0;
    }
    Ch = EntraString(20, 18, szTelFijo, 20);
    if (Ch == 13 || Ch == 10 || Ch == '\t')
    {
        goto entra_email;
    }
    if (Ch == 'P') //flecha abajo
    {
        goto entra_email;
    }
    if (Ch == 'H') //flecha arriba
    {
        goto entra_telmovil;
    }
    if (Ch == 'K') //flecha izquierda
    {
        goto entra_telmovil;
    }
    if (Ch == 'M') //flecha derecha
    {
        goto entra_email;
    }
entra_email:
    Ch = EntraString(20, 19, szEmail, 60);
    if (Ch == 27) //Tecla de escape, salir
    {
        ClrScr();
        return 0;
    }
    if (Ch == 13 || Ch == 10 || Ch == '\t')
    {
        goto entra_nombre;
    }
    if (Ch == 'P') //flecha abajo
    {
        goto entra_nombre;
    }
    if (Ch == 'H') //flecha arriba
    {
        goto entra_telfijo;
    }
    if (Ch == 'K') //flecha izquierda
    {
        goto entra_telfijo;
    }
    if (Ch == 'M') //flecha derecha
    {
        goto entra_nombre;
    }
    return 0;
}



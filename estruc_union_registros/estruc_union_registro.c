#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wtypes.h>

typedef struct 
{
  unsigned short AX;
  unsigned short BX;
  unsigned short CX;
  unsigned short DX;
  unsigned short EX;
}reg8086;

typedef struct 
{
    BYTE AL;
    BYTE AH;
    BYTE BL;
    BYTE BH;
    BYTE CL;
    BYTE CH;
    BYTE DL;
    BYTE DH;
    BYTE EL;
    BYTE EH;
}reg8085;

typedef union 
{
    reg8086 CPU8086;
    reg8085 CPU8085;
}tagRegistrosGenerales;

int main(int argc, char *argv[])
{
    tagRegistrosGenerales Intel;

    memset(&Intel, 0, sizeof(Intel));

    Intel.CPU8086.AX = 0x0020;

    Intel.CPU8085.EH = 0x08;
    Intel.CPU8085.EL = 0x04;
   
    printf("Contenido AH= %02x AL= %02x\n", Intel.CPU8085.AH, Intel.CPU8085.AL);
    printf("Contenido EX= %04x (decimal = %d)\n", Intel.CPU8086.EX, Intel.CPU8086.EX);

    return 0;
}

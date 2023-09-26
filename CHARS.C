/* Dyakonov Nickolay, ND4, 30.10.2020 */
#include "STARS.h"
#include <stdio.h>

unsigned char Font[2048];

void fope( void )
{
  FILE *F;    
  F = fopen("font3.fnt", "rb");
  if (F != NULL)
  {
    fread(Font, 1, 2048, F);
    fclose(F);
  }
}

void DrawChar( int X0, int Y0, char Ch, int R, int G, int B )
{
  int x, y;

  for (y = 0; y < 8; y++)
    for (x = 0; x < 8; x++)
      if ((Font[Ch * 8 + y] & (0x80 >> x)) != 0)
        PutPixel(X0 + x, Y0 + y, R, G, B);
}

void DrawChars( int X0, int Y0, char *Str, int R, int G, int B )
{
  int i;

  for (i = 0; Str[i] != 0; i++)
    DrawChar(X0, Y0, Str[i], R, G, B), X0 += 8;
}
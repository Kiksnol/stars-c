/* Dyakonov Nickolay, ND4, 30.10.2020 */
#include "STARS.h"
#include <stdio.h>

#include <pcxhead.h>
#pragma pack (push, 1)
#pragma pack (pop)

void DrawPCX( char *FileName, int x0, int y0 )
{
  pcxFILEHEAD Head;
  FILE *F;
  int x, y, rep, p, W, i, j, v, r, g, blue;
  char b;
  byte Pal[256][3];
  BYTE *Planes[4];

  if ((F = fopen(FileName, "rb")) == NULL)
    return;

  fread(&Head, sizeof(pcxFILEHEAD), 1 ,F);
  if (Head.Manuf != 0x0A ||
    Head.Encode != 1)
  {
    fclose(F);
    return;
  }
  
  fseek(F, -768, SEEK_END);
  fread(&Pal, 3, 256, F);

  fseek(F, sizeof(Head), SEEK_SET);

  Planes[0] = malloc(Head.BytesPerLine * Head.PlanesAmount);
    for (i = 1; i < Head.PlanesAmount; i++)
      Planes[i] = Planes[i - 1] + Head.BytesPerLine;

  p = x = y = 0;
  W = Head.X2 - Head.X1 + 1;

  while (y < Head.Y2 - Head.Y1 + 1)
  {
    b = fgetc(F);
    if ((b & 0xC0) == 0xC0)
    {
      rep = b & 0x3F;
      b = fgetc(F);
    }
    else 
      rep = 1;
    while (rep-- > 0)
    {
      Planes[p][x++] = b;
      if (x >= Head.BytesPerLine)
      {
        x = 0;
        if (++p >= Head.PlanesAmount)
        {
          p = 0;
          for (i = 0; i < W; i++)
          {
            if (Head.BitsPerPixel == 8 && Head.PlanesAmount == 1)
            {
              v = Planes[0][i];
              r = Pal[v][0];
              g = Pal[v][1];
              blue = Pal[v][2];
            }
            else if (Head.BitsPerPixel == 8 && Head.PlanesAmount == 3)
            {
              r = Planes[0][i];
              g = Planes[1][i];
              blue = Planes[2][i];
            }
            else if (Head.BitsPerPixel == 1 && Head.PlanesAmount == 1)
            {
              v = (Planes[0][i >> 3] >> (7 - (x & 7))) & 1;
              r = Head.Pal[v * 3 + 0];
              g = Head.Pal[v * 3 + 1];
              blue = Head.Pal[v * 3 + 2];
            }
            else if (Head.BitsPerPixel == 1 && Head.PlanesAmount == 4)
            {
              v = 0;
              for (j = 0; j < 4; j++)
                v |= ((Planes[j][i >> 3] >> (7 - (x & 7))) & 1) << j;
              r = Head.Pal[v * 3 + 0];
              g = Head.Pal[v * 3 + 1];
              blue = Head.Pal[v * 3 + 2]; 
            }
            else
              r = g = blue = 0;
            if (r < 200 && g < 200 && blue < 200)
              PutPixel(i + x0, y + y0, r, g, blue);
          }
          y++;
        }
      }
    }
  }
  fclose(F);
}
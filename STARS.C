/* Dyakonov Nickolay, ND4, 30.10.2020 */
#include "STARS.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#define M_1_PI 0.318309886183790671538

VEC Stars[MAX];
DBL Size = 1.0, ProjDist = 1.0, Wp, Hp;
int Ws, Hs, Speed = 50000, P = 0, K = 0;
int IsPause;

void SwirlLeft( DBL Angle )
{
  int i;
  DBL tmpX, tmpY;

  for (i = 0; i < MAX; i++)
  {
    tmpX = Stars[i].X;
    tmpY = Stars[i].Y;

    Stars[i].X = tmpX * cos(Angle * M_1_PI / 30) - tmpY * sin(Angle * M_1_PI / 30);
    Stars[i].Y = tmpX * sin(Angle * M_1_PI / 30) + tmpY * cos(Angle * M_1_PI / 30);
  }
}
                  /*
void SwirlRight( int Angle )
{
  int i;
  DBL tmpX, tmpY;

  for (i = 0; i < MAX; i++)
  {
    tmpX = Stars[i].X;
    tmpY = Stars[i].Y;

    Stars[i].X = tmpX * cos(Angle * M_1_PI / 30) - tmpY * sin(Angle * M_1_PI / 30);
    Stars[i].Y = tmpX * sin(Angle * M_1_PI / 30) + tmpY * cos(Angle * M_1_PI / 30);
  }
}               */

void StarsKeyboard( unsigned char Key )
{
  if (Key == 'a')
    P -= 10;
  else if (Key == 'd')
    P += 10;
  else if (Key == 'w')
    K -= 10;
  else if (Key == 's')
    K += 10;
  else if (Key == '1')
    Speed += 10000;
  else if (Key == '2')
    Speed -= 10000;
  else if (Key == 'r')
    K = 0, P = 0;
  else if (Key = 'q')
    SwirlLeft(1);
}

DBL R0( void )
{
  return rand() / (double)RAND_MAX;
}

DBL R1( void )
{
  return 2.0 * rand() / RAND_MAX - 1.0;
}

VEC StarInit( void )
{
  VEC S;

  S.X = INFINITY * R1();
  S.Y = INFINITY * R1();
  S.Z = INFINITY * R0();

  return S;
}

void StarsInit( void )
{
  int i;

  for (i = 0; i < MAX; i++)
    Stars[i] = StarInit();
}

void OneStarDraw( VEC Star )
{
  DBL xp, yp;
  int xs, ys, c;

  xp = Star.X * ProjDist / Star.Z;
  yp = Star.Y * ProjDist / Star.Z;

  xs = (int)(xp * Ws / Wp + Ws / 2);
  ys = (int)(-yp * Hs / Hp + Hs / 2);

  c = (int)((1 - Star.Z / INFINITY) * 255);

  PutPixel(xs  + P, ys  + K, c, c, c );
}

void StarsDraw( void )
{
  int i;

  for (i = 0; i < MAX; i++)
    OneStarDraw(Stars[i]);
}

void StarsMove( DBL DeltaTime )
{
  int i;

  for (i = 0; i < MAX; i++)
  {
    if (Stars[i].Z <= 0)
      Stars[i] = StarInit();
    else
      Stars[i].Z -= Speed * DeltaTime;
  }
}

void ProjSet( void )
{
  if (Ws >= Hs)
    Wp = Size * Ws / Hs, Hp = Size;
  else
    Wp = Size, Hp = Size * Hs / Ws;
}

void ProjSetSize( int NewWs, int NewHs )
{
  Ws = NewWs;
  Hs = NewHs;

  ProjSet();
}
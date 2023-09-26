/* Dyakonov Nickolay, ND4, 30.10.2020 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <time.h>
#include <string.h>

#include "STARS.h"

unsigned char Frame[FRAME_H][FRAME_W][3];
int Zoom = 1;
DBL SyncTime, DeltaTime, FPS;
int IsPaused = 0;
char TEXT[50] = "MADE BY DYAKONOV NICKOLAY 30.10.2020", cFPS[10];

void PutPixel( int x, int y, int r, int g, int b )
{
  if (x < 0 || y < 0 || x >= FRAME_W || y >= FRAME_H)
    return;
  Frame[y][x][0] = b;
  Frame[y][x][1] = g;
  Frame[y][x][2] = r;
}

void Keyboard( unsigned char Key, int X, int Y )
{
  if (Key == 27)
    exit(0);
  else if (Key == '-')
    Zoom--;
  else if (Key == '+')
    Zoom++;
  else if (Key == 'p' || Key == 'P')
    IsPaused = !IsPaused;
  StarsKeyboard(Key);
} 

void Timer ( void )
{
  long t;
  static long StartTime, PauseTime = 0, OldTime, FPSTime, FrameCount; 
  t = clock();
 
  if (StartTime == -1)
    StartTime = OldTime = FPSTime = t;

  if (IsPaused)
  {
    PauseTime += t - OldTime;
    DeltaTime = 0;
  }
  else
  {
    PauseTime = 0;
    DeltaTime = (t - OldTime) / (DBL)CLOCKS_PER_SEC;
    SyncTime = (t - StartTime) / (DBL)CLOCKS_PER_SEC;
  }

  FrameCount++;

  if (t - FPSTime > 2 * CLOCKS_PER_SEC)
  {
    FPS = FrameCount / ((t - FPSTime) / (DBL)CLOCKS_PER_SEC);
    FPSTime = t;
    FrameCount = 0;
  }
  OldTime = t;
}

void Display( void )
{
  SYSTEMTIME st;
  char ctime[30], cdate[30];

  fope();

  GetLocalTime(&st);
  sprintf(ctime, "Current time: %02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
  sprintf(cdate, "Current date: %02d:%02d:%d", st.wDay, st.wMonth, st.wYear);
  glClearColor(0.3, 0.5, 0.7, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  Timer();
  sprintf(cFPS, "FPS: %3d", FPS);
  memset(Frame, 0, sizeof(Frame));
  StarsMove(DeltaTime / 2);
  StarsDraw();
  StarsMove(DeltaTime / 2);
  StarsDraw();
  DrawChars(0, 0, TEXT, 255, 0, 0);
  DrawChars(0, 10, ctime, 255, 0, 0);
  DrawChars(0, 20, cdate, 255, 0, 0);
  DrawChars(0, 30, "\3CGSG FOREVER\3", rand() % 255, rand() % 255, rand() % 255);
  ///DrawChars(1200, 0, cFPS, 0, 255, 0);
  DrawPCX("LOGO30.PCX", 0, 625);
  DrawPCX("o.PCX", 1080, 550);

  glRasterPos2d(-1, 1);
  glPixelZoom(Zoom, -Zoom);
  glDrawPixels(FRAME_W, FRAME_H, GL_BGR_EXT, GL_UNSIGNED_BYTE, Frame);

  glFinish();

  glutSwapBuffers();
  glutPostRedisplay();
}

void main ( int ArgC, char *Argv[] )
{
  glutInit(&ArgC, Argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(1920, 1080);
  glutCreateWindow("ND4 T21STARS");
  glutFullScreen();

  StarsInit();
  ProjSetSize(FRAME_W, FRAME_H);


  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);

  glClearColor(0.3, 0.47, 0.7, 1);
  glutMainLoop();
}               

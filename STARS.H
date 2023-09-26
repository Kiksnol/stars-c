/* Dyakonov Nickolay, ND4, 30.10.2020 */
#define INFINITY 1000000.0
#define MAX 100000
#define FRAME_H 768
#define FRAME_W 1368

typedef double DBL;

typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

void StarsInit( void );
void PutPixel( int x, int y, int r, int g, int b );
void StarsMove( DBL DeltaTime );
void ProjSetSize( int NewWs, int NewHs );
void StarsDraw( void );
void ProjSet( void );
void StarsKeyboard( unsigned char Key );
void fope( void );
void DrawChars( int X0, int Y0, char *Str, int R, int G, int B );
void DrawPCX( char *FileName, int x0, int y0 );
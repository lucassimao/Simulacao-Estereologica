#include <list>
using std::list;

#include "..\defs.h"

#define NEW(p, type) if ((p=(type *) malloc (sizeof(type))) == NULL) {\
      printf ("NEW: Out of Memory!\n");\
      exit(EXIT_FAILURE);\
    }

#define FREE(p)  if (p) { free ((void *) p); p = NULL; }

#define SWAP(t,x,y)     { t = x; x = y; y = t; }


/*----------Point(s) Structure-------------*/
typedef struct tPointStructure tsPoint;
typedef tsPoint *tPoint;
struct tPointStructure {
   int     vnum;
   Ponto v;
   bool    excluir;
};
/*----------Stack Structure-------------*/
typedef struct tStackCell tsStack; /* Used on in NEW() */
typedef tsStack *tStack;
struct tStackCell {
   tPoint   p;
   tStack   next;
};

list<Ponto> ordenarVertices(list<Ponto> *vertices);

tStack	Pop( tStack s );
void    PrintStack( tStack t );
tStack  Push( tPoint p, tStack top );
tStack  Graham();
void    Squash();
void	Copy( int i, int j );
int     Compare( const void *tp1, const void *tp2 );
void    FindLowest( );
void	Swap( int i, int j );

int     AreaSign( Ponto a, Ponto b, Ponto c );
bool    Left( Ponto a, Ponto b, Ponto c );
int     ReadPoints( list<Ponto> *vertices );


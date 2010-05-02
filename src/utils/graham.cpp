#include   <cstdio>
#include   <cmath>
#include   <cstdlib>

#include <list>
using std::list;

#include "graham.h"

#define PMAX    10               /* Max # of points */
typedef tsPoint tPointArray[PMAX];
static tPointArray P;

int n = 0;/* Actual # of points */
int ndelete = 0;                   /* Number deleted */


list<Ponto> ordenarVertices(list<Ponto> *vertices)
{
   tStack   top;
   ndelete = 0;
   n = 0;

   n = ReadPoints(vertices);

   FindLowest();

   qsort(
      &P[1],             // pointer to 1st elem 
      n-1,               // number of elems 
      sizeof( tsPoint ), // size of each elem
      Compare            // -1,0,+1 compare function 
   );

   if (ndelete > 0) {
      Squash();
   }
   top = Graham();
   list<Ponto> verticesOrdenados;

   tStack iterator = top;

   while (iterator) { 
	   Ponto p = iterator->p->v;
	   verticesOrdenados.push_back(p);
	   iterator = iterator->next;
   }

   return verticesOrdenados;
  //PrintStack( top );
}

/*---------------------------------------------------------------------
FindLowest finds the rightmost lowest point and swaps with 0-th.
The lowest point has the min y-coord, and amongst those, the
max x-coord: so it is rightmost among the lowest.
---------------------------------------------------------------------*/
void   FindLowest( )
{
   int i;
   int m = 0;   /* Index of lowest so far. */

   for ( i = 1; i < n; i++ )
      if ( (P[i].v.z <  P[m].v.z) ||
          ((P[i].v.z == P[m].v.z) && (P[i].v.x > P[m].v.x)) ) 
         m = i;
   Swap(0,m); /* Swap P[0] and P[m] */
}

void	Swap( int i, int j )
{
   double temp;
   int itemp;
   bool btemp;
   /* Uses swap macro. */

   SWAP( itemp, P[i].vnum,   P[j].vnum );
   SWAP( temp, P[i].v.x,   P[j].v.x );
   SWAP( temp, P[i].v.z,   P[j].v.z );
   SWAP( btemp, P[i].excluir, P[j].excluir );

}
/*---------------------------------------------------------------------
Compare: returns -1,0,+1 if p1 < p2, =, or > respectively;
here "<" means smaller angle.  Follows the conventions of qsort.
---------------------------------------------------------------------*/
int   Compare( const void *tpi, const void *tpj )
{
   int a;             /* area */
   double x, y;          /* projections of ri & rj in 1st quadrant */
   tPoint pi, pj;
   pi = (tPoint)tpi;
   pj = (tPoint)tpj;

   a = AreaSign( P[0].v, pi->v, pj->v );
   if (a > 0)
      return -1;
   else if (a < 0)
      return 1;
   else { /* Collinear with P[0] */
      x =  abs( pi->v.x -  P[0].v.x ) - abs( pj->v.x -  P[0].v.x );
      y =  abs( pi->v.z -  P[0].v.z ) - abs( pj->v.z -  P[0].v.z );

      ndelete++;
      if ( (x < 0) || (y < 0) ) {
         pi->excluir = true;
         return -1;
      }
      else if ( (x > 0) || (y > 0) ) {
         pj->excluir = true;
         return 1;
      }
      else { /* points are coincident */
         if (pi->vnum > pj->vnum)
             pj->excluir = true;
         else 
             pi->excluir = true;
         return 0;
      }
   }
}


/*---------------------------------------------------------------------
Pops off top elment of stack s, frees up the cell, and returns new top.
---------------------------------------------------------------------*/
tStack   Pop( tStack s )
{
   tStack top;

   top = s->next;
   FREE( s );
   return top;
}

/*---------------------------------------------------------------------
Get a new cell, fill it with p, and push it onto the stack.
Return pointer to new stack top.
---------------------------------------------------------------------*/
tStack   Push( tPoint p, tStack top )
{
   tStack   s;

   /* Get new cell and fill it with point. */
   NEW( s, tsStack );
   s->p = p;
   s->next = top;
   return s;
}
/*---------------------------------------------------------------------
---------------------------------------------------------------------*/
void   PrintStack( tStack t )
{
   if (!t) printf("Empty stack\n");
   while (t) { 
      printf("vnum=%d\tx=%f\tz=%f\n", t->p->vnum,t->p->v.x,t->p->v.z); 
      t = t->next;
   }
}

/*---------------------------------------------------------------------
Performs the Graham scan on an array of angularly sorted points P.
---------------------------------------------------------------------*/
tStack   Graham()
{
   tStack   top;
   int i;
   tPoint p1, p2;  /* Top two points on stack. */

   /* Initialize stack. */
   top = NULL;
   top = Push ( &P[0], top );
   top = Push ( &P[1], top );

   /* Bottom two elements will never be removed. */
   i = 2;

   while ( i < n ) {
      if( !top->next) printf("Error\n"),exit(EXIT_FAILURE);
      p1 = top->next->p;
      p2 = top->p;
      if ( Left( p1->v , p2->v, P[i].v ) ) {
         top = Push ( &P[i], top );
         i++;
      } else    
         top = Pop( top );
   }

   return top;

}

/*---------------------------------------------------------------------
Squash removes all elements from P marked delete.
---------------------------------------------------------------------*/
void   Squash( )
{
   int i, j;
   i = 0; j = 0;
   /*printf("Squash: n=%d\n",n);*/
   while ( i < n ) {
      /*printf("i=%d,j=%d\n",i,j);*/
      if ( !P[i].excluir ) { /* if not marked for deletion */
         Copy( i, j ); /* Copy P[i] to P[j]. */
         j++;
      }
      /* else do nothing: delete by skipping. */
      i++;
   }
   n = j;

}

void Copy( int i, int j )
{
   P[j].v.x = P[i].v.x;
   P[j].v.z = P[i].v.z;
   P[j].vnum = P[i].vnum;
   P[j].excluir = P[i].excluir;
}


/*---------------------------------------------------------------------
Returns true iff c is strictly to the left of the directed
line through a to b.
---------------------------------------------------------------------*/
bool   Left( Ponto a, Ponto b, Ponto c )
{ 
   return  AreaSign( a, b, c ) > 0;
}

int    ReadPoints( list<Ponto> *vertices )
{
	int n = 0;
	list<Ponto>::const_iterator iterator = vertices->begin();

	while(iterator!=vertices->end())
	{
		Ponto p = *iterator;
		P[n].vnum = n;
		P[n].v = p;
		P[n].excluir = false;
		++iterator;
		++n;
	}

   return n;
}



int     AreaSign( Ponto a, Ponto b, Ponto c )
{
    double area2;

    area2 = ( b.x - a.x ) * (double)( c.z - a.z ) - ( c.x - a.x ) * (double)( b.z - a.z );

    /* The area should be an integer. */
    if      ( area2 >  0.5 ) return  1;
    else if ( area2 < -0.5 ) return -1;
    else                     return  0;
}

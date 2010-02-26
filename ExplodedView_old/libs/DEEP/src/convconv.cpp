/*************************************************************************\

  Copyright 2002 The University of North Carolina at Chapel Hill.
  All Rights Reserved.

  Permission to use, copy, modify OR distribute the MODIFIED portion of this
  software and its documentation for educational, research and non-profit 
  purposes, without fee, and without a written agreement is hereby granted, 
  provided that the above copyright notice and the following three paragraphs
  appear in all copies.

  IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL BE
  LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
  CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE
  USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY
  OF NORTH CAROLINA HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGES.

  THE UNIVERSITY OF NORTH CAROLINA SPECIFICALLY DISCLAIM ANY
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE
  PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
  NORTH CAROLINA HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT,
  UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

  The authors may be contacted via:

  US Mail:             Y. Kim, M. Lin
                       Department of Computer Science
                       Sitterson Hall, CB #3175
                       University of N. Carolina
                       Chapel Hill, NC 27599-3175

  Phone:               (919) 962-1749

  EMail:               geom@cs.unc.edu
                       youngkim@cs.unc.edu
					   lin@cs.unc.edu

\**************************************************************************/

// convconv.cpp was originally written by Joseph O'Rourke (orourke@cs.smith.edu)

/*
This code is described in "Computational Geometry in C" (Second Edition),
Chapter 7.  It is not written to be comprehensible without the
explanation in that book.

Written by Joseph O'Rourke.
Last modified: December 1997
Questions to orourke@cs.smith.edu.
--------------------------------------------------------------------
This code is Copyright 1997 by Joseph O'Rourke.  It may be freely
redistributed in its entirety provided that this copyright notice is
not removed.
--------------------------------------------------------------------
*/
#include <iostream.h>
#include        <stdio.h>
#include        <math.h>

#include <SWIFT_convconv.h>

#define X       0
#define Y       1

typedef enum { Pin, Qin, Unknown } tInFlag;
typedef enum { DISJOINT=0, INTERSECT, VERTEX_P0, VERTEX_P1, VERTEX_Q0, VERTEX_Q1,
	       FACE_VERTEX_P0_Q0, FACE_VERTEX_P0_Q1, FACE_VERTEX_P1_Q0,
	       FACE_VERTEX_P1_Q1,
	       EDGE_VERTEX_Q0_Q1, EDGE_VERTEX_P0_P1, EDGE_VERTEX_Q0_P1,
	       EDGE_VERTEX_Q0_P0, EDGE_VERTEX_Q1_P1, EDGE_VERTEX_Q1_P0
 } CODE_TYPE;



/*---------------------------------------------------------------------
Function prototypes.
---------------------------------------------------------------------*/
int       indices[4];
CODE_TYPE res_code;
double  *pref;
double   min_dist;
bool     *facetype0, *facetype1;
bool    vertextype0, vertextype1;
bool    use_original;

double  Dot( tPointd a, tPointd b );
double	AreaSign( tPointd a, tPointd b, tPointd c );
double  SqrDist(tPointd&, tPointd&);
double SqrDist(tPointd& a, double* b);
CODE_TYPE   SegSegInt( tPointd a, tPointd b, tPointd c, tPointd d, tPointd p, tPointd q);
CODE_TYPE   ParallelInt( tPointd a, tPointd b, tPointd c, tPointd d, tPointd p, tPointd q );
bool    Between( tPointd a, tPointd b, tPointd c );
void    Assigndi( tPointd p, tPointd a );
void    SubVec( tPointd a, tPointd b, tPointd c );
void    ConvexIntersect( tPolygond& P, tPolygond& Q, int n, int m );
tInFlag InOut( tPointd p, tInFlag inflag, double aHB, double bHA );
int     Advance( tPolygond& poly, int a, int *aa, int n);
void InitPosition( tPolygond& poly, int& a, int& aa, int n);

void HandleSingularPoint(bool PisOne, bool QisOne);
void check_max(CODE_TYPE code, tPointd& p, int a1, int a, int b1, int b);
void check_max(CODE_TYPE code, tPointd& p, tPointd& q, int a1, int a, int b1, int b);
void dump_poly( tPolygond& P, int n, CODE_TYPE code);
void find_vertex_share(tPolygond& P, tPolygond& Q, int n, int m);

/*-------------------------------------------------------------------*/

/*---------------------------------------------------------------------
---------------------------------------------------------------------*/
double GetNextSeg(tPolygond& P, tPolygond& Q, int n, int m, double* p, 
				   int *idx, CONV_RESULT& type )
{
	pref = p;
	res_code = DISJOINT;
	min_dist = 1e10;

	ConvexIntersect(P, Q, n, m);

	switch(res_code) {
	case INTERSECT:
	  type = CONV_EE;
	  idx[0] = indices[0];
	  idx[1] = indices[2];
	  break;
	
	case VERTEX_P0:
	  type = CONV_FV;
	  idx[0] = indices[0];
	  break;
	case VERTEX_P1:
	  type = CONV_FV;
	  idx[0] = indices[1];
	  break;
	case VERTEX_Q0:
	  type = CONV_VF;
	  idx[0] = indices[2];
	  break;
	case VERTEX_Q1:
	  type = CONV_VF;
	  idx[0] = indices[3];
	  break;
	
	case FACE_VERTEX_P0_Q0:
	  type = CONV_FF;
	  idx[0] = indices[0];
	  idx[1] = indices[2];
	  break;
	case FACE_VERTEX_P0_Q1:
	  type = CONV_FF;
	  idx[0] = indices[0];
	  idx[1] = indices[3];
	  break;
	case FACE_VERTEX_P1_Q0:
	  type = CONV_FF;
	  idx[0] = indices[1];
	  idx[1] = indices[2];
	  break;
	case FACE_VERTEX_P1_Q1:
	  type = CONV_FF;
	  idx[0] = indices[1];
	  idx[1] = indices[3];
	  break;

	case DISJOINT: default:
	  type = NO_FEATURE;
	  break;
	}

	return min_dist;

}

void    ConvexIntersect( tPolygond& P, tPolygond& Q, int n, int m )
                           /* P has n vertices, Q has m vertices. */
{
   int     a, b;           /* indices on P and Q (resp.) */
   int     a1, b1;         /* a-1, b-1 (resp.) */
   tPointd A, B;           /* directed edges on P and Q (resp.) */
   double     cross;          /* sign of z-component of A x B */
   double     bHA, aHB;       /* b in H(A); a in H(b). */
   tPointd Origin = {0,0}; /* (0,0) */
   tPointd p;              /* double point of intersection */
   tPointd q;              /* second point of intersection */
   tInFlag inflag;         /* {Pin, Qin, Unknown}: which inside */
   int     aa, ba;         /* # advances on a & b indices (after 1st inter.) */
   bool    FirstPoint;     /* Is this the first point? (used to initialize).*/
   tPointd p0;             /* The first point. */
   CODE_TYPE     code;           /* SegSegInt return code. */ 
   bool    inside;
   bool    PinQ = true, QinP = true;
   bool    PisOne = false, QisOne = false; // P(or Q) is degenerated to a point ?


   /* Initialize variables. */
   InitPosition(P, a, aa, n); PisOne = (aa == n-1);
   InitPosition(Q, b, ba, m); QisOne = (ba == m-1);

   if ( PisOne || QisOne ) {
	   HandleSingularPoint(PisOne, QisOne);
	   return;
   }
    
   inflag = Unknown; FirstPoint = true;

   do {
      
      /* Computations of key variables. */
      a1 = (a + n - 1) % n;
      b1 = (b + m - 1) % m;

      SubVec( P[a], P[a1], A );
      SubVec( Q[b], Q[b1], B );

      cross = AreaSign( Origin, A, B );
      aHB   = AreaSign( Q[b1], Q[b], P[a] );
      if (aHB < 0) PinQ = false;
      bHA   = AreaSign( P[a1], P[a], Q[b] );
      if (bHA < 0) QinP = false;

      /* If A & B intersect, update inflag. */
      code = SegSegInt( P[a1], P[a], Q[b1], Q[b], p, q );
      if ( code >= INTERSECT && code <= FACE_VERTEX_P1_Q1 ) {
         if ( inflag == Unknown && FirstPoint ) {
            aa = ba = 0;
            FirstPoint = false;
            p0[X] = p[X]; p0[Y] = p[Y];
         }

		 check_max(code, p, a1, a, b1, b);
		 inflag = InOut( p, inflag, aHB, bHA );
      }

      /*-----Advance rules-----*/

      /* Special case: A & B overlap and oppositely oriented. */
      if ( ( code >= EDGE_VERTEX_Q0_Q1 && code <= EDGE_VERTEX_Q1_P0 ) && (Dot( A, B ) < 0) ) {
		check_max(code, p, q, a1, a, b1, b);
	   return;
	  }


      /* Special case: A & B collinear. */
      else if ( (AlmostZero(cross)) && ( AlmostZero(aHB) ) && ( AlmostZero(bHA) ) ) {
            /* Advance but do not output point. */
            if ( inflag == Pin )
               b = Advance( Q, b, &ba, m);
            else
               a = Advance( P, a, &aa, n);
         }

      /* Generic cases. */
      else if ( cross >= 0 ) {
		if ( bHA > 0) {
		  inside = (inflag == Pin);
			if (inside)
				check_max(VERTEX_P1, P[a], a1, a, b1, b);
			a = Advance( P, a, &aa, n);
	    
		}
		else {
			inside = (inflag == Qin);
			if (inside)
				check_max(VERTEX_Q1, Q[b], a1, a, b1, b);
			b = Advance( Q, b, &ba, m);
		}
      }
      else /* if ( cross < 0 ) */{
		if ( aHB > 0) {
			inside = (inflag == Qin);
			if (inside)
				check_max(VERTEX_Q1, Q[b], a1, a, b1, b);
			b = Advance( Q, b, &ba, m);
		}
		else {
			inside = (inflag == Pin);
			if (inside)
				check_max(VERTEX_P1, P[a], a1, a, b1, b);
			a = Advance( P, a, &aa, n);
		}
      }

   /* Quit when both adv. indices have cycled, or one has cycled twice. */
   } while ( ((aa < n) || (ba < m)) && (aa < 2*n) && (ba < 2*m) );


   // inclusion case is handled below, but vertex sharing by two touching 
   // polygons are not implemented yet
   if ( inflag == Unknown) {
   // inclusion case
	if ( PinQ && !QinP)
		 dump_poly(P, n, VERTEX_P0);
	else if ( QinP && !PinQ )
		 dump_poly(Q, m, VERTEX_Q0);
	else // only one vertex sharing case
		 find_vertex_share(P, Q, n, m);
   }

}


void find_vertex_share(tPolygond& P, tPolygond& Q, int n, int m)
{
  int i, j;

	for (i=0; i<n; i++) {
		for (j=0; j<m; j++) {
			if (P[i][0] == Q[j][0] && P[i][1] == Q[j][1]) {
				res_code = FACE_VERTEX_P0_Q0;
				indices[0] = i;
				indices[2] = j;
				return;
			}
		}
	}

	res_code = DISJOINT;
}


inline void HandleSingularPoint(bool PisOne, bool QisOne)
// always assume that the intersection between P and Q is NOT empty
{

	if (PisOne && QisOne) {
		res_code = FACE_VERTEX_P0_Q0;
		indices[0] = 0;
		indices[2] = 0;
	}
	else if (PisOne) {
		res_code = VERTEX_P0;
		indices[0] = 0;
	}
	else { // QisOne
		res_code = VERTEX_Q0;
		indices[2] = 0;
	}

}

void dump_poly( tPolygond& P, int n, CODE_TYPE code)
{
  for (int i=0; i<n; i++) {
    check_max(code, P[i], i, i, i, i);
  }
}


/*---------------------------------------------------------------------
Prints out the double point of intersection, and toggles in/out flag.
---------------------------------------------------------------------*/
inline tInFlag InOut( tPointd p, tInFlag inflag, double aHB, double bHA )
{
   /* Update inflag. */
   if      ( aHB > 0)
      return Pin;
   else if ( bHA > 0)
      return Qin;
   else    /* Keep status quo. */
      return inflag;
}
/*---------------------------------------------------------------------
   Advances and prints out an inside vertex if appropriate.
---------------------------------------------------------------------*/
inline int     Advance( tPolygond& poly, int a, int *aa, int n)
{
	int i=0;
	int ap = (a+1)%n;
	
	while ( AlmostZero(poly[a][0]-poly[ap][0]) && 
		AlmostZero(poly[a][1]-poly[ap][1]) && ++i<n-1) {
		ap = (ap+1)%n;
	}
	
	*aa += i+1;
	return  ap;
}

inline void InitPosition( tPolygond& poly, int& a, int& aa, int n)
{
	a = 0; aa = 0;

	int a1 = (a+n-1)%n;
	int i=0;

	while ( AlmostZero(poly[a][0]-poly[a1][0]) && 
		AlmostZero(poly[a][1]-poly[a1][1]) && ++i<n-1) {
		a++; 
	}

	aa += i;

}


inline bool    Collinear( tPointd a, tPointd b, tPointd c )
{
    if (a[0]==b[0] && a[1]==b[1])
		return (a[0]==c[0] && a[1]==c[1]);
	else
		return  AlmostZero(AreaSign( a, b, c ));
}
/*---------------------------------------------------------------------
a - b ==> c.
---------------------------------------------------------------------*/
inline void    SubVec( tPointd a, tPointd b, tPointd c )
{
	c[X] = a[X] - b[X];
	c[Y] = a[Y] - b[Y];
}


inline double	AreaSign( tPointd a, tPointd b, tPointd c )
{
    return(  ( b[0] - a[0] ) * ( c[1] - a[1] ) -
			 ( c[0] - a[0] ) * ( b[1] - a[1] ) );
}

/*---------------------------------------------------------------------
SegSegInt: Finds the point of intersection p between two closed
segments ab and cd.  Returns p and a char with the following meaning:
   'e': The segments collinearly overlap, sharing a point.
		This category has been more refined such as EDGE_VERTEX ..., see CODE_TYPE.   
   'v': An endpoint (vertex) of one segment is on the other segment,
        but 'e' doesn't hold.
		This category has been more refined such as VERTEX_..., see CODE_TYPE.
   '1': The segments intersect properly (i.e., they share a point and
        neither 'v' nor 'e' holds).
   '0': The segments do not intersect (i.e., they share no points).
Note that two collinear segments that share just one point, an endpoint
of each, returns 'e' rather than 'v' as one might expect.
---------------------------------------------------------------------*/
CODE_TYPE SegSegInt( tPointd a, tPointd b, tPointd c, tPointd d, tPointd p, tPointd q )
{
   double  s, t;       /* The two parameters of the parametric eqns. */
   double num, denom;  /* Numerator and denoninator of equations. */
   CODE_TYPE code;    /* Return char characterizing intersection. */

   denom = a[X] * (double)( d[Y] - c[Y] ) +
           b[X] * (double)( c[Y] - d[Y] ) +
           d[X] * (double)( b[Y] - a[Y] ) +
           c[X] * (double)( a[Y] - b[Y] );

   /* If denom is zero, then segments are parallel: handle separately. */
   if (AlmostZero(denom))
      return  ParallelInt(a, b, c, d, p, q);

   num =    a[X] * (double)( d[Y] - c[Y] ) +
            c[X] * (double)( a[Y] - d[Y] ) +
            d[X] * (double)( c[Y] - a[Y] );

   s = num / denom;
 
   num = -( a[X] * (double)( c[Y] - b[Y] ) +
            b[X] * (double)( a[Y] - c[Y] ) +
            c[X] * (double)( b[Y] - a[Y] ) );
   
   t = num / denom;
 
   if      ( (ALMOST_ZERO < s) && (s < 1.0-ALMOST_ZERO) &&
             (ALMOST_ZERO < t) && (t < 1.0-ALMOST_ZERO) )
     code = INTERSECT;
   else if ( (-ALMOST_ZERO > s) || (s > 1.0+ALMOST_ZERO) ||
             (-ALMOST_ZERO > t) || (t > 1.0+ALMOST_ZERO) )
     code = DISJOINT;
   else if ( AlmostZero(s) ) {
	 s =0;
	 if ( AlmostZero(t) ) 
	   code = FACE_VERTEX_P0_Q0;
     else if ( AlmostZero(1-t) ) 
       code = FACE_VERTEX_P0_Q1;
     else 
       code = VERTEX_P0;
   }
   else if ( AlmostZero(1-s) ) {
	 s =1;
	 if ( AlmostZero(t) ) 
       code = FACE_VERTEX_P1_Q0;
     else if ( AlmostZero(1-t) ) 
       code = FACE_VERTEX_P1_Q1;
     else 
       code = VERTEX_P1;
   }
   else if ( AlmostZero(t) ) {
     code = VERTEX_Q0;
   }
   else if ( AlmostZero(1-t)) {
     code = VERTEX_Q1;
   }

   
   p[X] = a[X] + s * ( b[X] - a[X] );
   p[Y] = a[Y] + s * ( b[Y] - a[Y] );

   return code;
}

CODE_TYPE   ParallelInt( tPointd a, tPointd b, tPointd c, tPointd d, tPointd p, tPointd q )
{

   if ( !Collinear( a, b, c) )
      return DISJOINT;

   if ( Between( a, b, c ) && Between( a, b, d ) ) {
      Assigndi( p, c );
      Assigndi( q, d );
      return EDGE_VERTEX_Q0_Q1;
   }
   if ( Between( c, d, a ) && Between( c, d, b ) ) {
      Assigndi( p, a );
      Assigndi( q, b );
      return EDGE_VERTEX_P0_P1;
   }
   if ( Between( a, b, c ) && Between( c, d, b ) ) {
      Assigndi( p, c );
      Assigndi( q, b );
      return EDGE_VERTEX_Q0_P1;
   }
   if ( Between( a, b, c ) && Between( c, d, a ) ) {
      Assigndi( p, c );
      Assigndi( q, a );
      return EDGE_VERTEX_Q0_P0;
   }
   if ( Between( a, b, d ) && Between( c, d, b ) ) {
      Assigndi( p, d );
      Assigndi( q, b );
      return EDGE_VERTEX_Q1_P1;
   }
   if ( Between( a, b, d ) && Between( c, d, a ) ) {
      Assigndi( p, d );
      Assigndi( q, a );
      return EDGE_VERTEX_Q1_P0;
   }

   return DISJOINT;
}

inline void	Assigndi( tPointd p, tPointd a )
{
   p[X] = a[X];
   p[Y] = a[Y];
}
/*---------------------------------------------------------------------
Returns true iff point c lies on the closed segement ab.
Assumes it is already known that abc are collinear.
---------------------------------------------------------------------*/
bool    Between( tPointd a, tPointd b, tPointd c )
{
   /* If ab not vertical, check betweenness on x; else on y. */
   if ( a[X] != b[X] )
      return ((a[X] <= c[X]) && (c[X] <= b[X])) ||
             ((a[X] >= c[X]) && (c[X] >= b[X]));
   else
      return ((a[Y] <= c[Y]) && (c[Y] <= b[Y])) ||
             ((a[Y] >= c[Y]) && (c[Y] >= b[Y]));
}

/*---------------------------------------------------------------------
Returns the dot product of the two input vectors.
---------------------------------------------------------------------*/
inline double  Dot( tPointd a, tPointd b )
{
    return  a[X]*b[X] + a[Y]*b[Y];
}

inline double SqrDist(tPointd& a, tPointd& b)
{
	double t1 = a[0]-b[0];
	double t2 = a[1]-b[1];
	return t1*t1+t2*t2;
}

inline double SqrDist(tPointd& a, double* b)
{
	double dot = b[0]*a[0] + b[1]*a[1] + b[2];
	double len = sqrt(a[0]*a[0] + a[1]*a[1] + 1);

	return fabs(dot/len);
}

inline void check_max(CODE_TYPE code, tPointd& p, int a1, int a, int b1, int b)
{
  double temp_max;

  if ( (temp_max = SqrDist(p, pref)) < min_dist ) {
    min_dist = temp_max;
    indices[0] = a1; indices[1] = a; 
    indices[2] = b1; indices[3] = b;
    res_code = code;
  }

}

void check_max(CODE_TYPE code,
	       tPointd& p, tPointd& q, int a1, int a, int b1, int b)
{
	
  double temp_max = SqrDist(p, pref);
  double temp_max2 = SqrDist(q, pref);

  if ( temp_max < min_dist && temp_max <= temp_max2 )
	  {
    
    switch(code) {
    case EDGE_VERTEX_Q0_Q1: 
    case EDGE_VERTEX_Q0_P1:
    case EDGE_VERTEX_Q0_P0:
		min_dist = temp_max;
		res_code = VERTEX_Q0;
		indices[2] = b1;
		break;

    case EDGE_VERTEX_P0_P1:
		min_dist = temp_max;
		res_code = VERTEX_P0;
		indices[0] = a1;
		break;

    case EDGE_VERTEX_Q1_P1:
    case EDGE_VERTEX_Q1_P0:
		min_dist = temp_max;
		res_code = VERTEX_Q1;
		indices[3] = b;
		break;
    }

  }
  else if ( temp_max2 < min_dist && temp_max2 < temp_max )
	  {
    
    switch(code) {
    case EDGE_VERTEX_Q0_Q1: 
		min_dist = temp_max2;
		res_code = VERTEX_Q1;
		indices[3] = b;
      break;
    case EDGE_VERTEX_Q0_P1:
    case EDGE_VERTEX_P0_P1:
    case EDGE_VERTEX_Q1_P1:
		min_dist = temp_max2;
		res_code = VERTEX_P1;
		indices[1] = a;
      break;
    case EDGE_VERTEX_Q0_P0:
    case EDGE_VERTEX_Q1_P0:
		min_dist = temp_max2;
		res_code = VERTEX_P0;
		indices[0] = a1;
      break;
    }
  }

}

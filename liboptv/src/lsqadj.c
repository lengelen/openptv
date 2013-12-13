#include "lsqadj.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* parts of code of adjlib.c from Horst Beyer, Hannes Kirndorfer */


/* Multiply transpose of a matrix A by matrix A itself, creating symmetric matrix
*  with the option of working with the sub-matrix only 
*
*   Arguments:
*   a - matrix of doubles of the size (m x n_large).
*   ata  - matrix of the result multiply(a.T,a) of size (n x n)
*   m - number of rows in matrix a
*   n - number of rows and columns in the output ata - the size of the sub-matrix
*   n_large - number of columns in matrix a
*/

void ata (double *a, double *ata, int m, int n, int n_large ) {
/* matrix a and result matrix ata = at a 
		       a is m * n, ata is n * n  */

  register int      i, j, k;
  
  for (i = 0; i < n; i++)
    {
      for (j = 0; j < n; j++)
	{
	  *(ata+i*n_large+j) = 0.0;
	  for (k = 0; k < m; k++)
	    *(ata+i*n_large+j) +=  *(a+k*n_large+i)  * *(a+k*n_large+j);
	}
    }
}



/* Multiply transpose of a matrix A by vector l , creating vector u
*  with the option of working with the sub-vector only, when n < n_large 
*
*   Arguments:
*   u - vector of doubles of the size (n x 1)
*   a - matrix of doubles of the size (m x n_large).
*   l  - vector of doubles (m x 1)
*   m - number of rows in matrix a
*   n - length of the output u - the size of the sub-matrix
*   n_large - number of columns in matrix a
*/

void atl (double *u, double *a, double *l, int m, int n, int n_large) {
/* matrix a , vector l and 
			 result vector u = at l ,  a(m,n)  */

  int      i, k;
  
  for (i = 0; i < n; i++)
    {
      *(u + i) = 0.0;
      for (k = 0; k < m; k++)
	*(u + i) += *(a + k * n_large + i) * *(l + k);
    }  
}


/* Calculate inverse of a matrix A,
*  with the option of working with the sub-vector only, when n < n_large 
*
*   Arguments:
*   a - matrix of doubles of the size (n_large x n_large).
*   n - size of the output - size of the sub-matrix, number of observations
*   n_large - number of rows and columns in matrix a
*/

void matinv (double *a, int n, int n_large) {
  int      ipiv, irow, icol;
  double   pivot;	/* pivot element = 1.0 / aii */
  double	npivot;	/*	negative of pivot */
  
  
  for (ipiv = 0; ipiv < n; ipiv++)
    {
      pivot = 1.0 / *(a + ipiv * n_large + ipiv);
      npivot = - pivot;
      for (irow = 0; irow < n; irow++)
	{
	  for (icol = 0; icol < n; icol++)
	    {
	      if (irow != ipiv && icol != ipiv)
		{
		  *(a + irow * n_large + icol) -= *(a + ipiv * n_large + icol) * 
		    *(a + irow * n_large + ipiv) * pivot;
		}
	    }
	}
      for (icol = 0; icol < n; icol++)
	{
	  if (ipiv != icol) 
	    *(a + ipiv * n_large + icol) *= npivot;
	}
      for (irow = 0; irow < n; irow++)
	{
	  if (ipiv != irow)
	    *(a + irow * n_large + ipiv) *= pivot;
	}
      *(a + ipiv * n_large + ipiv) = pivot;
    }
}	/* end matinv */


void matmul (double *a, double *b, double *c, int m, int n, int k) {  

int    i,j,l;
double  x,*pa,*pb,*pc;

for (i=0; i<k; i++)
  {  pb = b;
  pa = a++;
  for (j=0; j<m; j++)
    {  pc = c;
    x = 0.0;
    for (l=0; l<n; l++)
      {  x = x + *pb++ * *pc;
      pc += k;
      }
    *pa = x;
    pa += k;
    }
  c++;
  }
}



void matmul_v2 (double *a, double *b, double *c, int m,int n,int k,\
int m_large, int n_large) { 

int    i,j,l;
double  x,*pa,*pb,*pc;

for (i=0; i<k; i++) {  
  pb = b;
  pa = a++;
  for (j=0; j<m; j++) {  
    pc = c;
    x = 0.0;
    for (l=0; l<n; l++) {  
      x = x + *pb++ * *pc;
      pc += k;
      }
	for (l=0;l<n_large-n;l++) {
	  pb++;
	  pc += k;
	  }
    *pa = x;
    pa += k;
    }
  for (j=0;j<m_large-m;j++) {
    pa += k;
    }
  c++;
  }
}


void transp (double a[], int m, int n) {  
  double  *b,*c,*d,*e;
  int    i,j;
  
  b = (double*) malloc (m*n*sizeof(double));
  if (b == 0) goto err;
  d = a;
  e = b;
  
  for (i=0; i<m; i++)
    {  c = b++;
    for (j=0; j<n; j++)
      {  *c = *a++;
      c += m;
      }
    }
  
  for (i=0; i<m*n; i++)
    *d++ = *e++;
  /*
    free (b);
    */   
  return;
  
err:
  printf ("\n\n ***   no memory space in C-subroutine transp   ***");
  printf ("\n\n");
  exit (-1);
}

void mat_transpose (double *mat1, double *mat2, int m, int n) {
  int		i, j;
  for (i=0; i<m; i++){ 
  	for (j=0; j<n; j++){
  		*(mat2+j*m+i) = *(mat1+i*n+j);
  	}
  }
}


void norm_cross(double a[3], double b[3], double *n1, double *n2, double *n3) {

	double  res[3], dummy, norm;

	res[0]=a[1]*b[2]-a[2]*b[1];
	res[1]=a[2]*b[0]-a[0]*b[2];
	res[2]=a[0]*b[1]-a[1]*b[0];
	
	modu(res,&norm);
	
	
	if (norm == 0.0){ // avoids zero length vector bug
		*n1 = res[0];
		*n2 = res[0];
		*n3 = res[0];
	} else {	
	*n1=res[0]/norm;
	*n2=res[1]/norm;
	*n3=res[2]/norm;
	}
}

/* Dot product of two vectors 
* TODO: use ready subroutines from vec_utils.h
*
*/

void dot(double a[3], double b[3], double *d) {

	*d = a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}


/* Modulus of a vector
* TODO: use ready subroutine called norm in vec_utils.h
*/
void modu(double a[3], double *m) {

	*m = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
}



// A minimalistic FFT library
// Copyright (c) 2021 Alexander Mukhin
// MIT License

#include "minfft.h"
#include <stdlib.h>
#include <math.h>

// suffix for math functions
#if MINFFT_SINGLE
#define MATH_SFX f
#elif MINFFT_EXTENDED
#define MATH_SFX l
#else
#define MATH_SFX
#endif

// macros for calling math functions
#define T(F) T2(F,MATH_SFX)
#define T2(F,S) T22(F,S)
#define T22(F,S) F##S

// constants
static const minfft_real pi=(minfft_real)(3.141592653589793238462643383279502884L);
static const minfft_real sqrt2=(minfft_real)(1.414213562373095048801688724209698079L);
static const minfft_real invsqrt2=(minfft_real)(0.707106781186547524400844362104849039L);

// aux structure
struct minfft_aux {
	int N; // number of elements to transform
	void *t; // temporary buffer
	void *e; // exponent vector
	struct minfft_aux *sub1; // subtransform structure
	struct minfft_aux *sub2; // subtransform structure
};

// *** meta-functions ***

// a pointer to a strided 1d complex transform routine
typedef
void (*s_cx_1d_t)
(minfft_cmpl*,minfft_cmpl*,int,const minfft_aux*);

// make a strided any-dimensional complex transform
// by repeated application of its strided one-dimensional routine
inline static void
mkcx (minfft_cmpl *x, minfft_cmpl *y, int sy, const minfft_aux *a, s_cx_1d_t s_1d) {
	if (a->sub2==NULL)
		(*s_1d)(x,y,sy,a);
	else {
		int N1=a->sub1->N,N2=a->sub2->N; // transform lengths
		int n; // counter
		minfft_cmpl *t=a->t; // temporary buffer
		// strided transform of contiguous hyperplanes
		for (n=0; n<N2; ++n)
			mkcx(x+n*N1,t+n,N2,a->sub1,s_1d);
		// strided transform of contiguous rows
		for (n=0; n<N1; ++n)
			(*s_1d)(t+n*N2,y+sy*n,sy*N1,a->sub2);
	}
}

// a pointer to a strided 1d real transform routine
typedef
void (*s_rx_1d_t)
(minfft_real*,minfft_real*,int,const minfft_aux*);

// make a strided any-dimensional real transform
// by repeated application of its strided one-dimensional routine
inline static void
mkrx (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a, s_rx_1d_t s_1d) {
	if (a->sub2==NULL)
		(*s_1d)(x,y,sy,a);
	else {
		int N1=a->sub1->N,N2=a->sub2->N; // transform lengths
		int n; // counter
		minfft_real *t=a->t; // temporary buffer
		// strided transform of contiguous hyperplanes
		for (n=0; n<N2; ++n)
			mkrx(x+n*N1,t+n,N2,a->sub1,s_1d);
		// strided transform of contiguous rows
		for (n=0; n<N1; ++n)
			(*s_1d)(t+n*N2,y+sy*n,sy*N1,a->sub2);
	}
}

// *** complex transforms ***

// recursive strided one-dimensional DFT
inline static void
rs_dft_1d (int N, minfft_cmpl *x, minfft_cmpl *t, minfft_cmpl *y, int sy, const minfft_cmpl *e) {
	int n; // counter
	// split-radix DIF
	if (N==1) {
		// terminal case
		minfft_real *xr=(minfft_real*)x,*yr=(minfft_real*)y;
		minfft_real *xi=xr+1,*yi=yr+1;
		// y[0]=x[0];
		yr[0]=xr[0];
		yi[0]=xi[0];
		return;
	}
	if (N==2) {
		// terminal case
		minfft_real *xr=(minfft_real*)x,*yr=(minfft_real*)y;
		minfft_real *xi=xr+1,*yi=yr+1;
		register minfft_real t0r,t1r;
		register minfft_real t0i,t1i;
		// t0=x[0]+x[1];
		t0r=xr[0]+xr[2];
		t0i=xi[0]+xi[2];
		// t1=x[0]-x[1];
		t1r=xr[0]-xr[2];
		t1i=xi[0]-xi[2];
		// y[0]=t0;
		yr[0]=t0r;
		yi[0]=t0i;
		// y[sy]=t1;
		yr[2*sy]=t1r;
		yi[2*sy]=t1i;
		return;
	}
	if (N==4) {
		// terminal case
		minfft_real *xr=(minfft_real*)x,*yr=(minfft_real*)y;
		minfft_real *xi=xr+1,*yi=yr+1;
		register minfft_real t0r,t1r,t2r,t3r;
		register minfft_real t0i,t1i,t2i,t3i;
		// t0=x[0]+x[2];
		t0r=xr[0]+xr[4];
		t0i=xi[0]+xi[4];
		// t1=x[1]+x[3];
		t1r=xr[2]+xr[6];
		t1i=xi[2]+xi[6];
		// t2=x[0]-x[2];
		t2r=xr[0]-xr[4];
		t2i=xi[0]-xi[4];
		// t3=I*(x[1]-x[3]);
		t3r=-xi[2]+xi[6];
		t3i=xr[2]-xr[6];
		// y[0]=t0+t1;
		yr[0]=t0r+t1r;
		yi[0]=t0i+t1i;
		// y[sy]=t2-t3;
		yr[2*sy]=t2r-t3r;
		yi[2*sy]=t2i-t3i;
		// y[2*sy]=t0-t1;
		yr[4*sy]=t0r-t1r;
		yi[4*sy]=t0i-t1i;
		// y[3*sy]=t2+t3;
		yr[6*sy]=t2r+t3r;
		yi[6*sy]=t2i+t3i;
		return;
	}
	if (N==8) {
		// terminal case
		minfft_real *xr=(minfft_real*)x,*yr=(minfft_real*)y;
		minfft_real *xi=xr+1,*yi=yr+1;
		register minfft_real t0r,t1r,t2r,t3r;
		register minfft_real t0i,t1i,t2i,t3i;
		register minfft_real t00r,t01r,t02r,t03r;
		register minfft_real t00i,t01i,t02i,t03i;
		register minfft_real t10r,t11r,t12r,t13r;
		register minfft_real t10i,t11i,t12i,t13i;
		register minfft_real ttr,tti;
		// t0=x[0]+x[4];
		t0r=xr[0]+xr[8];
		t0i=xi[0]+xi[8];
		// t1=x[2]+x[6];
		t1r=xr[4]+xr[12];
		t1i=xi[4]+xi[12];
		// t2=x[0]-x[4];
		t2r=xr[0]-xr[8];
		t2i=xi[0]-xi[8];
		// t3=I*(x[2]-x[6]);
		t3r=-xi[4]+xi[12];
		t3i=xr[4]-xr[12];
		// t00=t0+t1;
		t00r=t0r+t1r;
		t00i=t0i+t1i;
		// t01=t2-t3;
		t01r=t2r-t3r;
		t01i=t2i-t3i;
		// t02=t0-t1;
		t02r=t0r-t1r;
		t02i=t0i-t1i;
		// t03=t2+t3;
		t03r=t2r+t3r;
		t03i=t2i+t3i;
		// t0=x[1]+x[5];
		t0r=xr[2]+xr[10];
		t0i=xi[2]+xi[10];
		// t1=x[3]+x[7];
		t1r=xr[6]+xr[14];
		t1i=xi[6]+xi[14];
		// t2=x[1]-x[5];
		t2r=xr[2]-xr[10];
		t2i=xi[2]-xi[10];
		// t3=I*(x[3]-x[7]);
		t3r=-xi[6]+xi[14];
		t3i=xr[6]-xr[14];
		// t10=t0+t1;
		t10r=t0r+t1r;
		t10i=t0i+t1i;
		// t11=(t2-t3)*invsqrt2*(1-I);
		ttr=t2r-t3r;
		tti=t2i-t3i;
		t11r=invsqrt2*(ttr+tti);
		t11i=invsqrt2*(tti-ttr);
		// t12=(t0-t1)*(-I);
		t12r=t0i-t1i;
		t12i=-t0r+t1r;
		// t13=(t2+t3)*invsqrt2*(-1-I);
		ttr=t2r+t3r;
		tti=t2i+t3i;
		t13r=invsqrt2*(tti-ttr);
		t13i=-invsqrt2*(tti+ttr);
		// y[0]=t00+t10;
		yr[0]=t00r+t10r;
		yi[0]=t00i+t10i;
		// y[sy]=t01+t11;
		yr[2*sy]=t01r+t11r;
		yi[2*sy]=t01i+t11i;
		// y[2*sy]=t02+t12;
		yr[4*sy]=t02r+t12r;
		yi[4*sy]=t02i+t12i;
		// y[3*sy]=t03+t13;
		yr[6*sy]=t03r+t13r;
		yi[6*sy]=t03i+t13i;
		// y[4*sy]=t00-t10;
		yr[8*sy]=t00r-t10r;
		yi[8*sy]=t00i-t10i;
		// y[5*sy]=t01-t11;
		yr[10*sy]=t01r-t11r;
		yi[10*sy]=t01i-t11i;
		// y[6*sy]=t02-t12;
		yr[12*sy]=t02r-t12r;
		yi[12*sy]=t02i-t12i;
		// y[7*sy]=t03-t13;
		yr[14*sy]=t03r-t13r;
		yi[14*sy]=t03i-t13i;
		return;
	}
	// recursion
	minfft_real *xr=(minfft_real*)x,*tr=(minfft_real*)t,*er=(minfft_real*)e;
	minfft_real *xi=xr+1,*ti=tr+1,*ei=er+1;
	// prepare sub-transform inputs
	for (n=0; n<N/4; ++n) {
		register minfft_real t0r,t1r,t2r,t3r;
		register minfft_real t0i,t1i,t2i,t3i;
		// t0=x[n]+x[n+N/2];
		t0r=xr[2*n]+xr[2*n+N];
		t0i=xi[2*n]+xi[2*n+N];
		// t1=x[n+N/4]+x[n+3*N/4];
		t1r=xr[2*n+N/2]+xr[2*n+3*N/2];
		t1i=xi[2*n+N/2]+xi[2*n+3*N/2];
		// t2=x[n]-x[n+N/2];
		t2r=xr[2*n]-xr[2*n+N];
		t2i=xi[2*n]-xi[2*n+N];
		// t3=I*(x[n+N/4]-x[n+3*N/4]);
		t3r=-xi[2*n+N/2]+xi[2*n+3*N/2];
		t3i=xr[2*n+N/2]-xr[2*n+3*N/2];
		// t[n]=t0;
		tr[2*n]=t0r;
		ti[2*n]=t0i;
		// t[n+N/4]=t1;
		tr[2*n+N/2]=t1r;
		ti[2*n+N/2]=t1i;
		// t[n+N/2]=(t2-t3)*e[2*n];
		t0r=t2r-t3r;
		t0i=t2i-t3i;
		t1r=t2r+t3r;
		t1i=t2i+t3i;
		tr[2*n+N]=t0r*er[4*n]-t0i*ei[4*n];
		ti[2*n+N]=t0r*ei[4*n]+t0i*er[4*n];
		// t[n+3*N/4]=(t2+t3)*e[2*n+1];
		tr[2*n+3*N/2]=t1r*er[4*n+2]-t1i*ei[4*n+2];
		ti[2*n+3*N/2]=t1r*ei[4*n+2]+t1i*er[4*n+2];
	}
	// call sub-transforms
	rs_dft_1d(N/2,t,t,y,2*sy,e+N/2);
	rs_dft_1d(N/4,t+N/2,t+N/2,y+sy,4*sy,e+3*N/4);
	rs_dft_1d(N/4,t+3*N/4,t+3*N/4,y+3*sy,4*sy,e+3*N/4);
}

// strided one-dimensional DFT
inline static void
s_dft_1d (minfft_cmpl *x, minfft_cmpl *y, int sy, const minfft_aux *a) {
	rs_dft_1d(a->N,x,a->t,y,sy,a->e);
}

// strided DFT of arbitrary dimension
inline static void
s_dft (minfft_cmpl *x, minfft_cmpl *y, int sy, const minfft_aux *a) {
	mkcx(x,y,sy,a,s_dft_1d);
}

// user interface
void
minfft_dft (minfft_cmpl *x, minfft_cmpl *y, const minfft_aux *a) {
	s_dft(x,y,1,a);
}

// recursive strided one-dimensional inverse DFT
inline static void
rs_invdft_1d (int N, minfft_cmpl *x, minfft_cmpl *t, minfft_cmpl *y, int sy, const minfft_cmpl *e) {
	int n; // counter
	// split-radix DIF
	if (N==1) {
		// terminal case
		minfft_real *xr=(minfft_real*)x,*yr=(minfft_real*)y;
		minfft_real *xi=xr+1,*yi=yr+1;
		// y[0]=x[0];
		yr[0]=xr[0];
		yi[0]=xi[0];
		return;
	}
	if (N==2) {
		// terminal case
		minfft_real *xr=(minfft_real*)x,*yr=(minfft_real*)y;
		minfft_real *xi=xr+1,*yi=yr+1;
		register minfft_real t0r,t1r;
		register minfft_real t0i,t1i;
		// t0=x[0]+x[1];
		t0r=xr[0]+xr[2];
		t0i=xi[0]+xi[2];
		// t1=x[0]-x[1];
		t1r=xr[0]-xr[2];
		t1i=xi[0]-xi[2];
		// y[0]=t0;
		yr[0]=t0r;
		yi[0]=t0i;
		// y[sy]=t1;
		yr[2*sy]=t1r;
		yi[2*sy]=t1i;
		return;
	}
	if (N==4) {
		// terminal case
		minfft_real *xr=(minfft_real*)x,*yr=(minfft_real*)y;
		minfft_real *xi=xr+1,*yi=yr+1;
		register minfft_real t0r,t1r,t2r,t3r;
		register minfft_real t0i,t1i,t2i,t3i;
		// t0=x[0]+x[2];
		t0r=xr[0]+xr[4];
		t0i=xi[0]+xi[4];
		// t1=x[1]+x[3];
		t1r=xr[2]+xr[6];
		t1i=xi[2]+xi[6];
		// t2=x[0]-x[2];
		t2r=xr[0]-xr[4];
		t2i=xi[0]-xi[4];
		// t3=I*(x[1]-x[3]);
		t3r=-xi[2]+xi[6];
		t3i=xr[2]-xr[6];
		// y[0]=t0+t1;
		yr[0]=t0r+t1r;
		yi[0]=t0i+t1i;
		// y[sy]=t2+t3;
		yr[2*sy]=t2r+t3r;
		yi[2*sy]=t2i+t3i;
		// y[2*sy]=t0-t1;
		yr[4*sy]=t0r-t1r;
		yi[4*sy]=t0i-t1i;
		// y[3*sy]=t2-t3;
		yr[6*sy]=t2r-t3r;
		yi[6*sy]=t2i-t3i;
		return;
	}
	if (N==8) {
		// terminal case
		minfft_real *xr=(minfft_real*)x,*yr=(minfft_real*)y;
		minfft_real *xi=xr+1,*yi=yr+1;
		register minfft_real t0r,t1r,t2r,t3r;
		register minfft_real t0i,t1i,t2i,t3i;
		register minfft_real t00r,t01r,t02r,t03r;
		register minfft_real t00i,t01i,t02i,t03i;
		register minfft_real t10r,t11r,t12r,t13r;
		register minfft_real t10i,t11i,t12i,t13i;
		register minfft_real ttr,tti;
		// t0=x[0]+x[4];
		t0r=xr[0]+xr[8];
		t0i=xi[0]+xi[8];
		// t1=x[2]+x[6];
		t1r=xr[4]+xr[12];
		t1i=xi[4]+xi[12];
		// t2=x[0]-x[4];
		t2r=xr[0]-xr[8];
		t2i=xi[0]-xi[8];
		// t3=I*(x[2]-x[6]);
		t3r=-xi[4]+xi[12];
		t3i=xr[4]-xr[12];
		// t00=t0+t1;
		t00r=t0r+t1r;
		t00i=t0i+t1i;
		// t01=t2+t3;
		t01r=t2r+t3r;
		t01i=t2i+t3i;
		// t02=t0-t1;
		t02r=t0r-t1r;
		t02i=t0i-t1i;
		// t03=t2-t3;
		t03r=t2r-t3r;
		t03i=t2i-t3i;
		// t0=x[1]+x[5];
		t0r=xr[2]+xr[10];
		t0i=xi[2]+xi[10];
		// t1=x[3]+x[7];
		t1r=xr[6]+xr[14];
		t1i=xi[6]+xi[14];
		// t2=x[1]-x[5];
		t2r=xr[2]-xr[10];
		t2i=xi[2]-xi[10];
		// t3=I*(x[3]-x[7]);
		t3r=-xi[6]+xi[14];
		t3i=xr[6]-xr[14];
		// t10=t0+t1;
		t10r=t0r+t1r;
		t10i=t0i+t1i;
		// t11=(t2+t3)*invsqrt2*(1+I);
		ttr=t2r+t3r;
		tti=t2i+t3i;
		t11r=invsqrt2*(ttr-tti);
		t11i=invsqrt2*(ttr+tti);
		// t12=(t0-t1)*I;
		t12r=-t0i+t1i;
		t12i=t0r-t1r;
		// t13=(t2-t3)*invsqrt2*(-1+I);
		ttr=t2r-t3r;
		tti=t2i-t3i;
		t13r=-invsqrt2*(ttr+tti);
		t13i=invsqrt2*(ttr-tti);
		// y[0]=t00+t10;
		yr[0]=t00r+t10r;
		yi[0]=t00i+t10i;
		// y[sy]=t01+t11;
		yr[2*sy]=t01r+t11r;
		yi[2*sy]=t01i+t11i;
		// y[2*sy]=t02+t12;
		yr[4*sy]=t02r+t12r;
		yi[4*sy]=t02i+t12i;
		// y[3*sy]=t03+t13;
		yr[6*sy]=t03r+t13r;
		yi[6*sy]=t03i+t13i;
		// y[4*sy]=t00-t10;
		yr[8*sy]=t00r-t10r;
		yi[8*sy]=t00i-t10i;
		// y[5*sy]=t01-t11;
		yr[10*sy]=t01r-t11r;
		yi[10*sy]=t01i-t11i;
		// y[6*sy]=t02-t12;
		yr[12*sy]=t02r-t12r;
		yi[12*sy]=t02i-t12i;
		// y[7*sy]=t03-t13;
		yr[14*sy]=t03r-t13r;
		yi[14*sy]=t03i-t13i;
		return;
	}
	// recursion
	minfft_real *xr=(minfft_real*)x,*tr=(minfft_real*)t,*er=(minfft_real*)e;
	minfft_real *xi=xr+1,*ti=tr+1,*ei=er+1;
	// prepare sub-transform inputs
	for (n=0; n<N/4; ++n) {
		register minfft_real t0r,t1r,t2r,t3r;
		register minfft_real t0i,t1i,t2i,t3i;
		// t0=x[n]+x[n+N/2];
		t0r=xr[2*n]+xr[2*n+N];
		t0i=xi[2*n]+xi[2*n+N];
		// t1=x[n+N/4]+x[n+3*N/4];
		t1r=xr[2*n+N/2]+xr[2*n+3*N/2];
		t1i=xi[2*n+N/2]+xi[2*n+3*N/2];
		// t2=x[n]-x[n+N/2];
		t2r=xr[2*n]-xr[2*n+N];
		t2i=xi[2*n]-xi[2*n+N];
		// t3=I*(x[n+N/4]-x[n+3*N/4]);
		t3r=-xi[2*n+N/2]+xi[2*n+3*N/2];
		t3i=xr[2*n+N/2]-xr[2*n+3*N/2];
		// t[n]=t0;
		tr[2*n]=t0r;
		ti[2*n]=t0i;
		// t[n+N/4]=t1;
		tr[2*n+N/2]=t1r;
		ti[2*n+N/2]=t1i;
		// t[n+N/2]=(t2+t3)*conj(e[2*n]);
		t0r=t2r+t3r;
		t0i=t2i+t3i;
		t1r=t2r-t3r;
		t1i=t2i-t3i;
		tr[2*n+N]=t0r*er[4*n]+t0i*ei[4*n];
		ti[2*n+N]=-t0r*ei[4*n]+t0i*er[4*n];
		// t[n+3*N/4]=(t2-t3)*conj(e[2*n+1]);
		tr[2*n+3*N/2]=t1r*er[4*n+2]+t1i*ei[4*n+2];
		ti[2*n+3*N/2]=-t1r*ei[4*n+2]+t1i*er[4*n+2];
	}
	// call sub-transforms
	rs_invdft_1d(N/2,t,t,y,2*sy,e+N/2);
	rs_invdft_1d(N/4,t+N/2,t+N/2,y+sy,4*sy,e+3*N/4);
	rs_invdft_1d(N/4,t+3*N/4,t+3*N/4,y+3*sy,4*sy,e+3*N/4);
}

// strided one-dimensional inverse DFT
inline static void
s_invdft_1d (minfft_cmpl *x, minfft_cmpl *y, int sy, const minfft_aux *a) {
	rs_invdft_1d(a->N,x,a->t,y,sy,a->e);
}

// strided inverse DFT of arbitrary dimension
inline static void
s_invdft (minfft_cmpl *x, minfft_cmpl *y, int sy, const minfft_aux *a) {
	mkcx(x,y,sy,a,s_invdft_1d);
}

// user interface
void
minfft_invdft (minfft_cmpl *x, minfft_cmpl *y, const minfft_aux *a) {
	s_invdft(x,y,1,a);
}

// *** real transforms ***

// strided one-dimensional real DFT
inline static void
s_realdft_1d (minfft_real *x, minfft_cmpl *z, int sz, const minfft_aux *a) {
	int n; // counter
	int N=a->N; // transform length
	minfft_cmpl *e=a->e; // exponent vector
	minfft_cmpl *w=(minfft_cmpl*)x; // alias
	minfft_cmpl *t=a->t; // temporary buffer
	minfft_real *zr=(minfft_real*)z;
	minfft_real *zi=zr+1;
	if (N==1) {
		// trivial case
		zr[0]=x[0];
		zi[0]=0;
		return;
	}
	if (N==2) {
		// trivial case
		register minfft_real t0,t1;
		t0=x[0];
		t1=x[1];
		// z[0]=t0+t1;
		zr[0]=t0+t1;
		zi[0]=0;
		// z[sz]=t0-t1;
		zr[2*sz]=t0-t1;
		zi[2*sz]=0;
		return;
	}
	// reduce to complex DFT of length N/2
	// do complex DFT
	s_dft_1d(w,t,1,a->sub1);
	// recover results
	register minfft_real ur,vr;
	register minfft_real ui,vi;
	minfft_real *tr=(minfft_real*)t,*er=(minfft_real*)e;
	minfft_real *ti=tr+1,*ei=er+1;
	// u=t[0];
	ur=tr[0];
	ui=ti[0];
	// z[0]=creal(u)+cimag(u);
	zr[0]=ur+ui;
	zi[0]=0;
	// z[sz*N/2]=creal(u)-cimag(u);
	zr[sz*N]=ur-ui;
	zi[sz*N]=0;
	for (n=1; n<N/4; ++n) {
		register minfft_real ttr,ter;
		register minfft_real tti,tei;
		// u=(t[n]+conj(t[N/2-n]))/2;
		ur=(tr[2*n]+tr[N-2*n])/2;
		ui=(ti[2*n]-ti[N-2*n])/2;
		// v=(t[n]-conj(t[N/2-n]))*e[n]/(2*I);
		ttr=tr[2*n]-tr[N-2*n];
		tti=ti[2*n]+ti[N-2*n];
		ter=ei[2*n]; // te=e[n]/I
		tei=-er[2*n];
		vr=(ttr*ter-tti*tei)/2;
		vi=(ttr*tei+tti*ter)/2;
		// z[sz*n]=u+v;
		zr[2*sz*n]=ur+vr;
		zi[2*sz*n]=ui+vi;
		// z[sz*(N/2-n)]=conj(u-v);
		zr[sz*(N-2*n)]=ur-vr;
		zi[sz*(N-2*n)]=-ui+vi;
	}
	// z[sz*N/4]=conj(t[N/4]);
	zr[sz*N/2]=tr[N/2];
	zi[sz*N/2]=-ti[N/2];
}

// real DFT of arbitrary dimension
void
minfft_realdft (minfft_real *x, minfft_cmpl *z, const minfft_aux *a) {
	if (a->sub2==NULL)
		s_realdft_1d(x,z,1,a);
	else {
		int N1=a->sub1->N,N2=a->sub2->N; // transform lengths
		int n; // counter
		minfft_cmpl *t=a->t; // temporary buffer
		// strided real DFT of contiguous rows
		for (n=0; n<N2; ++n)
			s_realdft_1d(x+n*N1,t+n,N2,a->sub1);
		// strided complex DFT of contiguous hyperplanes
		for (n=0; n<N1/2+1; ++n)
			s_dft(t+n*N2,z+n,N1/2+1,a->sub2);
	}
}

// one-dimensional inverse real DFT
inline static void
invrealdft_1d (minfft_cmpl *z, minfft_real *y, const minfft_aux *a) {
	int n; // counter
	int N=a->N; // transform length
	minfft_cmpl *e=a->e; // exponent vector
	minfft_cmpl *w=(minfft_cmpl*)y; // alias
	minfft_cmpl *t=a->t; // temporary buffer
	minfft_real *zr=(minfft_real*)z;
	minfft_real *zi=zr+1;
	if (N==1) {
		// trivial case
		y[0]=zr[0];
		return;
	}
	if (N==2) {
		// trivial case
		register minfft_real t0,t1; // temporary values
		t0=zr[0];
		t1=zr[2];
		y[0]=t0+t1;
		y[1]=t0-t1;
		return;
	}
	// reduce to inverse complex DFT of length N/2
	// prepare complex DFT inputs
	minfft_real *tr=(minfft_real*)t,*er=(minfft_real*)e;
	minfft_real *ti=tr+1,*ei=er+1;
	// t[0]=(z[0]+z[N/2])+I*(z[0]-z[N/2]);
	tr[0]=zr[0]+zr[N];
	ti[0]=zr[0]-zr[N];
	for (n=1; n<N/4; ++n) {
		register minfft_real ur,vr;
		register minfft_real ui,vi;
		register minfft_real ttr,ter;
		register minfft_real tti,tei;
		// u=z[n]+conj(z[N/2-n]);
		ur=zr[2*n]+zr[N-2*n];
		ui=zi[2*n]-zi[N-2*n];
		// v=I*(z[n]-conj(z[N/2-n]))*conj(e[n]);
		ttr=zr[2*n]-zr[N-2*n];
		tti=zi[2*n]+zi[N-2*n];
		ter=ei[2*n]; // te=I*conj(e[n])
		tei=er[2*n];
		vr=ttr*ter-tti*tei;
		vi=ttr*tei+tti*ter;
		// t[n]=u+v;
		tr[2*n]=ur+vr;
		ti[2*n]=ui+vi;
		// t[N/2-n]=conj(u-v);
		tr[N-2*n]=ur-vr;
		ti[N-2*n]=-ui+vi;
	}
	// t[N/4]=2*conj(z[N/4]);
	tr[N/2]=2*zr[N/2];
	ti[N/2]=-2*zi[N/2];
	// do inverse complex DFT
	s_invdft_1d(t,w,1,a->sub1);
}

// inverse real DFT of arbitrary dimension
void
minfft_invrealdft (minfft_cmpl *z, minfft_real *y, const minfft_aux *a) {
	if (a->sub2==NULL)
		invrealdft_1d(z,y,a);
	else {
		int N1=a->sub1->N,N2=a->sub2->N; // transform lengths
		int n; // counter
		minfft_cmpl *t=a->t; // temporary buffer
		minfft_real *zr=(minfft_real*)z,*tr=(minfft_real*)t;
		minfft_real *zi=zr+1,*ti=tr+1;
		int k;
		// transpose
		for (n=0; n<N2; ++n)
			for (k=0; k<N1/2+1; ++k) {
				// t[n+N2*k]=z[(N1/2+1)*n+k];
				tr[2*n+2*N2*k]=zr[2*(N1/2+1)*n+2*k];
				ti[2*n+2*N2*k]=zi[2*(N1/2+1)*n+2*k];
			}
		// strided complex DFT of contiguous hyperplanes
		for (n=0; n<N1/2+1; ++n)
			s_invdft(t+n*N2,z+n,N1/2+1,a->sub2);
		// inverse real DFT of contiguous rows
		for (n=0; n<N2; ++n)
			invrealdft_1d(z+n*(N1/2+1),y+n*N1,a->sub1);
	}
}

// *** real symmetric transforms ***

// strided one-dimensional DCT-2
inline static void
s_dct2_1d (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a) {
	int n; // counter
	int N=a->N; // transform length
	minfft_real *t=a->t; // temporary buffer
	minfft_cmpl *z=(minfft_cmpl*)t; // its alias
	minfft_cmpl *e=a->e; // exponent vector
	if (N==1) {
		// trivial case
		y[0]=2*x[0];
		return;
	}
	// reduce to real DFT of length N
	// prepare sub-transform inputs
	for (n=0; n<N/2; ++n) {
		t[n]=x[2*n];
		t[N/2+n]=x[N-1-2*n];
	}
        // do real DFT in-place
	s_realdft_1d(t,z,1,a->sub1);
	// recover results
	minfft_real *er=(minfft_real*)e;
	minfft_real *ei=er+1;
	// y[0]=2*creal(z[0]);
	y[0]=2*t[0];
	for (n=1; n<N/2; ++n) {
		// y[sy*n]=2*creal(z[n]*e[n]);
		y[sy*n]=2*(t[2*n]*er[2*n]-t[2*n+1]*ei[2*n]);
		// y[sy*(N-n)]=-2*cimag(z[n]*e[n]);
		y[sy*(N-n)]=-2*(t[2*n]*ei[2*n]+t[2*n+1]*er[2*n]);
	}
	// y[sy*N/2]=sqrt2*creal(z[N/2]);
	y[sy*N/2]=sqrt2*t[N];
}

// strided DCT-2 of arbitrary dimension
inline static void
s_dct2 (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a) {
	mkrx(x,y,sy,a,s_dct2_1d);
}

// user interface
void
minfft_dct2 (minfft_real *x, minfft_real *y, const minfft_aux *a) {
	s_dct2(x,y,1,a);
}

// strided one-dimensional DST-2
inline static void
s_dst2_1d (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a) {
	int n; // counter
	int N=a->N; // transform length
	minfft_real *t=a->t; // temporary buffer
	minfft_cmpl *z=(minfft_cmpl*)t; // its alias
	minfft_cmpl *e=a->e; // exponent vector
	if (N==1) {
		// trivial case
		y[0]=2*x[0];
		return;
	}
	// reduce to real DFT of length N
	// prepare sub-transform inputs
	for (n=0; n<N/2; ++n) {
		t[n]=x[2*n];
		t[N/2+n]=-x[N-1-2*n];
	}
        // do real DFT in-place
	s_realdft_1d(t,z,1,a->sub1);
	// recover results
	minfft_real *er=(minfft_real*)e;
	minfft_real *ei=er+1;
	// y[sy*(N-1)]=2*creal(z[0]);
	y[sy*(N-1)]=2*t[0];
	for (n=1; n<N/2; ++n) {
		// y[sy*(n-1)]=-2*cimag(z[n]*e[n]);
		y[sy*(n-1)]=-2*(t[2*n]*ei[2*n]+t[2*n+1]*er[2*n]);
		// y[sy*(N-n-1)]=2*creal(z[n]*e[n]);
		y[sy*(N-n-1)]=2*(t[2*n]*er[2*n]-t[2*n+1]*ei[2*n]);
	}
	// y[sy*(N/2-1)]=sqrt2*creal(z[N/2]);
	y[sy*(N/2-1)]=sqrt2*t[N];
}

// strided DST-2 of arbitrary dimension
inline static void
s_dst2 (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a) {
	mkrx(x,y,sy,a,s_dst2_1d);
}

// user interface
void
minfft_dst2 (minfft_real *x, minfft_real *y, const minfft_aux *a) {
	s_dst2(x,y,1,a);
}

// strided one-dimensional DCT-3
inline static void
s_dct3_1d (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a) {
	int n; // counter
	int N=a->N; // transform length
	minfft_cmpl *z=a->t; // temporary buffer
	minfft_real *t=(minfft_real*)z; // its alias
	minfft_cmpl *e=a->e; // exponent vector
	if (N==1) {
		// trivial case
		y[0]=x[0];
		return;
	}
	// reduce to inverse real DFT of length N
	// prepare sub-transform inputs
	minfft_real *er=(minfft_real*)e,*zr=(minfft_real*)z;
	minfft_real *ei=er+1,*zi=zr+1;
	// z[0]=x[0];
	zr[0]=x[0];
	zi[0]=0;
	for (n=1; n<N/2; ++n) {
		// z[n]=conj((x[n]+I*x[N-n])*e[n]);
		zr[2*n]=x[n]*er[2*n]-x[N-n]*ei[2*n];
		zi[2*n]=-x[n]*ei[2*n]-x[N-n]*er[2*n];
	}
	// z[N/2]=sqrt2*x[N/2];
	zr[N]=sqrt2*x[N/2];
	zi[N]=0;
	// do inverse real DFT in-place
	invrealdft_1d(z,t,a->sub1);
	// recover results
	for (n=0; n<N/2; ++n) {
		y[sy*2*n]=t[n];
		y[sy*(N-1-2*n)]=t[N/2+n];
	}
}

// strided DCT-3 of arbitrary dimension
inline static void
s_dct3 (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a) {
	mkrx(x,y,sy,a,s_dct3_1d);
}

// user interface
void
minfft_dct3 (minfft_real *x, minfft_real *y, const minfft_aux *a) {
	s_dct3(x,y,1,a);
}

// strided one-dimensional DST-3
inline static void
s_dst3_1d (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a) {
	int n; // counter
	int N=a->N; // transform length
	minfft_cmpl *z=a->t; // temporary buffer
	minfft_real *t=(minfft_real*)z; // its alias
	minfft_cmpl *e=a->e; // exponent vector
	if (N==1) {
		// trivial case
		y[0]=x[0];
		return;
	}
	// reduce to inverse real DFT of length N
	// prepare sub-transform inputs
	minfft_real *er=(minfft_real*)e,*zr=(minfft_real*)z;
	minfft_real *ei=er+1,*zi=zr+1;
	// z[0]=x[N-1];
	zr[0]=x[N-1];
	zi[0]=0;
	for (n=1; n<N/2; ++n) {
		// z[n]=conj((x[N-n-1]+I*x[n-1])*e[n]);
		zr[2*n]=x[N-n-1]*er[2*n]-x[n-1]*ei[2*n];
		zi[2*n]=-x[N-n-1]*ei[2*n]-x[n-1]*er[2*n];
	}
	// z[N/2]=sqrt2*x[N/2-1];
	zr[N]=sqrt2*x[N/2-1];
	zi[N]=0;
	// do inverse real DFT in-place
	invrealdft_1d(z,t,a->sub1);
	// recover results
	for (n=0; n<N/2; ++n) {
		y[sy*2*n]=t[n];
		y[sy*(N-1-2*n)]=-t[N/2+n];
	}
}

// strided DST-3 of arbitrary dimension
inline static void
s_dst3 (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a) {
	mkrx(x,y,sy,a,s_dst3_1d);
}

// user interface
void
minfft_dst3 (minfft_real *x, minfft_real *y, const minfft_aux *a) {
	s_dst3(x,y,1,a);
}

// strided one-dimensional DCT-4
inline static void
s_dct4_1d (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a) {
	int n; // counter
	int N=a->N; // transform length
	minfft_cmpl *t=a->t; // temporary buffer
	minfft_cmpl *e=a->e; // exponent vector
	if (N==1) {
		// trivial case
		y[0]=sqrt2*x[0];
		return;
	}
	// reduce to complex DFT of length N/2
	// prepare sub-transform inputs
	minfft_real *tr=(minfft_real*)t,*er=(minfft_real*)e;
	minfft_real *ti=tr+1,*ei=er+1;
	for (n=0; n<N/2; ++n) {
		// t[n]=*e++*(x[2*n]+I*x[N-1-2*n]);
		tr[2*n]=er[2*n]*x[2*n]-ei[2*n]*x[N-1-2*n];
		ti[2*n]=er[2*n]*x[N-1-2*n]+ei[2*n]*x[2*n];
	}
	// do complex DFT in-place
	s_dft_1d(t,t,1,a->sub1);
	// recover results
	er+=N;
	ei+=N;
	for (n=0; n<N/2; ++n) {
		// y[sy*2*n]=2*creal(*e++*t[n]);
		y[sy*2*n]=2*(er[4*n]*tr[2*n]-ei[4*n]*ti[2*n]);
		// y[sy*(2*n+1)]=2*creal(*e++*conj(t[N/2-1-n]));
		y[sy*(2*n+1)]=2*(er[4*n+2]*tr[N-2-2*n]+ei[4*n+2]*ti[N-2-2*n]);
	}
}

// strided DCT-4 of arbitrary dimension
inline static void
s_dct4 (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a) {
	mkrx(x,y,sy,a,s_dct4_1d);
}

// user interface
void
minfft_dct4 (minfft_real *x, minfft_real *y, const minfft_aux *a) {
	s_dct4(x,y,1,a);
}

// strided one-dimensional DST-4
inline static void
s_dst4_1d (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a) {
	int n; // counter
	int N=a->N; // transform length
	minfft_cmpl *t=a->t; // temporary buffer
	minfft_cmpl *e=a->e; // exponent vector
	if (N==1) {
		// trivial case
		y[0]=sqrt2*x[0];
		return;
	}
	// reduce to complex DFT of length N/2
	// prepare sub-transform inputs
	minfft_real *tr=(minfft_real*)t,*er=(minfft_real*)e;
	minfft_real *ti=tr+1,*ei=er+1;
	for (n=0; n<N/2; ++n) {
		// t[n]=-*e++*(x[2*n]-I*x[N-1-2*n]);
		tr[2*n]=-er[2*n]*x[2*n]-ei[2*n]*x[N-1-2*n];
		ti[2*n]=er[2*n]*x[N-1-2*n]-ei[2*n]*x[2*n];
	}
	// do complex DFT in-place
	s_dft_1d(t,t,1,a->sub1);
	// recover results
	er+=N;
	ei+=N;
	for (n=0; n<N/2; ++n) {
		// y[sy*2*n]=2*cimag(*e++*t[n]);
		y[sy*2*n]=2*(er[4*n]*ti[2*n]+ei[4*n]*tr[2*n]);
		// y[sy*(2*n+1)]=2*cimag(*e++*conj(t[N/2-1-n]));
		y[sy*(2*n+1)]=2*(-er[4*n+2]*ti[N-2-2*n]+ei[4*n+2]*tr[N-2-2*n]);
	}
}

// strided DST-4 of arbitrary dimension
inline static void
s_dst4 (minfft_real *x, minfft_real *y, int sy, const minfft_aux *a) {
	mkrx(x,y,sy,a,s_dst4_1d);
}

// user interface
void
minfft_dst4 (minfft_real *x, minfft_real *y, const minfft_aux *a) {
	s_dst4(x,y,1,a);
}

// *** making of aux data ***

// make aux data for any transform of arbitrary dimension
// using its one-dimensional version
static minfft_aux*
make_aux (int d, int *Ns, int datasz, minfft_aux* (*aux_1d)(int N)) {
	minfft_aux *a;
	int p; // product of all transform lengths
	int i; // array index
	if (d==1)
		return (*aux_1d)(Ns[0]);
	else {
		p=1;
		for (i=0; i<d; ++i)
			p*=Ns[i];
		a=malloc(sizeof(minfft_aux));
		if (a==NULL)
			goto err;
		a->N=p;
		a->t=malloc(p*datasz);
		if (a->t==NULL)
			goto err;
		a->e=NULL;
		a->sub1=make_aux(d-1,Ns+1,datasz,aux_1d);
		if (a->sub1==NULL)
			goto err;
		a->sub2=(*aux_1d)(Ns[0]);
		if (a->sub2==NULL)
			goto err;
		return a;
	}
err:	// memory allocation error
	minfft_free_aux(a);
	return NULL;
}

// make aux data for one-dimensional forward or inverse complex DFT
minfft_aux*
minfft_mkaux_dft_1d (int N) {
	minfft_aux *a;
	int n;
	minfft_real *e;
	if (N<=0 || N&(N-1))
		// error if N is negative or not a power of two
		return NULL;
	a=malloc(sizeof(minfft_aux));
	if (a==NULL)
		goto err;
	a->N=N;
	if (N>=16) {
		a->t=malloc(N*sizeof(minfft_cmpl));
		if (a->t==NULL)
			goto err;
		a->e=malloc(N*sizeof(minfft_cmpl));
		if (a->e==NULL)
			goto err;
		e=(minfft_real*)a->e;
		while (N>=16) {
			for (n=0; n<N/4; ++n) {
				*e++=T(cos)(-2*pi*n/N);
				*e++=T(sin)(-2*pi*n/N);
				*e++=T(cos)(-2*pi*3*n/N);
				*e++=T(sin)(-2*pi*3*n/N);
			}
			N/=2;
		}
	} else {
		a->t=NULL;
		a->e=NULL;
	}
	a->sub1=a->sub2=NULL;
	return a;
err:	// memory allocation error
	minfft_free_aux(a);
	return NULL;
}

// make aux data for any-dimensional forward or inverse complex DFT
minfft_aux*
minfft_mkaux_dft (int d, int *Ns) {
	return make_aux(d,Ns,sizeof(minfft_cmpl),minfft_mkaux_dft_1d);
}

// convenience routines for two- and three-dimensional complex DFT
minfft_aux*
minfft_mkaux_dft_2d (int N1, int N2) {
	int Ns[2]={N1,N2};
	return minfft_mkaux_dft(2,Ns);
}
minfft_aux*
minfft_mkaux_dft_3d (int N1, int N2, int N3) {
	int Ns[3]={N1,N2,N3};
	return minfft_mkaux_dft(3,Ns);
}

// make aux data for one-dimensional forward or inverse real DFT
minfft_aux*
minfft_mkaux_realdft_1d (int N) {
	minfft_aux *a;
	int n;
	minfft_real *e;
	if (N<=0 || N&(N-1))
		// error if N is negative or not a power of two
		return NULL;
	a=malloc(sizeof(minfft_aux));
	if (a==NULL)
		goto err;
	a->N=N;
	if (N>=4) {
		a->t=malloc((N/2)*sizeof(minfft_cmpl));
		if (a->t==NULL)
			goto err;
		a->e=malloc((N/4)*sizeof(minfft_cmpl));
		if (a->e==NULL)
			goto err;
		e=(minfft_real*)a->e;
		for (n=0; n<N/4; ++n) {
			*e++=T(cos)(-2*pi*n/N);
			*e++=T(sin)(-2*pi*n/N);
		}
		a->sub1=minfft_mkaux_dft_1d(N/2);
	} else {
		a->t=NULL;
		a->e=NULL;
		a->sub1=NULL;
	}
	a->sub2=NULL;
	return a;
err:	// memory allocation error
	minfft_free_aux(a);
	return NULL;
}

// make aux data for any-dimensional real DFT
minfft_aux*
minfft_mkaux_realdft (int d, int *Ns) {
	minfft_aux *a;
	int p; // product of transform lengths
	int i; // array index
	if (d==1)
		return minfft_mkaux_realdft_1d(Ns[0]);
	else {
		p=1;
		for (i=0; i<d-1; ++i)
			p*=Ns[i];
		a=malloc(sizeof(minfft_aux));
		if (a==NULL)
			goto err;
		a->N=Ns[d-1]*p;
		a->t=malloc((Ns[d-1]/2+1)*p*sizeof(minfft_cmpl));
		if (a->t==NULL)
			goto err;
		a->e=NULL;
		a->sub1=minfft_mkaux_realdft_1d(Ns[d-1]);
		if (a->sub1==NULL)
			goto err;
		a->sub2=minfft_mkaux_dft(d-1,Ns);
		if (a->sub2==NULL)
			goto err;
		return a;
	}
err:	// memory allocation error
	minfft_free_aux(a);
	return NULL;
}

// convenience routines for two- and three-dimensional real DFT
minfft_aux*
minfft_mkaux_realdft_2d (int N1, int N2) {
	int Ns[2]={N1,N2};
	return minfft_mkaux_realdft(2,Ns);
}
minfft_aux*
minfft_mkaux_realdft_3d (int N1, int N2, int N3) {
	int Ns[3]={N1,N2,N3};
	return minfft_mkaux_realdft(3,Ns);
}

// make aux data for one-dimensional Type-2 or Type-3 transforms
minfft_aux*
minfft_mkaux_t2t3_1d (int N) {
	minfft_aux *a;
	int n;
	minfft_real *e;
	if (N<=0 || N&(N-1))
		// error if N is negative or not a power of two
		return NULL;
	a=malloc(sizeof(minfft_aux));
	if (a==NULL)
		goto err;
	a->N=N;
	if (N>=2) {
		a->t=malloc((N+2)*sizeof(minfft_real)); // for in-place real DFT
		if (a->t==NULL)
			goto err;
		a->e=malloc((N/2)*sizeof(minfft_cmpl));
		if (a->e==NULL)
			goto err;
		e=(minfft_real*)a->e;
		for (n=0; n<N/2; ++n) {
			*e++=T(cos)(-2*pi*n/4/N);
			*e++=T(sin)(-2*pi*n/4/N);
		}
	} else {
		a->t=NULL;
		a->e=NULL;
	}
	a->sub1=minfft_mkaux_realdft_1d(N);
	if (a->sub1==NULL)
		goto err;
	a->sub2=NULL;
	return a;
err:	// memory allocation error
	minfft_free_aux(a);
	return NULL;
}

// make aux data for any-dimensional Type-2 or Type-3 transforms
minfft_aux*
minfft_mkaux_t2t3 (int d, int *Ns) {
	return make_aux(d,Ns,sizeof(minfft_real),minfft_mkaux_t2t3_1d);
}

// convenience routines for two- and three-dimensional Type 2 or 3 transforms
minfft_aux*
minfft_mkaux_t2t3_2d (int N1, int N2) {
	int Ns[2]={N1,N2};
	return minfft_mkaux_t2t3(2,Ns);
}
minfft_aux*
minfft_mkaux_t2t3_3d (int N1, int N2, int N3) {
	int Ns[3]={N1,N2,N3};
	return minfft_mkaux_t2t3(3,Ns);
}

// make aux data for an one-dimensional Type-4 transform
minfft_aux*
minfft_mkaux_t4_1d (int N) {
	minfft_aux *a;
	int n;
	minfft_real *e;
	if (N<=0 || N&(N-1))
		// error if N is negative or not a power of two
		return NULL;
	a=malloc(sizeof(minfft_aux));
	if (a==NULL)
		goto err;
	a->N=N;
	if (N>=2) {
		a->t=malloc((N/2)*sizeof(minfft_cmpl));
		if (a->t==NULL)
			goto err;
		a->e=malloc((N/2+N)*sizeof(minfft_cmpl));
		if (a->e==NULL)
			goto err;
		e=(minfft_real*)a->e;
		for (n=0; n<N/2; ++n) {
			*e++=T(cos)(-2*pi*n/2/N);
			*e++=T(sin)(-2*pi*n/2/N);
		}
		for (n=0; n<N; ++n) {
			*e++=T(cos)(-2*pi*(2*n+1)/8/N);
			*e++=T(sin)(-2*pi*(2*n+1)/8/N);
		}
		a->sub1=minfft_mkaux_dft_1d(N/2);
		if (a->sub1==NULL)
			goto err;
	} else {
		a->t=NULL;
		a->e=NULL;
		a->sub1=NULL;
	}
	a->sub2=NULL;
	return a;
err:	// memory allocation error
	minfft_free_aux(a);
	return NULL;
}

// make aux data for an any-dimensional Type-4 transform
minfft_aux*
minfft_mkaux_t4 (int d, int *Ns) {
	return make_aux(d,Ns,sizeof(minfft_real),minfft_mkaux_t4_1d);
}

// convenience routines for two- and three-dimensional Type 4 transforms
minfft_aux*
minfft_mkaux_t4_2d (int N1, int N2) {
	int Ns[2]={N1,N2};
	return minfft_mkaux_t4(2,Ns);
}
minfft_aux*
minfft_mkaux_t4_3d (int N1, int N2, int N3) {
	int Ns[3]={N1,N2,N3};
	return minfft_mkaux_t4(3,Ns);
}

// free aux chain
void
minfft_free_aux (minfft_aux *a) {
	if (a==NULL)
		return;
	free(a->t);
	free(a->e);
	minfft_free_aux(a->sub1);
	minfft_free_aux(a->sub2);
	free(a);
}

// A minimalistic FFT library
// Copyright (c) 2021 Alexander Mukhin
// MIT License

#ifndef MINFFT_H
#define MINFFT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef complex
// use C99 complex
#if MINFFT_SINGLE
typedef float minfft_real;
typedef float complex minfft_cmpl;
#elif MINFFT_EXTENDED
typedef long double minfft_real;
typedef long double complex minfft_cmpl;
#else
typedef double minfft_real;
typedef double complex minfft_cmpl;
#endif
#else
// not using C99 complex
#if MINFFT_SINGLE
typedef float minfft_real;
#elif MINFFT_EXTENDED
typedef long double minfft_real;
#else
typedef double minfft_real;
#endif
typedef minfft_real minfft_cmpl[2];
#endif

typedef struct minfft_aux minfft_aux;

void minfft_dft (minfft_cmpl*, minfft_cmpl*, const minfft_aux*);
void minfft_invdft (minfft_cmpl*, minfft_cmpl*, const minfft_aux*);
void minfft_realdft (minfft_real*, minfft_cmpl*, const minfft_aux*);
void minfft_invrealdft (minfft_cmpl*, minfft_real*, const minfft_aux*);
void minfft_dct2 (minfft_real*, minfft_real*, const minfft_aux*);
void minfft_dst2 (minfft_real*, minfft_real*, const minfft_aux*);
void minfft_dct3 (minfft_real*, minfft_real*, const minfft_aux*);
void minfft_dst3 (minfft_real*, minfft_real*, const minfft_aux*);
void minfft_dct4 (minfft_real*, minfft_real*, const minfft_aux*);
void minfft_dst4 (minfft_real*, minfft_real*, const minfft_aux*);

minfft_aux* minfft_mkaux_dft_1d (int);
minfft_aux* minfft_mkaux_dft_2d (int, int);
minfft_aux* minfft_mkaux_dft_3d (int, int, int);
minfft_aux* minfft_mkaux_dft (int, int*);
minfft_aux* minfft_mkaux_realdft_1d (int);
minfft_aux* minfft_mkaux_realdft_2d (int, int);
minfft_aux* minfft_mkaux_realdft_3d (int, int, int);
minfft_aux* minfft_mkaux_realdft (int, int*);
minfft_aux* minfft_mkaux_t2t3_1d (int);
minfft_aux* minfft_mkaux_t2t3_2d (int, int);
minfft_aux* minfft_mkaux_t2t3_3d (int, int, int);
minfft_aux* minfft_mkaux_t2t3 (int, int*);
minfft_aux* minfft_mkaux_t4_1d (int);
minfft_aux* minfft_mkaux_t4_2d (int, int);
minfft_aux* minfft_mkaux_t4_3d (int, int, int);
minfft_aux* minfft_mkaux_t4 (int, int*);

void minfft_free_aux (minfft_aux*);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MINFFT_H

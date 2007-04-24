#ifndef __FIXED_FFT_H__
#define __FIXED_FFT_H__

#define N_WAVE          1024    /* dimension of Sinewave[] */
#define LOG2_N_WAVE     10      /* log2(N_WAVE) */
#define N_LOUD          100	/* dimension of Loudampl[] */

#ifndef fixed
#define fixed int16_t
#endif
int16_t fix_fft(fixed fr[], fixed fi[], int16_t m, int16_t inverse);


#endif



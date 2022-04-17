#include <msp430.h>
#include <libdsp/DSPLib.h>
#include <libio/console.h>
#include <stdio.h>
#include <libfakecatnap/culpeo.h>
#include <libfakecatnap/hw.h>
#include <libfakecatnap/catnap.h>

void fft(float *buf, float *result, unsigned size)
{
	// FFT lib
	msp_fft_q15_params fftParams;
	fftParams.length = size;
	fftParams.bitReverse = true;
	// Note: to use other table, uncomment it from libdsp
	//fftParams.twiddleTable = msp_cmplx_twiddle_table_256_q15;
	// Note: Be careful!!
	// TI FFT library cannot handle overflow nicely.
	// To prevent overflow, the input needs to be scaled and/or
	// adjusted (subtracting DC bias if any).
	switch (size) {
			case 64:
			fftParams.twiddleTable = msp_cmplx_twiddle_table_64_q15;
			break;
		case 32:
			fftParams.twiddleTable = msp_cmplx_twiddle_table_32_q15;
			break;
		case 16:
			fftParams.twiddleTable = msp_cmplx_twiddle_table_16_q15;
			break;
}
  LCFN_INTERRUPTS_DISABLE;
	msp_fft_fixed_q15(&fftParams, buf);
  LCFN_INTERRUPTS_ENABLE;

	// Calculate magnitude
	for (unsigned i = 0; i < size / 2; i += 1) {
		_q15 mag = __saturated_add_q15(
				__q15mpy(CMPLX_REAL(buf + i*2), CMPLX_REAL(buf + i*2))
				, __q15mpy(CMPLX_IMAG(buf + i*2), CMPLX_IMAG(buf + i*2)));
		result[i] = mag;
	}
}


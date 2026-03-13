#pragma once

//For some reason these template functions with the static variables are slower than the class based fft

#include <stdint.h>
#include <cmath>

#define MATH_PI 3.14159265358979323846264338327950288419716939937510f

//Returns the floor of log base 2 of x
uint32_t log2floor(uint32_t x){
	uint32_t res = 0;
	while (!( (x>>res)&1) ){
		res++;
	}
	return res;
}

//Returns the reverse bit of a number for a certain number of bits
uint32_t bitRev(uint32_t x, uint32_t bits){
	uint32_t res = 0;
	__asm(
		"RBIT %[result], %[input_x]"
		: [result] "=r" (res)
		: [input_x] "r" (x)
	);
	return res >> (32-bits);
}

struct complex {
	float real;
	float imag;
};

//Swap function for complex numbers
void swap(complex& x, complex& y){
	float tempReal = x.real;
	float tempImag = x.imag;
	x.real = y.real;
	x.imag = y.imag;
	y.real = tempReal;
	y.imag = tempImag;
}

//Reverse permutes an array of length N. M is logbase2(N)
void reversePermute(complex x[], unsigned N, unsigned M){
    uint32_t revN;
    for (uint32_t n = 0; n < N/2; n++){
        revN = bitRev(n, M);
        if (n < revN){
            swap(x[n], x[revN]);
            swap(x[(~n) & (N-1)], x[(~revN) & (N-1)]);
        }
        n++;
        swap(x[n], x[bitRev(n, M)]);
    }
}

template <uint32_t N>
inline void fft(complex x[]){
    const static unsigned M = log2floor(N);

    static bool twiddleCalculated = false;
    static complex WN[N/2];
	if (!twiddleCalculated){
    	float baseParameter = (2 * M_PI) / N;
    	for (uint32_t r = 0; r < N/2; r++){
    		float parameter = baseParameter * r;
    		uint32_t index = bitRev(r,M-1);
    		WN[index].real =  cosf(parameter);
    		WN[index].imag = -sinf(parameter);
    	}
    	twiddleCalculated = true;
	}

    uint32_t _N = N;		//Current size of each DFT at the current stage
    uint32_t numberOfDFT = 1;
    uint32_t numBF = 1 << (M-1);
    for (uint32_t m = 0; m < M; m++){
        uint32_t t = 0;
        for (uint32_t l = 0; l < numberOfDFT; l++){
            for (uint32_t BF = 0; BF < numBF; BF++){
                uint32_t p = BF + t;
                uint32_t q = p + numBF;
                complex prevQRes;
				prevQRes.real = x[q].real * WN[l].real;
				prevQRes.imag = x[q].real * WN[l].imag;
				prevQRes.real -= x[q].imag * WN[l].imag;
				prevQRes.imag += + x[q].imag * WN[l].real;
                x[q].real = x[p].real - prevQRes.real;
                x[q].imag = x[p].imag - prevQRes.imag;
                x[p].real = x[p].real + prevQRes.real;
                x[p].imag = x[p].imag + prevQRes.imag;
            }
            t += _N;
        }
        numberOfDFT = numberOfDFT * 2;
        numBF = numBF / 2;
        _N = _N / 2;
    }
}

//Based off of the pack_2real_fft python example from https://kovleventer.com/blog/fft_real/
template <uint32_t N>
void rfft(complex x[], complex y[]){
    //Pack y into the imaginary parts of x
    for (unsigned int i = 0; i < N; i++){
        x[i].imag = y[i].real;
    }
    fft<N>(x);
    for (unsigned int n = 0; n < N; n++){
        y[n].real = x[n].imag + x[(N-n) % N].imag;
        y[n].imag = x[n].real - x[(N-n) % N].real;
    }
    for (unsigned int n = 0; n < N/2 + 1; n++){
        complex currentxn = x[n];
        complex mirrorxn = x[(N-n) % N];

        x[n].real = currentxn.real + mirrorxn.real;
        x[n].imag = currentxn.imag - mirrorxn.imag;

        x[(N-n) % N].real = mirrorxn.real + currentxn.real;
        x[(N-n) % N].imag = mirrorxn.imag - currentxn.imag;
    }
    for (unsigned int n = 0; n < N; n++){
        y[n].real *= 0.5f;
        y[n].imag *= 0.5f;
        x[n].real *= 0.5f;
        x[n].imag *= 0.5f;
    }
}

//Generates a hanning window of length M within the array w
void hanning(int M, float w[]){
    for (int m = 0; m < M; m++){
        w[m] = 0.5f - 0.5f*cosf((2*MATH_PI*m)/(M-1));
    }
}

//Computes N magnitudes from a complex array x to a float array res.
void magnitude(complex x[], float res[], uint32_t N){
    for (uint32_t n = 0; n < N; n++){
        res[n] = sqrtf(powf(x[n].real, 2.0f) + powf(x[n].imag, 2.0f));
    }
}

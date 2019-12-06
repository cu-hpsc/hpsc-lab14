#include <complex.h>
#include <stdlib.h> // for rand48
#include <math.h> // for exp
#include <omp.h>

#include <fftw3.h>
#include "parse.h"
#include "rdtsc.h"


int main(int argc, char **argv)
{
  struct args args;
  long N;
  fftw_complex *base;
  fftw_complex *freq;
  fftw_plan plan;
  double rand;
  ticks_t tick;

  // argument parsing via parse.h
  parse_args(&args, argc, argv);
  
  // allocate
  N = args.array_len;
  base = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
  freq = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

  // initialize
  for (long i = 0; i < N; ++i) {
    rand = drand48();
    base[i] = rand*cos(i) + (1-rand)*(1-2*drand48()) + I*(rand*sin(i) + (1-rand)*(1-2*drand48()));
  }
  
  // fftw
  fftw_init_threads();
  fftw_plan_with_nthreads(args.threads ? args.threads : omp_get_max_threads());
  tick = rdtsc();
  for (int r = 0; r < args.reps; ++r) {
    plan = fftw_plan_dft_1d(N, base, freq, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
  }
  fprintf(stderr,"%ld ticks\n",(long)(rdtsc()-tick));

  if (args.verbosity) {
    // print
    for (long i = 0; i < N; ++i) {
      printf("%f,%f\n",creal(freq[i]),cimag(freq[i]));
    }
  }
  
  // cleanup
  fftw_cleanup_threads();
  fftw_free(base);
  fftw_free(freq);
}

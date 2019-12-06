# HPSC Lab 14
2019-12-06

Click to [make your own repo](https://classroom.github.com/a/ybZp1i5C).

The goals for this lab include:
* Use FFTW
* Investigate FFTW Wisdom as an autotuning approach.

A parallel lab possibility is to use Fourier Transforms for solving PDEs.  If that is more your interest, see the Spectral.ipynb notebook.

-----

## FFTW Installation

```
wget "http://www.fftw.org/fftw-3.3.8.tar.gz"
tar -xzf fftw-3.3.8.tar.gz
cd fftw-3.3.8
./configure --prefix $PWD/build_omp --enable-openmp --disable-fortran --enable-avx --enable-avx2
make
make install
```

The configure options allow FFTW to run with omp threads as well as AVX registers, and cause the install to use a local `build_omp` directory for which you have permission.

-----

## Simple example

Compile flags:
```
gcc -o dft_dim1 dft_dim1.c -Ifftw-3.3.8/build_omp/include/ -Lfftw-3.3.8/build_omp/lib/ -lfftw3_omp -lfftw3 -lm -fopenmp -O3
```

Run `./dft_dim1` with `--help` to see options.  The reported number of ticks is encapsulating just the critical FFTW region of code.  Measure the timing when varying the array length, starting with powers of 2 and then trying lengths with non-2 factors.  As always, repeat over a number of trials such that the timing is somewhat stable on your resource.

*When testing on CSEL, the default `-t 2` seemed to cause terrible overhead and instabilities.  I suggest sticking to `-t 1` on that resource.*


-----

## Wisdom

One of the neat aspects of FFTW is that it offers a kind of "auto-tuning", where the particular methods can be optimized for an array length on the particular compute resource.  *Wisdom* allows for more than just optimizing for a specific array length because the underlying methods can be used when transforming various array lengths.

Try changing the plan flag from `FFTW_ESTIMATE` to `FFTW_MEASURE`.  With this flag, FFTW will take some time to compute a few FFTs and measure their execution time.  `FFTW_PATIENT` and `FFTW_EXHAUSTIVE` are increasingly extreme versions of this approach.

When one of these alternatives to `FFTW_ESTIMATE` are used, FFTW accumulates timing information in its own internal data structures that is automatically used when designing future plans in the same run (assuming they aren't flagged to be more patient).  Additionally, this *Wisdom* can be saved to and loaded from external files (available prototypes for this below), although there is some danger that what is optimal may have changed.
```
int fftw_export_wisdom_to_filename(const char *filename);
int fftw_import_wisdom_from_filename(const char *filename);
```

Measure the timing with and without Wisdom, now while varying the number of repetitions.  When is the cost of acquiring Wisdom outweighed by the benefit for various array lengths?  Check with others around you to tackle different ranges of array lengths to compare.
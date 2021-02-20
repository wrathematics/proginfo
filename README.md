# proginfo

* **Version:** 0.1-0
* **License:** [BSL-1.0](http://opensource.org/licenses/BSL-1.0)
* **Project home**: https://github.com/wrathematics/proginfo
* **Bug reports**: https://github.com/wrathematics/proginfo/issues


proginfo is a small utility for getting some basic information about a program, sort of like the unix command `time`. This is not a replacement for a real profiler.

Under development. More later.


## Compiling

Edit the makefile and type `make`. Real make system later.



## Examples

Here's a basic example 

```
$ ./proginfo Rscript -e "1+1"
[1] 2

## Program Info (from 28 polls)
* CPU
  - Wall time: 0.285
  - Utilization: 1.284%
  - RAM: 15.451/15.482/15.506 (62.797) / 62.814 GiB
* GPU
  - RAM:
    + (Device 0) 1.084/1.084/1.084 (0.000) / 7.926 GiB
```

And here's a code that allocates some data on the GPU:

```
$ ./proginfo Rscript -e 'suppressMessages(library(fmlr)); c = card(); x = gpumat(c, 25000, 25000, type="float"); x$fill_zero(); x$info()'

# gpumat 25000x25000 type=f 

## Program Info (from 119 polls)
* CPU
  - Wall time: 1.235
  - Utilization: 0.919%
  - RAM: 15.471/15.632/15.855 (62.669) / 62.814 GiB
* GPU
  - RAM:
    + (Device 0) 1.084/1.180/3.636 (0.329) / 7.926 GiB
```

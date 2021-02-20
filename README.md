# proginfo

* **Version:** 0.1-0
* **License:** [BSL-1.0](http://opensource.org/licenses/BSL-1.0)
* **Project home**: https://github.com/wrathematics/proginfo
* **Bug reports**: https://github.com/wrathematics/proginfo/issues


proginfo is a small utility for getting some basic information about a program, sort of like a fancier version of the unix command `time`. This is not a replacement for a real profiler.

The program reports wallclock time, and system-level utilization and RAM usage for both CPU and any (NVIDIA) GPUs.


## Notes

1. Sampled values are reported as follows
```
MIN/MEAN/MAX (SD) / TOTAL
```

2. GPU utilization rates are the "percent of time over the past second in which any work has been executing on the GPU." ([reference](http://developer.download.nvidia.com/compute/DevZone/NVML/doxygen/structnvml_utilization__t.html)).

3. The consumed memory on GPUs is "equal to the sum of memory allocated by all active channels on the device." ([reference](http://developer.download.nvidia.com/compute/DevZone/NVML/doxygen/group__group4.html#g2dfeb1db82aa1de91aa6edf941c85ca8))

4. The consumed memory for CPU is reported as the complement (from total) of the sum of buffered, cached, and free RAM on Linux.

5. All memory numbers are in GiB (divide bytes by 1024^3).

6. CPU utilization is reported by `clock()`, which is wrong. Will probably fix eventually.



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
  - Utilization
      + (Device 0) 1/1.00/1 (0.00) / 100%
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
  - Utilization
      + (Device 0) 0/6.69/44 (12.37) / 100%
  - RAM:
      + (Device 0) 1.059/1.159/3.611 (0.342) / 7.926 GiB
```

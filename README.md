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

## Program Info
* Wall time: 0.264
* CPU utilization: 1.014%
* Max CPU RAM usage: 15.132 / 62.814 GiB
* Max GPU RAM usage: 
  - Device 0: 1.145 / 7.926 GiB
```

And here's a code that allocates some data on the GPU:

```
$ ./proginfo Rscript -e 'suppressMessages(library(fmlr)); c = card(); x = gpumat(c, 25000, 25000, type="float"); x$fill_zero(); x$info()'
# gpumat 25000x25000 type=f 

## Program Info
* Wall time: 1.092
* CPU utilization: 0.649%
* Max CPU RAM usage: 15.459 / 62.814 GiB
* Max GPU RAM usage: 
  - Device 0: 3.697 / 7.926 GiB
```

#ifndef PROGINFO_GPU_GPUINFO_H
#define PROGINFO_GPU_GPUINFO_H
#pragma once


#include <cstdio>

#include "../stats.hh"
#include "nvml.hh"


namespace proginfo
{
  class gpuinfo
  {
    public:
      gpuinfo()
      {
        nvml::init();
        
        _ngpus = nvml::device::get_count();
        
        _totalram = new double[_ngpus];
        _ramstats = new stats<double>[_ngpus];
        
        for (int gpu=0; gpu<_ngpus; gpu++)
        {
          _dev = nvml::device::get_handle_by_index(gpu);
          double tmp;
          nvml::device::get_memory_info(_dev, &tmp, _totalram+gpu);
          
          _ramstats[gpu].add_sample(tmp);
        }
      };
      
      
      
      ~gpuinfo()
      {
        delete _totalram;
        delete _ramstats;
        
        nvml::shutdown();
      };
      
      
      
      void poll()
      {
        poll_ram();
      }
      
      
      
      void print()
      {
        print_md();
      };
    
    
    
    protected:
      int _ngpus;
      double *_totalram;
      stats<double> *_ramstats;
    
    
    
    private:
      nvmlDevice_t _dev;
      
      
      
      void print_md()
      {
        printf("* GPU RAM usage [MIN/MEAN/MAX (SDEV) / TOTAL]: \n");
        for (int gpu=0; gpu<_ngpus; gpu++)
          printf("  - Device %d: %.3f/%.3f/%.3f (%.3f) / %.3f GiB\n",
            gpu,
            b2gb(_ramstats[gpu].min()),
            b2gb(_ramstats[gpu].mean()),
            b2gb(_ramstats[gpu].max()),
            b2gb(_ramstats[gpu].sd()),
            b2gb(_totalram[gpu]));
      }
      
      
      
      double b2gb(double x, bool si_unit=false)
      {
        if (si_unit)
          return x / 1000.0 / 1000.0 / 1000.0;
        else
          return x / 1024.0 / 1024.0 / 1024.0;
      };
      
      
      
      void poll_ram()
      {
        double tmp;
        double tmp2;
        
        for (int gpu=0; gpu<_ngpus; gpu++)
        {
          _dev = nvml::device::get_handle_by_index(gpu);
          nvml::device::get_memory_info(_dev, &tmp, &tmp2);
          _ramstats[gpu].add_sample(tmp);
        }
      };
  };
}


#endif

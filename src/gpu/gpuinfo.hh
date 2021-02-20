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
        
        _utilstats = new stats<int>[_ngpus];
        
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
        
        delete _utilstats;
        
        nvml::shutdown();
      };
      
      
      
      void poll()
      {
        poll_ram();
        poll_util();
      }
      
      
      
      void print()
      {
        print_md();
      };
    
    
    
    protected:
      int _ngpus;
      double *_totalram;
      stats<double> *_ramstats;
      stats<int> *_utilstats;
    
    
    
    private:
      nvmlDevice_t _dev;
      
      
      
      void print_md()
      {
        printf("  - Utilization\n");
        for (int gpu=0; gpu<_ngpus; gpu++)
          printf("      + (Device %d) %d/%.2f/%d (%.2f) / 100%%\n",
            gpu,
            _utilstats[gpu].min(),
            _utilstats[gpu].mean(),
            _utilstats[gpu].max(),
            _utilstats[gpu].sd());
        
        printf("  - RAM:\n");
        for (int gpu=0; gpu<_ngpus; gpu++)
          printf("      + (Device %d) %.3f/%.3f/%.3f (%.3f) / %.3f GiB\n",
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
      
      
      
      void poll_util()
      {
        for (int gpu=0; gpu<_ngpus; gpu++)
        {
          _dev = nvml::device::get_handle_by_index(gpu);
          int util = nvml::device::get_utilization(_dev);
          _utilstats[gpu].add_sample(util);
        }
      };
  };
}


#endif

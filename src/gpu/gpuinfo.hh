#ifndef PROGINFO_GPU_GPUINFO_H
#define PROGINFO_GPU_GPUINFO_H
#pragma once


#include <cstdio>

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
        _usedram = new double[_ngpus];
        
        for (int gpu=0; gpu<_ngpus; gpu++)
        {
          _dev = nvml::device::get_handle_by_index(gpu);
          nvml::device::get_memory_info(_dev, _usedram+gpu, _totalram+gpu);
        }
      };
      
      ~gpuinfo()
      {
        delete _totalram;
        delete _usedram;
        
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
      nvmlDevice_t _dev;
      double *_totalram;
      double *_usedram;
    
    
    
    private:
      void print_md()
      {
        printf("* Max GPU RAM usage: \n");
        for (int gpu=0; gpu<_ngpus; gpu++)
          printf("  - Device %d: %.3f / %.3f GiB\n", gpu, usedram(gpu), totalram(gpu));
      }
      
      
      
      double b2gb(double x, bool si_unit=false)
      {
        if (si_unit)
          return x / 1000.0 / 1000.0 / 1000.0;
        else
          return x / 1024.0 / 1024.0 / 1024.0;
      };
      
      
      
      double totalram(int gpu, bool si_unit=false)
      {
        return b2gb(_totalram[gpu], si_unit);
      };
      
      double usedram(int gpu, bool si_unit=false)
      {
        return b2gb(_usedram[gpu], si_unit);
      };
      
      double freeram(int gpu, bool si_unit=false)
      {
        return b2gb(_totalram[gpu] - _usedram[gpu], si_unit);
      };
      
      
      
      void poll_ram()
      {
        double tmp1;
        double tmp2;
        
        for (int gpu=0; gpu<_ngpus; gpu++)
        {
          _dev = nvml::device::get_handle_by_index(gpu);
          nvml::device::get_memory_info(_dev, &tmp1, &tmp2);
          if (tmp1 > _usedram[gpu])
            _usedram[gpu] = tmp1;
        }
      };
  };
}


#endif

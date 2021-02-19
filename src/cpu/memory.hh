#ifndef PROGINFO_CPU_MEMORY_H
#define PROGINFO_CPU_MEMORY_H
#pragma once


#include <cstdint>
#include <cstdio>

#include "platform.h"


namespace proginfo
{
  class meminfo
  {
    public:
      meminfo()
      {
        totalram(&_totalram);
        _freeram = totalfree();
      };
      
      
      
      void poll()
      {
        memsize_t tmp = totalfree();
        if (tmp < _freeram)
          _freeram = tmp;
      };
      
      
      
      void print()
      {
        print_md();
      };
    
    
    
    protected:
      memsize_t _freeram;
      memsize_t _totalram;
    
    
    
    private:
      void print_md()
      {
        printf("* Max CPU RAM usage: %.3f / %.3f GiB\n", usedram(), totalram());
      };
      
      
      
      float totalram()
      {
        return  memsize_2_floatgb(_totalram);
      };
      
      float usedram()
      {
        return memsize_2_floatgb(_totalram - _freeram);
      };
      
      float freeram()
      {
        return memsize_2_floatgb(_freeram);
      };
      
      
      
      memsize_t totalfree()
      {
        memsize_t buffer, cached, free;
        bufferram(&buffer);
        cachedram(&cached);
        freeram(&free);
        
        return free + buffer + cached;
      };
      
      
      
      float memsize_2_floatgb(memsize_t x, bool si_unit=false)
      {
        if (si_unit)
          return (float) x / 1000 / 1000 / 1000;
        else
          return (float) x / 1024 / 1024 / 1024;
      };
      
      
      
      int totalram(memsize_t *totalram)
      {
        int ret = MEMINFO_OK;
        *totalram = 0L;
        
        
      #if OS_LINUX
        struct sysinfo info;
        int test = sysinfo(&info);
        chkret(test, FAILURE);
        
        *totalram = (memsize_t) info.totalram * info.mem_unit;
      #elif OS_MAC
        int test = sysctl_val("hw.memsize", totalram);
        chkret(test, FAILURE);
      #elif OS_WINDOWS
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        
        int test = GlobalMemoryStatusEx(&status);
        winchkret(test, FAILURE);
        
        *totalram = (memsize_t) status.ullTotalPhys;
      #elif OS_FREEBSD
        int test = sysctl_val("hw.physmem", totalram);
        chkret(test, FAILURE);
      #elif OS_NIX
        memsize_t npages, pagesize;
        
        npages = sysconf(_SC_PHYS_PAGES);
        if (npages == (memsize_t)FAILURE)
          return FAILURE;
        
        pagesize = sysconf(_SC_PAGESIZE);
        if (pagesize == (memsize_t)FAILURE)
          return FAILURE;
        
        *totalram = (memsize_t) npages * pagesize;
      #else
        ret = PLATFORM_ERROR;
      #endif
        
        return ret;
      };
      
      
      
      int freeram(memsize_t *freeram)
      {
        int ret = MEMINFO_OK;
        *freeram = 0L;
        
        
      #if OS_LINUX
        struct sysinfo info;
        int test = sysinfo(&info);
        chkret(test, FAILURE);
        
        *freeram = (memsize_t) info.freeram * info.mem_unit;
      #elif OS_MAC
        vm_size_t page_size;
        mach_port_t mach_port;
        mach_msg_type_number_t count;
        vm_statistics_data_t vm_stats;
        
        mach_port = mach_host_self();
        count = sizeof(vm_stats) / sizeof(natural_t);
        
        int test = host_page_size(mach_port, &page_size);
        if (test != KERN_SUCCESS)
          return FAILURE;
        
        test = host_statistics(mach_port, HOST_VM_INFO, (host_info_t)&vm_stats, &count);
        if (test != KERN_SUCCESS)
          return FAILURE;
        
        *freeram = (memsize_t) vm_stats.free_count * (memsize_t) page_size;
      #elif OS_WINDOWS
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        
        int test = GlobalMemoryStatusEx(&status);
        winchkret(test, FAILURE);
        
        *freeram = (memsize_t) status.ullAvailPhys;
      #elif OS_FREEBSD
        int page = sysconf(_SC_PAGESIZE);
        if (page == -1)
          return FAILURE;
        
        int test = sysctl_val("vm.stats.vm.v_free_count", freeram);
        chkret(test, FAILURE);
        
        *freeram *= (memsize_t) page;
      #elif OS_NIX
        memsize_t pagesize, freepages;
        
        pagesize = (memsize_t) sysconf(_SC_PAGESIZE);
        if (pagesize == (memsize_t)FAILURE)
          return FAILURE;
        
        freepages = (memsize_t) sysconf(_SC_AVPHYS_PAGES);
        if (freepages == (memsize_t)FAILURE)
          return FAILURE;
        
        *freeram = pagesize * freepages;
      #else
        ret = PLATFORM_ERROR;
      #endif
        
        return ret;
      };
      
      
      
      int bufferram(memsize_t *bufferram)
      {
        int ret = MEMINFO_OK;
        *bufferram = 0L;
        
      #if OS_LINUX
        struct sysinfo info;
        
        int test = sysinfo(&info);
        chkret(test, FAILURE);
        
        *bufferram = info.bufferram * info.mem_unit;
      #elif OS_FREEBSD
        memsize_t v = 0L;
        
        int test = sysctl_val("vfs.bufspace", &v);
        chkret(test, FAILURE);
        
        *bufferram = v;
      #else
        ret = PLATFORM_ERROR;
      #endif
        
        return ret;
      };
      
      
      
      int cachedram(memsize_t *cachedram)
      {
        int ret = MEMINFO_OK;
        *cachedram = 0L;
        
        
      #if OS_LINUX
        int test = read_proc_file("/proc/meminfo", cachedram, (char*)"Cached:", 7);
        chkret(test, FAILURE);
        
        *cachedram *= 1024L;
      #elif OS_FREEBSD
        int page;
        memsize_t v = 0;
        
        page = sysconf(_SC_PAGESIZE);
        if (page == -1)
          return FAILURE;
        
        int test = sysctl_val("vm.stats.vm.v_cache_count", &v);
        chkret(test, FAILURE);
        
        *cachedram = (memsize_t) v*page;
      #else
        ret = PLATFORM_ERROR;
      #endif
        
        return ret;
      };
  };
}


#endif

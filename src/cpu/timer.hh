#ifndef PROGINFO_CPU_TIMER_H
#define PROGINFO_CPU_TIMER_H
#pragma once


#include <chrono>
#include <climits>
#include <ctime>


namespace proginfo
{
  class timer
  {
    public:
      timer()
      {
        reset();
      };
      
      
      
      void on()
      {
        _wstart = query_clock();
        _pstart = clock();
      };
      
      void off()
      {
        std::chrono::duration<double> elapsed = query_clock() - _wstart;
        _wtime += elapsed.count();
        
        _ptime += ((float) (clock() - _pstart)) / ((float)CLOCKS_PER_SEC);
      };
      
      void reset()
      {
        _wtime = 0;
        _ptime = 0;
      };
      
      
      
      void print()
      {
        print_md();
      };
      
      
      
      void sleep(int ms)
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
      };
      
      
      
      float wtime() const {return _wtime;};
      float ptime() const {return _ptime;};
      
      float cpu_util() const
      {
        return _ptime / _wtime * 100;
      };
    
    
    
    protected:
      std::chrono::high_resolution_clock::time_point _wstart;
      clock_t _pstart;
      float _wtime;
      float _ptime;
    
    
    
    private:
      void print_md()
      {
        printf("* Wall time: %.3f\n", _wtime);
        printf("* CPU utilization: %.3f%%\n", cpu_util());
      };
      
      
      
      std::chrono::high_resolution_clock::time_point query_clock() const
      {
        return std::chrono::high_resolution_clock::now();
      };
  };
}


#endif

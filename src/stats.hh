#ifndef PROGINFO_STATS_H
#define PROGINFO_STATS_H
#pragma once


#include <cmath>


// Welford's method for one-pass mean/variance
namespace proginfo
{
  template <typename T>
  class stats
  {
    public:
      stats()
      {
        _mean = 0.0;
        _s = 0;
        _n = 0;
      };
      
      
      
      void add_sample(T x)
      {
        if (_n == 0)
        {
          _max = x;
          _min = x;
        }
        else
        {
          if (_max < x)
            _max = x;
          if (_min > x)
            _min = x;
        }
        
        _n++;
        double r = ((double) x) - _mean;
        _mean += r / ((double) _n);
        _s += r * (((double) x) - _mean);
      };
      
      
      
      T max() const { return _max; };
      T min() const { return _min; };
      double mean() const { return _mean; };
      double var() const { return _s / ((double) _n - 1); };
      double sd() const { return sqrt(var()); };
      
      
    protected:
      T _max;
      T _min;
      double _mean;
      double _s;
      unsigned int _n;
  };
}


#endif

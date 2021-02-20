#include <cstdint>

#include "catch.hpp"
#include "../src/stats.hh"


static inline double round4(double x)
{
  return round(x * 10000) / 10000;
}



TEMPLATE_TEST_CASE("trivial", "[stats]", int, uint64_t, float, double)
{
  proginfo::stats<TestType> s;
  s.add_sample((TestType) 0);
  
  REQUIRE( round4(s.min()) == (TestType) 0 );
  REQUIRE( round4(s.max()) == (TestType) 0 );
  REQUIRE( round4(s.mean()) == 0 );
}



TEMPLATE_TEST_CASE("flat", "[stats]", int, uint64_t, float, double)
{
  proginfo::stats<TestType> s;
  
  const int len = 4;
  for (int i=1; i<=len; i++)
    s.add_sample((TestType) 1);
  
  REQUIRE( s.min() == (TestType) 1 );
  REQUIRE( s.max() == (TestType) 1 );
  REQUIRE( s.mean() == 1 );
  REQUIRE( s.var() == 0 );
  REQUIRE( s.sd() == 0 );
}



TEMPLATE_TEST_CASE("linspace", "[stats]", int, uint64_t, float, double)
{
  proginfo::stats<TestType> s;
  
  const int len = 10;
  for (int i=1; i<=len; i++)
    s.add_sample((TestType) i);
  
  REQUIRE( s.min() == (TestType) 1 );
  REQUIRE( s.max() == (TestType) 10 );
  REQUIRE( s.mean() == 5.5 );
  REQUIRE( round4(s.var()) == 9.1667 );
  REQUIRE( round4(s.sd()) == 3.0277 );
}

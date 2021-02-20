#include "catch.hpp"
#include "../src/cpu/timer.hh"


class vals : public proginfo::timer
{
  public:
    float wtime() { return this->_wtime; };
    float ptime() { return this->_ptime; };
};



TEST_CASE("timer", "[timer]")
{
  vals t;
  
  t.on();
  clock();
  t.off();
  
  REQUIRE( t.wtime() > 0 );
  REQUIRE( t.ptime() > 0 );
}

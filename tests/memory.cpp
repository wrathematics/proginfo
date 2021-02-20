#include <cstdint>

#include "catch.hpp"
#include "../src/cpu/memory.hh"


class vals : public proginfo::meminfo
{
  public:
    double totalram() { return this->_totalram / 1024 / 1024; };
    proginfo::stats<memsize_t> ramstats() { return this->_ramstats; };
};



TEST_CASE("CPU memory", "[meminfo]")
{
  vals m;
  
  // /!\ NOTE /!\ requires at least 1 mb of RAM on test system
  double tot_mb = m.totalram();
  REQUIRE( tot_mb > 1 );
  
  // zero-values indicate failure
  auto s = m.ramstats();
  REQUIRE( s.min() > 1 );
  REQUIRE( s.mean() > 1 );
  REQUIRE( s.max() > 1 );
}

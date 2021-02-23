#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <thread>

#include "argparse.hh"

#include "cpu/memory.hh"
#include "cpu/timer.hh"

#include "gpu/gpuinfo.hh"



int main(int argc, char **argv)
{
  proginfo::args a(argc, argv);
  
  proginfo::timer t;
  proginfo::meminfo m;
  proginfo::gpuinfo g;
  unsigned int ct = 0;
  
  std::atomic<bool> child_done(false);
  std::thread child([&child_done, &a] {
    int ret = std::system(a.get_cmd());
    if (ret < 0) // TODO actually deal with this
      throw std::runtime_error("Unable to launch command");
    child_done = true;
  });
  
  
  t.on();
  
  while (!child_done)
  {
    ct++;
    
    m.poll();
    g.poll();
    
    t.sleep(10);
  }
  
  child.join();
  t.off();
  
  
  printf("\n## Program Info (from %d polls)\n", ct);
  printf("* CPU\n");
  t.print();
  m.print();
  
  g.print();
  
  return 0;
}

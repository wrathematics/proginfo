#ifndef PROGINFO_PROCESS_H
#define PROGINFO_PROCESS_H
#pragma once


#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>


namespace proginfo
{
  class args
  {
    public:
      args(int argc_, char **argv_)
      {
        argc = argc_;
        argv = argv_;
        
        if (argc == 1)
          help();
        
        set_cmd();
      };
      
      
      
      const char *get_cmd() const { return cmd.c_str(); };
    
    
    
    protected:
      std::string cmd;
    
    
    
    private:
      int argc;
      char **argv;
      
      void help()
      {
        // TODO
        exit(EXIT_SUCCESS);
      };
      
      
      
      void set_cmd()
      {
        std::stringstream s;
        std::string argv_str;
        
        for (int i=1; i<argc; i++)
        {
          bool quote = false;
          argv_str = argv[i];
          
          if (argv_str.find(' ') < argv_str.length())
          {
            quote = true;
            s << "\'";
          }
          
          s << argv[i] << " ";
          
          if (quote)
            s << "\'";
        }
        
        cmd = s.str();
      }
  };
}


#endif

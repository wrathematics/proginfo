#ifndef PROGINFO_PROCESS_H
#define PROGINFO_PROCESS_H
#pragma once


#include <cstring>
#include <iostream>
#include <sstream>


namespace proginfo
{
  static inline std::stringstream process_args(int argc, char **argv)
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
    
    return s;
  }
}


#endif

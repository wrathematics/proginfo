#include "os.h"

#include <stdint.h>

typedef uint64_t memsize_t;

#define chkret(ret,val) if(ret)return(val)
#if OS_WINDOWS
// Windows.h functions return non-zero if successful IN SPITEFUL DEFIANCE OF THE FUCKING C STANDARD
#define winchkret(ret,val) if(!ret)return(val)
#endif

#define MEMINFO_OK       0
#define FAILURE         -1   // internal error
#define FILE_ERROR      -2   // bad file
#define CACHE_ERROR     -3   // impossible cache value
#define PLATFORM_ERROR  -10  // platform not supported





#if OS_LINUX

#include <sys/sysinfo.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#elif OS_MAC

#include <stdlib.h>

#include <mach/vm_statistics.h>
#include <mach/mach.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <sys/mount.h>


#elif OS_WINDOWS

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <Psapi.h>

typedef BOOL (WINAPI *LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);


#elif OS_FREEBSD

#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/sysctl.h>
#include <vm/vm_param.h>



#elif OS_NIX

#include <stdlib.h> 
#include <unistd.h>

#endif



// For some platforms, this is an empty file. The dummy typedef should shut them up
typedef int dummy_typedef;


#if OS_LINUX

// for getline()
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700

#include <inttypes.h>

static inline int read_proc_file(const char *file, memsize_t *val, char *field, int fieldlen)
{
  size_t len = 0;
  char *tmp = NULL;
  memsize_t value = FAILURE;
  
  *val = 0L;
  
  FILE* fp = fopen(file, "r");
  
  if (fp != NULL)
  {
    while (getline(&tmp, &len, fp) >= 0)
    {
      if (strncmp(tmp, field, fieldlen) == 0)
      {
        sscanf(tmp, "%*s%" SCNu64, &value);
        break;
      }
    }
    
    fclose(fp);
    free(tmp);
    
    if (value != (memsize_t)FAILURE)
    {
      *val = value;
      return MEMINFO_OK;
    }
  }
  
  return FAILURE;
}



#elif OS_MAC || OS_FREEBSD

#include "platform.h"

static inline int sysctl_mib(char *name, int *mib, size_t *mibsize){
  return sysctlnametomib(name,mib,mibsize);
}

static inline int sysctlmib_val(int *mib, size_t mibsize, void *data, size_t *datasize){
  return sysctl(mib, mibsize+1, data, datasize, NULL, 0);
}

static inline int sysctl_val(char *name, memsize_t *val)
{
  int ret;
  size_t vallen;
  vallen = sizeof(*val);
  
  ret = sysctlbyname(name, val, &vallen, NULL, 0);
  
  return ret;
}


#endif

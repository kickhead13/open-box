#ifndef __O_ERROR_
#define __O_ERROR_

#include <string.h>

#define __ERROR(message) write(2, (message), strlen((message)))
      
#define FAILED_TO_OPEN_BOXFILE_ERROR   " [x] ERROR(BOXFILE) failed to open/create BOXFILE or the file descriptor of the BOXFILE is not 3 (quitting...)\n"
#define FAILED_TO_OPEN_CRYPTFILE_ERROR " [x] ERROR(CRYPTFILE) failed to open/create CRYPTFILE for the file descriptor of the CRYPTFILE is not 4 (quitting...)\n"

#endif

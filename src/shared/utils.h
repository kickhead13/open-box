#ifndef __OPEN_BOX_UTILS
#define __OPEN_BOX_UTILS

#include <string.h>              /* strlen(..)          */
#include <pthread.h>             /* Mutex / pthreading. */
#include <unistd.h>

#define BOXFILE_NAME "BOXFILE"
#define CRYPTFILE_NAME "CRYPTFILE"

#define BOXFILE_FD   3           /* File descriptor for the BOXFILE. The BOXFILE file descriptor is the first opened file on the server so it should be always 3 */
#define CRYPTFILE_FD 4           /* Same here but for the CRYPTFILE. */

enum alg {
  FAIL = -1,
  RSA = 1,
  TOWF = 2,
  AES = 3,
  SHA256 = 4,
  SHA512 = 5,
  CHACHA = 6
};

struct eorder_t {
  enum alg         alg;   /* Algorithm (shared/utils.h) */
  char            *key;   /* Key for algorithm to use.  */
  unsigned int   enc:1;   /* Encryption / Decryption bit. */
  char             *fn;   /* Filename of file to be encrypted/decrypted/hashed  */
};

/* Writes to BOXFILE being given a struct eorder_t.
 * IMPORTANT: BOXFILE location is expected at to be 
 * within the directory from which the server is run.
 *
 * RETURN VALUE: -1 on error, >=0 on success
 * */
ssize_t write_order_to_box(char *restrict const);
#endif

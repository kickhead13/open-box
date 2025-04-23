#ifndef __EXECUTIONER_THREAD
#define __EXECUTIONER_THREAD
#include "../shared/crypto.h"
#include "../shared/utils.h"

/* Data for the executioner thread. */
struct executioner_thread_data {};
#define __ETD_CAST(x) ((struct executioner_thread_data)(x))

/* Executioner thread main function.
 * Behaviour: 
 *  - sleeps until inotify(..) wakes up on BOXFILE change
 *  - locks BOXFILE 
 *  - executes encryption
 *  - unlocks BOXFILE and goes back to sleep
 * */
void ethread(void*);

struct eorder_t {
  enum alg         alg;   /* Algorithm (shared/utils.h) */
  char            *key;   /* Key for algorithm to use.  */
  unsigned int   enc:1;   /* Encryption / Decryption bit. */
  char             *fn;   /* Filename of file to be encrypted/decrypted/hashed  */
};

/* Given a BOXFILE file descriptor, returns a execution order.
 * I.E. it reads and discards one of the lines of the BOXFILE,
 * and saves it in a eorder_t structure.
 * */
struct eorder_t next_eorder(size_t);


/* Given an execuiton order (eorder_t) it executes it, meaning
 * it encrypts/decrypts the file approprietly. It saves the 
 * filename of the original (before execution) file and
 * the new file (.crypt) in the CRYPTFILE, so that
 * uthreads can relay the post-operation file to the users.
 * 
 * RETURN VALUE: 0 on success, -1 on failure 
 * */
int8_t exec_order(struct eorder_t);

#endif

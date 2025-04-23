#ifndef __ADMIN_THREAD
#define __ADMIN_THREAD
/* Runtime for admin user thread. Handels AF_LOCAL(UNIX) socket
 * for user with admin role. Allows for monitoring server activity,
 * and interacting with ongoing processes.
 * */


/* User command number: 
 *  -1 unsupported
 *  0  Twofish encryption
 *  1  AES encryption
 *  2  EE encryption
 *  10 Twofish decryption
 *  11 AES decryption
 *  12 EE decryption
 * */
#define cmd_t ssize_t 

/* Data passed to admin handler thread. */
struct admin_thread_data {  
};
#define __ATD_CAST(x) ((struct admin_thread_data *) (x))
/* Main runtime loop of admin handler component of server.*/
void admin_handle(void *);

/* Type of command variables: a client would send a command 
 * and the server will parse it to a variable of this type;
 * */
struct a_cmd {
  size_t    argc;
  char    **args;
};
struct a_cmd parse_cmd(char *); /*parses `a_cmd` variable from str*/
void executioner(struct a_cmd); /*executes admin commands*/

/* Ongoing server processes struct. */
struct s_proc_info { 
  size_t     pid; /* process id */
  cmd_t      cmd; /* command number */
  char   fn[256]; /* file name of the file processed */
};

#endif // !__ADMIN_THREAD

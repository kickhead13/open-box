/* CENTRAL SERVER FOR OPEN.BOX ECOSYSTEM
 * 
 * Responsability: 
 *  - handles server - user communication 
 *      - initializes SECURE communication
 *          - first message with new client connection must be a RSA public key
 *          - client handler recieves user RSA public key encrypted with previous public key
 *          - client handler sends back a TWOFISH key encrypted with user's RSA public key
 *          - communication from this point forwards is done with TWOFISH encryption (every
 *            message in encrypted using TWOFISH, by both the client and the handler)
 *      - drops user requests into BOXFILE
 *          - BOXFILE looks like this:
 *          ###################################################################################
 *          # <ENCRYPTION ALG.(4B)><ENCRYPTION KEY><ENCRYPT(e)/DECRYPT(d) BYTE><FILENAME>     #
 *          # rsaa<RSA PUBLIC KEY>evideo.mp4                                                  #
 *          # rsaa<RSA PRIVATE KEY>dvideo.crypto                                              #
 *          # s256eefiletohash.txt                                                            #
 *          # twof<TWOFISH KEY>efiletoenc.txt                                                 #
 *          ###################################################################################
 *  - "executioner"
 *      - uses inotify(..) to check BOXFILE change
 *      - locks file descriptor and encrypts some of the files before unlocking the fd and 
 *        going to sleep
 * */


#include <unistd.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#include "server/ethread.h"
#include "shared/o_error.h"
#include "shared/utils.h"

int main() {
  
  if(open(BOXFILE_NAME, O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO) != 3) {
    __ERROR(FAILED_TO_OPEN_BOXFILE_ERROR);
    return -1;
  }
  
  if(open(CRYPTFILE_NAME, O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO) != 4) {
    __ERROR(FAILED_TO_OPEN_CRYPTFILE_ERROR);
    return -2;
  }

  write_order_to_box("testasd");
  close(BOXFILE_FD);
  close(CRYPTFILE_FD);
  return 0;
}

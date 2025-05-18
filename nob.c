#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define SRC_FOLDER "src/"
#ifndef INSTALL_FOLDER
#define BUILD_FOLDER "target/"
#else 
#define BUILD_FOLDER INSTALL_FOLDER
#endif

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    Cmd cmd = {0};
    Procs procs = {0};

    if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    // Spawn three async processes collecting them to procs dynamic array
#ifdef INSTALL_FOLDER 
    cmd_append(&cmd, "cc", "-DINSTALL_FOLDER=\""BUILD_FOLDER"\"", "-o", BUILD_FOLDER"openbox", SRC_FOLDER"openbox.c");
#else 
    cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"openbox", SRC_FOLDER"openbox.c");
#endif
    da_append(&procs, cmd_run_async_and_reset(&cmd));
    cmd_append(&cmd, "cc", "-o",
      BUILD_FOLDER"openbox_server", SRC_FOLDER"server.c", SRC_FOLDER"server/ethread.c", SRC_FOLDER"shared/rsa.c", SRC_FOLDER"shared/utils.c", SRC_FOLDER"shared/des.c"
      ,"-lgmp","-lnettle", "-lpthread");
    da_append(&procs, cmd_run_async_and_reset(&cmd));
    cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"openbox_client", SRC_FOLDER"uclient.c");
    da_append(&procs, cmd_run_async_and_reset(&cmd));
    cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"openbox_admin", SRC_FOLDER"aclient.c");
    da_append(&procs, cmd_run_async_and_reset(&cmd));
    cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"openbox_webapi", SRC_FOLDER"webapi.c");
    da_append(&procs, cmd_run_async_and_reset(&cmd));
    
        // Wait on all the async processes to finish
    if (!procs_wait_and_reset(&procs)) return 1;

    // TODO: add some examples with nob_procs_append_with_flush()

    return 0;
}

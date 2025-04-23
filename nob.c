#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define BUILD_FOLDER "target/"
#define SRC_FOLDER "src/"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    Cmd cmd = {0};
    Procs procs = {0};

    if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    // Spawn three async processes collecting them to procs dynamic array
    cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"server", SRC_FOLDER"server.c", SRC_FOLDER"server/ethread.c", SRC_FOLDER"shared/crypto.c", "-lnettle");
    da_append(&procs, cmd_run_async_and_reset(&cmd));
    cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"uclient", SRC_FOLDER"uclient.c");
    da_append(&procs, cmd_run_async_and_reset(&cmd));
    cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"aclient", SRC_FOLDER"aclient.c");
    da_append(&procs, cmd_run_async_and_reset(&cmd));
    cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"webapi", SRC_FOLDER"webapi.c");
    da_append(&procs, cmd_run_async_and_reset(&cmd));
    
        // Wait on all the async processes to finish
    if (!procs_wait_and_reset(&procs)) return 1;

    // TODO: add some examples with nob_procs_append_with_flush()

    return 0;
}

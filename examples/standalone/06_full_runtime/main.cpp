// A standalone context on the full runtime, linking C++ modules.
//
// dasHV (an HTTP client) and fio (a child process) come in as static archives,
// so the binary compiles nothing at run time and loads no shared module - the
// shape of a supervisor that must never hold a lock on the files it deploys. The
// generated constructor registers the modules it links; main constructs the
// context and calls what the script exported.
//
//   service_probe [url]        probe `url` (default: a port nobody listens on),
//                              then run a copy of this program as the child
//   service_probe --child      the child: print one line, exit 7

#include "daScript/daScript.h"
#include "service_probe.das.h"

#include <stdio.h>
#include <string.h>

using namespace das;

int main ( int argc, char * argv[] ) {
    if ( argc > 1 && strcmp(argv[1], "--child") == 0 ) {
        printf("hello from the child\n");
        return 7;
    }
    const char * url = argc > 1 ? argv[1] : "http://127.0.0.1:1/";
    service_probe::Standalone ctx;
    const int status = ctx.http_status((char *)url);
    printf("GET %s -> %d%s\n", url, status, status < 0 ? " (nobody answered)" : "");
    const bool status_is_sane = status == -1 || (status >= 100 && status <= 599);
    const int code = ctx.run_child(argv[0], (char *)"--child");
    printf("child exit code %d\n", code);
    return (status_is_sane && code == 7) ? 0 : 1;
}

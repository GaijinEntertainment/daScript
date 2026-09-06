// Two generated standalone contexts with different C++ module sets in one binary: the dasHV
// + fio example and a fio-only fixture. Each TU adds its modules before main; the first context
// constructed registers the union and initializes once; the last one destroyed shuts down. A
// second owner would shut the registry down twice, and an unbalanced Initialize trips the exit
// audit - both fail this test's exit code.
//
//   test_standalone_modules                  the generated code owns the registry lifetime
//   test_standalone_modules --host-registers main registers every module first and shuts
//                                            down itself; the generated code registers nothing
//   test_standalone_modules --host-partial   main registers the builtin set but not dasHV:
//                                            the generated code stops the program by name
//                                            instead of taking a second registry lifetime
//   test_standalone_modules --child          the child process run_child spawns

#include "daScript/daScript.h"
#include "daScript/daScriptModule.h"
#include "service_probe.das.h"
#include "standalone_modules_fixture.das.h"

#include <stdio.h>
#include <string.h>

using namespace das;

DECLARE_MODULE(Module_HV);

static int run_contexts ( char * self ) {
    int failures = 0;
    standalone_modules_fixture::Standalone fio_only;
    if ( !fio_only.has_path_variable() ) {
        printf("has_path_variable() = false, expected true\n");
        failures ++;
    }
    service_probe::Standalone probe;
    const int status = probe.http_status((char *)"http://127.0.0.1:1/");
    if ( status != -1 ) {
        printf("http_status(dead port) = %d, expected -1\n", status);
        failures ++;
    }
    const int code = probe.run_child(self, (char *)"--child");
    if ( code != 7 ) {
        printf("run_child() = %d, expected 7\n", code);
        failures ++;
    }
    return failures;
}

int main ( int argc, char * argv[] ) {
    if ( argc > 1 && strcmp(argv[1], "--child") == 0 ) {
        printf("child\n");
        return 7;
    }
    const bool host_registers = argc > 1 && strcmp(argv[1], "--host-registers") == 0;
    const bool host_partial = argc > 1 && strcmp(argv[1], "--host-partial") == 0;
    if ( host_registers || host_partial ) {
        NEED_ALL_DEFAULT_MODULES;
        NEED_MODULE(Module_UriParser);
        NEED_MODULE(Module_JobQue);
        if ( host_registers ) {
            NEED_MODULE(Module_HV);
        }
        Module::Initialize();
    }
    const int failures = run_contexts(argv[0]);
    if ( host_registers || host_partial ) {
        Module::Shutdown();
    }
    printf(failures ? "standalone_modules: %d failure(s)\n" : "standalone_modules: ok\n", failures);
    return failures ? 1 : 0;
}

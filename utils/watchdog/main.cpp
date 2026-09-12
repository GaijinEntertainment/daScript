// The watchdog as a standalone context on the full runtime: libDaScript and dasHV linked as static
// archives, no compiler, no shared module, no lock on any file a deploy replaces. main owns what a
// context cannot - argv, the process id, the signals - and drives the supervisor one tick at a time;
// everything else is utils/watchdog/watchdog.das.

#include "daScript/daScript.h"
#include "daScript/simulate/aot_builtin.h"
#include "main.das.h"

#include <atomic>
#include <csignal>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace das;

static std::atomic<bool> g_stop_requested{false};

static void on_stop_signal ( int ) {
    g_stop_requested = true;
}

int main ( int argc, char * argv[] ) {
    setCommandLineArguments(argc, argv);
    std::signal(SIGINT, on_stop_signal);
    std::signal(SIGTERM, on_stop_signal);
#ifdef _WIN32
    std::signal(SIGBREAK, on_stop_signal);
    const int32_t pid = (int32_t)GetCurrentProcessId();
#else
    const int32_t pid = (int32_t)getpid();
#endif
    ctx_main::Standalone ctx;
    if ( !ctx.start(pid) ) return ctx.result();
    bool done = false;
    while ( !done ) {
        if ( g_stop_requested ) ctx.request_stop();
        done = ctx.tick();
    }
    return ctx.result();
}

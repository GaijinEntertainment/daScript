#include "daScript/misc/platform.h"
#include "daScript/misc/handle_registry.h"

namespace das {

    // Singletons live in the main DAS library so all DLLs (daslang.exe + each
    // dasModule*.shared_module) share one hooks vector and one mutex.
    // Without this, inline function-local statics would be per-DLL and dump
    // callbacks registered from a module DLL would never be seen by the host.

    DAS_API vector<HandleLeakDumpFn> & handleRegistry_dumpHooks_impl () {
        static vector<HandleLeakDumpFn> hooks;
        return hooks;
    }

    DAS_API mutex & handleRegistry_dumpMutex_impl () {
        static mutex m;
        return m;
    }

    DAS_API void handleRegistry_registerDump_impl ( HandleLeakDumpFn fn ) {
        lock_guard<mutex> g(handleRegistry_dumpMutex_impl());
        auto & hooks = handleRegistry_dumpHooks_impl();
        for ( auto e : hooks ) if ( e == fn ) return;
        hooks.push_back(fn);
    }

    DAS_API void handleRegistry_dumpAll_impl () {
        // Snapshot hooks under the registry mutex, then invoke outside so
        // that a callback freeing handles (or re-registering) cannot deadlock
        // against a concurrent addHandleAnnotation<T> on another thread.
        vector<HandleLeakDumpFn> hooks;
        {
            lock_guard<mutex> g(handleRegistry_dumpMutex_impl());
            hooks = handleRegistry_dumpHooks_impl();
        }
        for ( auto fn : hooks ) fn();
    }

}

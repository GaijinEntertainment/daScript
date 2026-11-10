// Two processes compiling the same program write the same module cache file: each writes a
// temp file and renames it over the target. On Windows the rename cannot replace a file
// another process holds open, so the second writer's rename fails while the first writer's
// complete stream sits at the path - that is not a failed save, and finish() must not report
// one (the host printed "ser: cannot write" into a protocol server's stdout for it).

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/ast/ast_serializer.h"
#include "daScript/misc/sysos.h"
#include "daScript/simulate/aot_builtin_fio.h"

#include <cstdio>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

using namespace das;

namespace {

ModuleFileCache::Result compile_with_cache ( const string & script, const string & cachePath ) {
    TextPrinter tout;
    ModuleFileCache cache;
    ModuleGroup libGroup;
    auto fAccess = make_smart<FsFileAccess>();
    cache.install(cachePath, cachePath, true);
    CodeOfPolicies policies;
    auto program = compileDaScript(script, fAccess, tout, libGroup, policies, cache.stream());
    const bool compiled = program && !program->failed();
    REQUIRE_MESSAGE(compiled, "the probe program failed to compile");
    return cache.finish();
}

void write_text ( const string & path, const char * text ) {
    FILE * f = das_fopen_utf8(path.c_str(), "wb");
    REQUIRE_MESSAGE(f != nullptr, "cannot write " << path);
    fputs(text, f);
    fclose(f);
}

long long process_id () {
#ifdef _WIN32
    return _getpid();
#else
    return getpid();
#endif
}

// the probe and its cache carry the process id, so parallel shards never share them, and
// both go away however the test case ends
struct ProbeFiles {
    string script;
    string cachePath;
    ProbeFiles () {
        const string dir = getDasRoot() + "/.cache/daslang/";   // gitignored, outside the pruned default directory
        builtin_mkdir((getDasRoot() + "/.cache").c_str());
        builtin_mkdir(dir.c_str());
        const string stem = dir + "tests_cpp_cache_rename_race." + to_string(process_id());
        script = stem + ".das";
        cachePath = stem + ".dascache";
        remove(cachePath.c_str());
    }
    ~ProbeFiles () {
        remove(cachePath.c_str());
        remove(script.c_str());
    }
};

}

TEST_CASE("module cache: a write that loses the rename to another writer is not a failed save") {
    if ( Module::requireEx("debug", true) ) {
        MESSAGE("skipped: the debugger is promoted in this process, so no compile serializes");
        return;
    }
    ProbeFiles files;

    write_text(files.script, "options gen2\n[export]\ndef main() {\n    print(\"one\\n\")\n}\n");
    auto cold = compile_with_cache(files.script, files.cachePath);
    REQUIRE_MESSAGE(cold.wrote, "the cold compile wrote no cache");

    // the source changes, so the next compile has a new stream to write; the cache file is
    // held open meanwhile, the way a reader in another process holds it
    write_text(files.script, "options gen2\n[export]\ndef main() {\n    print(\"two\\n\")\n}\n");
    FILE * held = das_fopen_utf8(files.cachePath.c_str(), "rb");
    REQUIRE_MESSAGE(held != nullptr, "cannot hold the cache file open");
    auto contended = compile_with_cache(files.script, files.cachePath);
    fclose(held);
    CHECK_FALSE_MESSAGE(contended.saveFailed, "a rename lost to an open cache file was reported as a failed save");

    uint64_t size = 0; int64_t mtime = 0; bool isDirectory = false;
    const bool present = das_file_stat_utf8(files.cachePath.c_str(), size, mtime, isDirectory) && size > 0;
    CHECK_MESSAGE(present, "the cache file is gone or empty after the contended write");
}

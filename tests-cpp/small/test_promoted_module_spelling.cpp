#include <doctest/doctest.h>
#include "daScript/daScript.h"

using namespace das;

// A promoted (shared) module must be identity-matched by the file it was built from, not only
// by the require spelling it was promoted with — the same file is legitimately spelled several
// ways (mount prefix, directory-relative vs bare). Rejecting a legitimate respelling compiles
// and promotes a SECOND instance of the module; a later compile that reaches the first instance
// through one require and the second through another gets a library where the module NAME binds
// the duplicate, and a fresh generic instantiation inside the first instance stops resolving:
//
//   error[30341]: no matching functions or generics: square(float const&)
//       while compiling common`use_square` ... instanced from main
//
// It takes three compiles in one process to surface (found in dagor's multi-file AOT step):
//   prog1 promotes promo_base as 'promo_base' (same-dir spelling inside promo_common)
//   prog2 requires the same file as 'promo_libs/promo_base' — the respelling under test
//   prog3 reaches both spellings at once
// Without the file-identity match in Module::requireEx, prog2 duplicates the module and prog3
// fails with the error above.

static const char * PROMO_BASE =
    "options gen2\n"
    "module promo_base shared public\n"
    "def promo_square(x) {\n"
    "    return x * x\n"
    "}\n";

static const char * PROMO_COMMON =
    "options gen2\n"
    "module promo_common shared public\n"
    "require promo_base\n"
    "def promo_use_square(x) {\n"
    "    return promo_square(x)\n"
    "}\n";

static const char * PROG1 =
    "options gen2\n"
    "require promo_libs/promo_common\n"
    "[export] def main() { print(\"{promo_use_square(2)}\\n\") }\n";

static const char * PROG2 =
    "options gen2\n"
    "require promo_libs/promo_base\n"
    "[export] def main() { print(\"{promo_square(4.0)}\\n\") }\n";

static const char * PROG3 =
    "options gen2\n"
    "require promo_libs/promo_base\n"
    "require promo_libs/promo_common\n"
    "[export] def main() { print(\"{promo_use_square(3.5)} {promo_square(4.0)}\\n\") }\n";

static ProgramPtr compile_one ( const char * name, const char * src ) {
    TextWriter tout;
    ModuleGroup libGroup;
    auto fa = make_smart<FsFileAccess>();
    fa->setFileInfo("promo_libs/promo_base.das",
        make_unique<TextFileInfo>(PROMO_BASE, uint32_t(strlen(PROMO_BASE)), false));
    fa->setFileInfo("promo_libs/promo_common.das",
        make_unique<TextFileInfo>(PROMO_COMMON, uint32_t(strlen(PROMO_COMMON)), false));
    fa->setFileInfo(name, make_unique<TextFileInfo>(src, uint32_t(strlen(src)), false));
    auto program = compileDaScript(name, fa, tout, libGroup);
    if ( program->failed() ) {
        for ( auto & err : program->errors ) {
            MESSAGE(name << ": " << reportError(err.at, err.what, err.extra, err.fixme, err.cerr));
        }
    }
    return program;
}

// promoted modules live in the process-global registry until Module::Shutdown; delete this
// test's own so repeated in-process runs start clean and the suite-end leak check stays green
static void unpromote ( const char * name ) {
    vector<Module *> victims;
    Module::foreach([&](Module * m){
        if ( m->promoted && m->name==name ) victims.push_back(m);
        return true;
    });
    for ( auto v : victims ) delete v;
}

TEST_CASE("promoted shared module matches by file across require spellings") {
    auto p1 = compile_one("promo_prog1.das", PROG1);
    CHECK_FALSE(p1->failed());
    auto p2 = compile_one("promo_prog2.das", PROG2);
    CHECK_FALSE(p2->failed());
    auto p3 = compile_one("promo_prog3.das", PROG3);
    CHECK_FALSE(p3->failed());
    unpromote("promo_common");
    unpromote("promo_base");
}

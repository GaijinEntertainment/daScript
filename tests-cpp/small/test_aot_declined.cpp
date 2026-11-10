#include <doctest/doctest.h>
#include <cstring>
#include "daScript/daScript.h"
#include "daScript/simulate/aot_library.h"

using namespace das;

extern "C" void das_aot_register ( uint64_t aotHash, void * publ );
extern "C" void das_aot_register_declined ( uint64_t aotHash, const char * reason );

namespace {
    const char * PROBE_SOURCE = "options gen2\n[export]\ndef main : int {\n    return 7\n}\n";

    ProgramPtr compileProbe ( TextWriter & tout, ModuleGroup & group, bool strictAot ) {
        auto access = make_smart<FsFileAccess>();
        access->setFileInfo("aot_declined_probe.das",
            make_unique<TextFileInfo>(PROBE_SOURCE, uint32_t(strlen(PROBE_SOURCE)), false));
        CodeOfPolicies policies;
        policies.aot = true;
        policies.fail_on_no_aot = strictAot;
        policies.keep_symbol_tables = true;
        return compileDaScript("aot_declined_probe.das", access, tout, group, policies);
    }

    Function * mainOf ( const ProgramPtr & program ) {
        Function * found = nullptr;
        program->thisModule->functions.foreach([&](auto fn) {
            if ( fn->name == "main" ) found = fn;
        });
        return found;
    }

    bool errorsMention ( const vector<Error> & errors, const char * text ) {
        for ( auto & err : errors ) {
            if ( err.what.find(text) != string::npos || err.extra.find(text) != string::npos ) return true;
        }
        return false;
    }
}

TEST_CASE("an AOT object's declined function registers its reason, and an emitted body wins over it") {
    das_aot_register_declined(0xDEC1u, "probe reason");
    das_aot_register(0xDEC2u, (void *) &das_aot_register);
    das_aot_register_declined(0xDEC2u, "late placeholder");
    AotLibrary lib;
    AotListBase::registerAot(lib);
    REQUIRE(lib.count(0xDEC1u) == 1);
    CHECK(string(lib.at(0xDEC1u).declineReason) == "probe reason");
    REQUIRE(lib.count(0xDEC2u) == 1);
    CHECK(lib.at(0xDEC2u).declineReason == nullptr);
    getGlobalAotLibrary().erase(0xDEC1u);
    getGlobalAotLibrary().erase(0xDEC2u);
}

TEST_CASE("a declined function links as a reported interpreter fallback, an error only under fail_on_no_aot") {
    TextWriter tout;
    ModuleGroup group;
    auto probe = compileProbe(tout, group, false);
    REQUIRE(probe);
    INFO(tout.str());
    REQUIRE_FALSE(probe->failed());
    Context first(probe->getContextStackSize());
    REQUIRE(probe->simulate(first, tout));
    auto mainFn = mainOf(probe);
    REQUIRE(mainFn);
    const uint64_t hash = getFunctionAotHash(probe.get(), mainFn);
    aotLibraryEmplace(getGlobalAotLibrary(), hash, AotFactory::declined("the probe declines main"));

    auto strict = compileProbe(tout, group, true);
    REQUIRE(strict);
    REQUIRE_FALSE(strict->failed());
    Context strictCtx(strict->getContextStackSize());
    CHECK_FALSE(strict->simulate(strictCtx, tout));
    CHECK(errorsMention(strict->errors, "LLVM AOT declined this function"));
    CHECK(errorsMention(strict->errors, "the probe declines main"));
    CHECK_FALSE(errorsMention(strict->errors, "stale"));

    auto lenient = compileProbe(tout, group, false);
    REQUIRE(lenient);
    REQUIRE_FALSE(lenient->failed());
    Context lenientCtx(lenient->getContextStackSize());
    REQUIRE(lenient->simulate(lenientCtx, tout));
    CHECK(errorsMention(lenient->aotErrors, "the probe declines main"));
    auto fn = lenientCtx.findFunction("main");
    REQUIRE(fn);
    auto result = lenientCtx.evalWithCatch(fn, nullptr);
    CHECK(lenientCtx.getException() == nullptr);
    CHECK(cast<int32_t>::to(result) == 7);

    getGlobalAotLibrary().erase(hash);
}

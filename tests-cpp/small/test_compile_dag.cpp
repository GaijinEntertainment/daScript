#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/ast/ast_serializer.h"

using namespace das;

TEST_CASE("compile DAG: four threads compile the modules one thread compiles") {
    auto compileWith = []( int32_t threads ) {
        auto fAccess = make_smart<FsFileAccess>();
        TextWriter logs;
        ModuleGroup libGroup;
        CodeOfPolicies policies;
        policies.compile_threads = threads;
        auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_compile_dag.das", fAccess, logs, libGroup, policies);
        REQUIRE_MESSAGE(!program->failed(), logs.str());
        vector<string> names;
        for ( auto mod : libGroup.getModules() ) names.push_back(mod->name + (mod->macroNestsCompile ? " nesting" : ""));
        sort(names.begin(), names.end());
        return names;
    };
    auto parallel = compileWith(4);
    CHECK(find(parallel.begin(), parallel.end(), "test_compile_dag_nesting nesting") != parallel.end());
    CHECK(find(parallel.begin(), parallel.end(), "test_compile_dag_leaf") != parallel.end());
    CHECK_EQ(parallel, compileWith(1));
}

TEST_CASE("compile DAG: the modules after an !inscope module wait for it, so its comment reader sees a later module") {
    auto fAccess = make_smart<FsFileAccess>();
    TextWriter logs;
    ModuleGroup libGroup;
    CodeOfPolicies policies;
    policies.compile_threads = 4;
    auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_compile_dag_reader_root.das", fAccess, logs, libGroup, policies);
    REQUIRE_MESSAGE(!program->failed(), logs.str());
    auto reader = libGroup.findModule("test_compile_dag_reader");
    REQUIRE(reader);
    REQUIRE(reader->macroContext);
    auto ctx = reader->macroContext.get();
    auto index = ctx->findVariable("sibling_comment_chars");
    REQUIRE(index >= 0);
    CHECK_GT(*(int32_t *)ctx->getVariable(index), 0);
}

TEST_CASE("compile DAG: a module that installs a comment reader without !inscope fails to compile") {
    auto fAccess = make_smart<FsFileAccess>();
    auto dir = getDasRoot() + "/tests-cpp/small/";
    string reader = "options gen2\nmodule test_compile_dag_unmarked public\nrequire daslib/ast_boost\n"
        "require test_compile_dag_reader\n[comment_reader]\nclass UnmarkedCommentCounter : SiblingCommentCounter {}\n";
    string root = "options gen2\nrequire test_compile_dag_unmarked\n";
    fAccess->setFileInfo(dir + "test_compile_dag_unmarked.das", make_unique<TextFileInfo>(reader.c_str(), uint32_t(reader.size()), false));
    fAccess->setFileInfo(dir + "test_compile_dag_unmarked_root.das", make_unique<TextFileInfo>(root.c_str(), uint32_t(root.size()), false));
    TextWriter logs;
    ModuleGroup libGroup;
    auto program = compileDaScript(dir + "test_compile_dag_unmarked_root.das", fAccess, logs, libGroup);
    REQUIRE(program->failed());
    string errors;
    for ( auto & err : program->errors ) errors += err.what + " " + err.extra + "\n";
    CHECK_MESSAGE(errors.find("!inscope") != string::npos, errors);
}

TEST_CASE("compile DAG: a compile that starts with a comment reader loaded runs its jobs, and the reader reads what one thread reads") {
    auto readWith = []( int32_t threads ) {
        auto fAccess = make_smart<FsFileAccess>();
        TextWriter logs;
        ModuleGroup libGroup;
        CodeOfPolicies policies;
        policies.compile_threads = 1;
        auto withReader = compileDaScript(getDasRoot() + "/tests-cpp/small/test_compile_dag_reader_root.das", fAccess, logs, libGroup, policies);
        REQUIRE_MESSAGE(!withReader->failed(), logs.str());
        auto ctx = libGroup.findModule("test_compile_dag_reader")->macroContext.get();
        auto chars = (int32_t *)ctx->getVariable(ctx->findVariable("all_comment_chars"));
        auto before = *chars;
        policies.compile_threads = threads;
        auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_compile_dag_reader_jobs.das", fAccess, logs, libGroup, policies);
        REQUIRE_MESSAGE(!program->failed(), logs.str());
        return *chars - before;
    };
    readWith(1);
    auto read = readWith(1);
    CHECK_GT(read, 0);
    CHECK_EQ(readWith(4), read);
}

TEST_CASE("compile DAG deadlock prevention: a macro module that forks a debug agent context is nesting") {
    auto fAccess = make_smart<FsFileAccess>();
    TextWriter logs;
    ModuleGroup libGroup;
    CodeOfPolicies policies;
    policies.compile_threads = 1;
    auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_compile_dag_fork_root.das", fAccess, logs, libGroup, policies);
    REQUIRE_MESSAGE(!program->failed(), logs.str());
    auto fork = libGroup.findModule("test_compile_dag_fork");
    REQUIRE(fork);
    REQUIRE(fork->macroContext);
    CHECK(fork->macroNestsCompile);
}

TEST_CASE("compile DAG deadlock prevention: a macro module reaching a nesting builtin through a helper is nesting") {
    auto fAccess = make_smart<FsFileAccess>();
    TextWriter logs;
    ModuleGroup libGroup;
    auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_compile_dag.das", fAccess, logs, libGroup);
    REQUIRE_MESSAGE(!program->failed(), logs.str());
    auto nesting = libGroup.findModule("test_compile_dag_nesting");
    auto leaf = libGroup.findModule("test_compile_dag_leaf");
    REQUIRE(nesting);
    REQUIRE(leaf);
    REQUIRE(nesting->macroContext);
    REQUIRE(leaf->macroContext);
    CHECK(nesting->macroNestsCompile);
    CHECK_FALSE(leaf->macroNestsCompile);
}

TEST_CASE("compile DAG cached prefix: a cache four threads wrote serves clean, and an edit before both macro modules recompiles them in parallel") {
    auto fAccess = make_smart<FsFileAccess>();
    auto root = getDasRoot() + "/tests-cpp/small/";
    auto compileWith = [&]( SerializationStorageVector * readFrom, SerializationStorageVector * writeTo ) {
        TextWriter logs;
        ModuleGroup libGroup;
        CodeOfPolicies policies;
        policies.compile_threads = 4;
        unique_ptr<AstSerializer> reader, writer;
        if ( readFrom ) {
            readFrom->bufferPos = 0;
            reader = make_unique<AstSerializer>(readFrom, false);
        }
        if ( writeTo ) writer = make_unique<AstSerializer>(writeTo, true);
        auto program = compileDaScript(root + "test_compile_dag.das", fAccess, logs, libGroup, policies,
            ModuleCacheStream{reader.get(), writer.get()});
        REQUIRE_MESSAGE(!program->failed(), logs.str());
        struct { bool failed; uint64_t served, resumed; string cutoff, reason; } read = { false, 0, 0, "", "" };
        if ( reader ) read = { reader->failed, reader->servedModules, reader->resumedModules, reader->cutoffFile, reader->cutoffReason };
        if ( reader ) reader->moduleLibrary = nullptr;
        if ( writer ) writer->moduleLibrary = nullptr;
        return read;
    };
    compileWith(nullptr, nullptr);
    SerializationStorageVector cold;
    compileWith(nullptr, &cold);
    REQUIRE(cold.writingSize() > 0);
    auto warm = compileWith(&cold, nullptr);
    CHECK_FALSE(warm.failed);
    CHECK(warm.resumed == 0);
    CHECK(warm.served > 0);
    auto edit = root + "test_compile_dag_base.das";
    string source;
    {
        auto fi = fAccess->getFileInfo(edit);
        REQUIRE(fi);
        const char * src = nullptr;
        uint32_t len = 0;
        fi->getSourceAndLength(src, len);
        source.assign(src, len);
    }
    source += "\n// edited\n";
    fAccess->setFileInfo(edit, make_unique<TextFileInfo>(source.c_str(), uint32_t(source.size()), false));
    SerializationStorageVector edited;
    auto cut = compileWith(&cold, &edited);
    CHECK(cut.failed);
    CHECK(cut.cutoff == edit);
    CHECK(cut.reason == "file changed");
    REQUIRE(edited.writingSize() > 0);
    auto rewarm = compileWith(&edited, nullptr);
    CHECK_FALSE(rewarm.failed);
    CHECK(rewarm.served == warm.served);
}

TEST_CASE("compile DAG: a structure of a frozen module reads its aliases without a lock, so jobs share it") {
    auto fAccess = make_smart<FsFileAccess>();
    TextWriter logs;
    ModuleGroup libGroup;
    CodeOfPolicies policies;
    policies.compile_threads = 4;
    auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_compile_dag.das", fAccess, logs, libGroup, policies);
    REQUIRE_MESSAGE(!program->failed(), logs.str());
    int32_t checked = 0;
    auto checkModule = [&]( Module * mod ) {
        if ( !mod->frozen ) return true;
        for ( auto & st : mod->structures.each() ) {
            CHECK_MESSAGE(st->aliases.isFrozen(), mod->name, "::", st->name);
            checked ++;
        }
        return true;
    };
    libGroup.foreach(checkModule, "*");
    Module::foreach(checkModule);
    CHECK(checked > 0);
}

TEST_CASE("compile DAG: a module that writes or folds a global of a frozen module leaves that global's flags alone, so jobs share it") {
    auto fAccess = make_smart<FsFileAccess>();
    TextWriter logs;
    ModuleGroup libGroup;
    CodeOfPolicies policies;
    policies.compile_threads = 1;
    auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_compile_dag.das", fAccess, logs, libGroup, policies);
    REQUIRE_MESSAGE(!program->failed(), logs.str());
    auto base = libGroup.findModule("test_compile_dag_base");
    REQUIRE(base);
    REQUIRE(base->frozen);
    auto counter = base->findVariable("compile_dag_counter");
    REQUIRE(counter);
    CHECK_FALSE(bool(counter->access_ref));
    CHECK_FALSE(bool(counter->access_info_pass_mutable));
    auto limit = base->findVariable("compile_dag_limit");
    REQUIRE(limit);
    CHECK_FALSE(bool(limit->access_fold));
}

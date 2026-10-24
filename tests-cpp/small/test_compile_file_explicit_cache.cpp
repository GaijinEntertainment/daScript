#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/ast/ast_serializer.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_builtin_fio.h"
#include "daScript/simulate/aot_builtin_rtti.h"

#include <stdexcept>

using namespace das;

#if !DAS_NO_FILEIO
namespace {

struct ExplicitCacheFixture {
    Context context { 65536 };
    string dir;
    ExplicitCacheFixture () {
        context.setup(0, 0, CodeOfPolicies(), AnnotationArgumentList());
        char * error = nullptr;
        auto path = builtin_fs_create_temp_directory(
            "das_explicit_cache", error, &context, nullptr);
        REQUIRE_MESSAGE(path != nullptr, (error ? error : "no temp path"));
        dir = path;
    }
    ~ExplicitCacheFixture () {
        char * error = nullptr;
        CHECK(builtin_rmdir_rec_ec(dir.c_str(), error, &context, nullptr));
    }
    template <typename Callback>
    string compile ( const string & file, const string & cache,
            FileAccessPtr access, ModuleGroup & group,
            Callback && callback, bool noopt = false ) {
        das_stack_prologue frame(&context, 256, "explicit_cache_callback");
        auto onCompiled = [&](bool ok, ProgramPtr program, string * issues) {
            callback(ok, program, *issues);
        };
        das_make_block<void,bool,ProgramPtr,string *> block(
            &context, uint32_t(sizeof(Prologue)), 0, nullptr, onCompiled);
        CodeOfPolicies policies;
        policies.ignore_shared_modules = true;
        policies.no_optimizations = noopt;
        return rtti_builtin_compile_file_cached(
            const_cast<char *>(file.c_str()), access, &group, policies,
            const_cast<char *>(cache.c_str()), block, &context, nullptr);
    }
};

struct OuterSerializers {
    daScriptEnvironment & env = *daScriptEnvironment::getBound();
    AstSerializer * read = env.serializer_read;
    AstSerializer * write = env.serializer_write;
    SerializationStorageVector readStorage, writeStorage;
    AstSerializer reader { &readStorage, false };
    AstSerializer writer { &writeStorage, true };
    OuterSerializers () {
        env.serializer_read = &reader;
        env.serializer_write = &writer;
    }
    ~OuterSerializers () {
        env.serializer_read = read;
        env.serializer_write = write;
    }
    void check () {
        CHECK(env.serializer_read == &reader);
        CHECK(env.serializer_write == &writer);
        CHECK(readStorage.buffer.empty());
        CHECK(writeStorage.buffer.empty());
    }
};

struct ThrowingFileAccess : FsFileAccess {
    FileInfo * getNewFileInfo ( const string & ) override {
        throw std::runtime_error("explicit cache compile failure");
    }
};

struct ExplicitCacheLocation : FunctionAnnotation {
    FileInfoPtr source = make_unique<FileInfo>();
    ExplicitCacheLocation () : FunctionAnnotation("explicit_cache_location") {
        source->name = "generated-explicit-cache-location";
    }
    bool apply ( const FunctionPtr & function, ModuleGroup &,
            AnnotationArgumentList &, string & ) override {
        function->at.fileInfo = source.get();
        function->at.line = 17;
        function->at.last_line = 17;
        return true;
    }
    bool finalize ( const FunctionPtr &, ModuleGroup &,
            AnnotationArgumentList &, const AnnotationArgumentList &,
            string & ) override {
        return true;
    }
    bool apply ( ExprBlock *, ModuleGroup &, AnnotationArgumentList &,
            string & ) override {
        return false;
    }
    bool finalize ( ExprBlock *, ModuleGroup &, AnnotationArgumentList &,
            const AnnotationArgumentList &, string & ) override {
        return false;
    }
};

struct LocationRegistration {
    Module module { "explicit_cache_test" };
    ExplicitCacheLocation * annotation = new ExplicitCacheLocation();
    LocationRegistration () {
        module.addAnnotation(annotation);
        module.gc_collect(&gc_root::gc_get_thread_root());
    }
};

FileInfo * checkLocation ( const ProgramPtr & program,
        const ExplicitCacheLocation & annotation ) {
    auto function = program->thisModule->findUniqueFunction("probe");
    REQUIRE(function != nullptr);
    REQUIRE(function->at.fileInfo != nullptr);
    CHECK(function->at.fileInfo != annotation.source.get());
    CHECK(function->at.fileInfo->name == annotation.source->name);
    CHECK(function->at.line == 17);
    CHECK_FALSE(function->at.describe().empty());
    return function->at.fileInfo;
}

}

TEST_CASE("explicit compile_file cache restores outer serializer slots") {
    ExplicitCacheFixture fixture;
    auto file = getDasRoot() + "/tests-cpp/small/test_env_serializer.das";
    auto cache = fixture.dir + "/cache.bin";
    OuterSerializers outer;
    ModuleGroup group;
    auto access = make_smart<FsFileAccess>();
    bool called = false;
    auto callback = [&](bool ok, ProgramPtr program, const string issues) {
        called = true;
        REQUIRE_MESSAGE(ok, issues);
        REQUIRE(program != nullptr);
        outer.check();
    };
    SUBCASE("empty explicit path hides the outer streams") {
        CHECK(fixture.compile(file, "", access, group, callback) == "none");
    }
    SUBCASE("a null file access uses the filesystem") {
        CHECK(fixture.compile(file, cache, nullptr, group,
            callback) == "none");
    }
    SUBCASE("cold and warm explicit paths restore both slots") {
        CHECK(fixture.compile(file, cache, access, group, callback) == "none");
        ModuleGroup warmGroup;
        CHECK(fixture.compile(file, cache, make_smart<FsFileAccess>(),
            warmGroup, callback) == "clean");
    }
    SUBCASE("different compile policies return a partial verdict") {
        CHECK(fixture.compile(file, cache, access, group, callback) == "none");
        ModuleGroup warmGroup;
        CHECK(fixture.compile(file, cache, make_smart<FsFileAccess>(),
            warmGroup, callback, true) == "partial");
    }
    SUBCASE("failed cache writes reach the compile callback") {
        auto blocked = fixture.dir + "/regular-file";
        auto handle = fopen(blocked.c_str(), "wb");
        REQUIRE(handle != nullptr);
        fclose(handle);
        fixture.compile(file, blocked + "/cache.bin", access, group,
            [&](bool ok, ProgramPtr, const string issues) {
                called = true;
                CHECK(ok);
                CHECK(issues.find("ser: cannot write '") != string::npos);
                CHECK(issues.find(blocked) != string::npos);
                outer.check();
            });
    }
    SUBCASE("nested compilation failure restores before its callback") {
        fixture.compile(file, cache, access, group,
            [&](bool ok, ProgramPtr, const string issues) {
                REQUIRE_MESSAGE(ok, issues);
                ModuleGroup nestedGroup;
                fixture.compile(file + ".missing", "", access, nestedGroup,
                    [&](bool nestedOk, ProgramPtr, const string) {
                        called = true;
                        CHECK_FALSE(nestedOk);
                        outer.check();
                    });
                outer.check();
            });
    }
    SUBCASE("a callback exception leaves the outer slots installed") {
        auto boundProgram = outer.env.g_Program;
        CHECK_THROWS_AS(fixture.compile(file, cache, access, group,
            [&](bool ok, ProgramPtr, const string issues) {
                REQUIRE_MESSAGE(ok, issues);
                called = true;
                outer.check();
                throw std::runtime_error("explicit callback failure");
            }), std::runtime_error);
        outer.env.g_Program = boundProgram;
    }
    SUBCASE("an escaped compile exception restores both slots") {
        auto throwing = make_smart<ThrowingFileAccess>();
        CHECK_THROWS_AS(fixture.compile(file, cache, throwing, group,
            callback), std::runtime_error);
        called = true;
    }
    outer.check();
    CHECK(called);
}

TEST_CASE("explicit compile_file cache retains restored source locations") {
    ExplicitCacheFixture fixture;
    LocationRegistration registration;
    auto annotation = registration.annotation;
    auto file = fixture.dir + "/root.das";
    auto cache = fixture.dir + "/cache.bin";
    auto makeAccess = [&]() {
        auto access = make_smart<FsFileAccess>();
        static const char source[] = "options gen2\n"
            "require explicit_cache_test\n"
            "[explicit_cache_location, export]\n"
            "def probe() : int { return 7 }\n";
        access->setFileInfo(file, make_unique<TextFileInfo>(
            const_cast<char *>(source), uint32_t(sizeof(source) - 1), false));
        return access;
    };
    {
        ModuleGroup coldGroup;
        CHECK(fixture.compile(file, cache, makeAccess(),
            coldGroup, [](bool ok, ProgramPtr, const string issues) {
                REQUIRE_MESSAGE(ok, issues);
            }) == "none");
    }
    ModuleGroup group;
    ProgramPtr restored;
    CHECK(fixture.compile(file, cache, makeAccess(), group,
        [&](bool ok, ProgramPtr program, const string issues) {
            REQUIRE_MESSAGE(ok, issues);
            restored = program;
        }) == "clean");
    REQUIRE(restored != nullptr);
    auto owner = group.getUserData("module_cache_file_infos");
    REQUIRE(owner != nullptr);
    ProgramPtr second;
    string secondIssues;
    auto secondVerdict = fixture.compile(file, cache, makeAccess(), group,
        [&](bool ok, ProgramPtr program, const string issues) {
            REQUIRE_MESSAGE(ok, issues);
            second = program;
            secondIssues = issues;
        });
    CHECK_MESSAGE(secondVerdict == "fallback", secondIssues);
    ProgramPtr third;
    auto thirdVerdict = fixture.compile(file, cache, makeAccess(), group,
        [&](bool ok, ProgramPtr program, const string issues) {
            REQUIRE_MESSAGE(ok, issues);
            third = program;
            secondIssues = issues;
        });
    CHECK_MESSAGE(thirdVerdict == "clean", secondIssues);
    REQUIRE(second != nullptr);
    CHECK(group.getUserData("module_cache_file_infos") == owner);
    REQUIRE(third != nullptr);
    auto firstLocation = checkLocation(restored, *annotation);
    CHECK(checkLocation(third, *annotation) != firstLocation);
    TextPrinter logs;
    Context context(restored->getContextStackSize());
    REQUIRE(restored->simulate(context, logs));
    auto test = context.findFunction("probe");
    REQUIRE(test != nullptr);
    auto result = context.evalWithCatch(test, nullptr);
    CHECK(context.getException() == nullptr);
    CHECK(cast<int32_t>::to(result) == 7);
}

#else

TEST_CASE("explicit compile_file cache reports unavailable file IO") {
    Context context(65536);
    CodeOfPolicies policies;
    context.setup(0, 0, policies, AnnotationArgumentList());
    TBlock<void,bool,ProgramPtr,const string> block;
    auto unavailable = [&](auto operation) {
        CHECK_FALSE(context.runWithCatch(operation));
        REQUIRE(context.getException() != nullptr);
        CHECK(string(context.getException()).find("DAS_NO_FILEIO")
            != string::npos);
        context.clearException();
    };
    SUBCASE("cache path") {
        unavailable([&]() {
            rtti_builtin_module_cache_path(nullptr, policies,
                &context, nullptr);
        });
    }
    SUBCASE("compilation") {
        unavailable([&]() {
            rtti_builtin_compile_file_cached(nullptr, nullptr, nullptr,
                policies, nullptr, block, &context, nullptr);
        });
    }
}

#endif

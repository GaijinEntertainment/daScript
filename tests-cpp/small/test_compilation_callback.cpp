#include <doctest/doctest.h>

#include "daScript/daScript.h"

#include <algorithm>
#include <mutex>
#include <string>
#include <vector>

using namespace das;

namespace {

struct CallbackEvent {
    string moduleName;
    string fileName;
    string status;
};

static mutex gCallbackEventsMutex;
static vector<CallbackEvent> gCallbackEvents;

static void testCompilationCallback(const string & moduleName, const string & fileName, const string & status) {
    lock_guard<mutex> lock(gCallbackEventsMutex);
    gCallbackEvents.push_back({moduleName, fileName, status});
}

static void clearCallbackEvents() {
    lock_guard<mutex> lock(gCallbackEventsMutex);
    gCallbackEvents.clear();
}

static vector<string> statusesForFile(const string & fileName) {
    lock_guard<mutex> lock(gCallbackEventsMutex);
    vector<string> statuses;
    for (const auto & ev : gCallbackEvents) {
        if (ev.fileName == fileName) {
            statuses.push_back(ev.status);
        }
    }
    return statuses;
}

struct CompilationCallbackScope {
    CompilationCallbackScope() {
        clearCallbackEvents();
        setCompilationCallback(&testCompilationCallback);
    }

    ~CompilationCallbackScope() {
        setCompilationCallback(nullptr);
    }
};

}  // namespace

#define OK_SCRIPT_PATH "/tests-cpp/small/test_compilation_callback_ok.das"
#define FAIL_SCRIPT_PATH "/tests-cpp/small/test_compilation_callback_fail.das"

TEST_CASE("compilation callback reports success and failure phases") {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();

    SUBCASE("successful compile emits start/parse/infer/end for target file") {
        CompilationCallbackScope callbackScope;
        const string scriptPath = getDasRoot() + OK_SCRIPT_PATH;

        auto program = compileDaScript(scriptPath, fAccess, tout, dummyLibGroup);
        REQUIRE(program);
        REQUIRE_FALSE(program->failed());

        auto statuses = statusesForFile(scriptPath);
        REQUIRE_FALSE(statuses.empty());
        CHECK(statuses.front() == "compilation start");
        CHECK(statuses.back() == "compilation end");
        CHECK(std::find(statuses.begin(), statuses.end(), "parse") != statuses.end());
        CHECK(std::find(statuses.begin(), statuses.end(), "infer") != statuses.end());
    }

    SUBCASE("failed compile still emits start and end for target file") {
        CompilationCallbackScope callbackScope;
        const string scriptPath = getDasRoot() + FAIL_SCRIPT_PATH;

        auto program = compileDaScript(scriptPath, fAccess, tout, dummyLibGroup);
        REQUIRE(program);
        REQUIRE(program->failed());

        auto statuses = statusesForFile(scriptPath);
        REQUIRE_FALSE(statuses.empty());
        CHECK(statuses.front() == "compilation start");
        CHECK(statuses.back() == "compilation end");
        CHECK(std::find(statuses.begin(), statuses.end(), "parse") != statuses.end());
    }
}

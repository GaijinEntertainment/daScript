#include <doctest/doctest.h>
#include "daScript/daScript.h"

using namespace das;

namespace {

    const char * g_logged_message = "the message the caller handed to to_log";

    struct TempTrafficAgent {
        string seen_after_traffic;
        TempTrafficAgent() {
            onLogCppDebugAgent("to_out_temp_traffic_probe", [this](Context * ctx, const LineInfo * at, int, const char * text) {
                auto stamp = ctx->allocateTempString("1970-01-01T00:00:00.000Z", 24, at);
                ctx->freeTempString(stamp, at);
                ctx->allocateString("a same-bin string lands in the freed slot", 41, at);
                seen_after_traffic = text;
                return true;
            });
        }
        ~TempTrafficAgent() { uninstallCppDebugAgent("to_out_temp_traffic_probe"); }
    };
}

TEST_CASE("to_out keeps the caller's message whole while an agent's temp strings churn the logging context") {
    TextPrinter tout;
    ModuleGroup libGroup;
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_to_out_agent_temp_strings.das", fAccess, tout, libGroup);
    REQUIRE_FALSE(program->failed());
    Context ctx(program->getContextStackSize());
    REQUIRE(program->simulate(ctx, tout));
    TempTrafficAgent agent;
    auto message = ctx.allocateTempString(g_logged_message, uint32_t(strlen(g_logged_message)), nullptr);
    ctx.freeTempString(message, nullptr);
    ctx.to_out(nullptr, LogLevel::info, message);
    CHECK_EQ(agent.seen_after_traffic, g_logged_message);
}

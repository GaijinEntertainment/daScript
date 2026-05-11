---
slug: how-do-i-suppress-noisy-to-log-log-info-chatter-from-daslang-s-jit-runtime-e-g-llvm-jit-dll-cache-miss-hit-library-made-ok-from
title: How do I suppress noisy `to_log(LOG_INFO, ...)` chatter from daslang's JIT runtime (e.g. "LLVM JIT: DLL cache miss/hit", "Library ... made - ok") from a host .das program?
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

Install a DebugAgent whose `onLog` returns `true` to swallow matching messages. `Context::to_out` in src/runtime/context.cpp routes EVERY log call (including `print` and `to_log`) through `pAgent->onLog(ctx, at, level, text)` on all registered agents; if any agent returns `true` the message is suppressed before the prefix (`[I] ` etc.) and stdout write.

**Minimal companion file (log_filter.das):**

```das
options gen2
require daslib/debugger
require daslib/strings_boost

class LogFilterAgent : DapiDebugAgent {
    def override onLog(context : Context?; at : LineInfo const?; level : int; text : string#) : bool {
        return text |> starts_with("LLVM JIT:") || text |> starts_with("Library ")
    }
}

def private setup_log_filter(ctx : Context) {
    install_new_debug_agent(new LogFilterAgent(), "log_filter")
}

def install_log_filter {
    if (!has_debug_agent_context("log_filter")) {
        fork_debug_agent_context(@@setup_log_filter)
    }
}
```

**Host main.das:**

```das
require ./log_filter.das

[export]
def main {
    install_log_filter()
    // ... rest
}
```

**Gotchas:**

- The JIT noise comes from two places — `to_log(LOG_INFO, "LLVM JIT: …")` calls in `modules/dasLLVM/daslib/llvm_macro.das` and `context->to_out(&li, LogLevel::info, "Library X made - ok\n")` in `src/builtin/module_jit.cpp:862`. Both route through onLog. Filter by text prefix; filtering only by `level == LOG_INFO` is safe (`print` uses `LogLevel::defaultPrint`, not `info`) but less explicit.

- **Order of operations matters.** Messages emitted before `def main` runs cannot be suppressed by the agent — including the first JIT codegen of `main.das` itself plus its dependencies (e.g. `log_filter.das` if you require it). So the first couple of `[I]` lines at startup are unavoidable from das side. Everything emitted by INNER `compile_file` calls (a host running benchmarks against per-file scripts) is silenced.

- The agent class must inherit `DapiDebugAgent` from `daslib/debugger`. Override only the methods you care about; the rest stay abstract no-ops.

- `install_new_debug_agent` requires running inside an agent context, which is why the setup goes through `fork_debug_agent_context(@@setup_fn)`. Don't call `install_new_debug_agent` directly from `def main` — it asserts.

- Gate re-install with `has_debug_agent_context("log_filter")`; the test pattern in `tests/debug_agent/test_on_log.das` uses a module-level `log_agent_ready` bool which doesn't actually work across contexts (each context has its own copy of module globals) but happens to be fine for single-shot test invocations.

- The agent's `onLog` runs in the agent's own forked context. The `text` parameter is `string#` (temp ref) — `starts_with` works on it; if you need to retain the string outside the call use `:=` to clone.

- For thread-local scope use `install_new_thread_local_debug_agent(...)` instead (in `daslib/debugger`), but the global category-based form is right for a benchmark harness.

Reference: `dasProfile/log_filter.das`, `dasProfile/main.das`. Working example also in `tests/debug_agent/test_on_log.das` (don't suppress — it just observes — but same shape).

## Questions
- How do I suppress noisy `to_log(LOG_INFO, ...)` chatter from daslang's JIT runtime (e.g. "LLVM JIT: DLL cache miss/hit", "Library ... made - ok") from a host .das program?

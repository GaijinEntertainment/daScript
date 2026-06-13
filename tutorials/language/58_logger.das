// Tutorial 58: Logger
//
// This tutorial covers:
//   - Structured logging with daslib/logger (one JSON object per line)
//   - Configuring the log file: logger_set_path
//   - Level helpers: logger_info / warning / error / debug
//   - Attaching a structured fields object to a record
//   - Global level filtering: logger_set_min_level
//   - Per-category overrides with dotted-prefix matching
//   - The print/to_log diversion hook for stdio-transport tools
//
// daslib/logger writes "JSON Lines" (ndjson): one self-contained JSON
// object per line, carrying an ISO-8601 UTC timestamp, level, category,
// message, and an optional fields object. It is built for tool back-ends
// (MCP servers, language servers) where stdout is a wire protocol and a
// stray print() would corrupt the stream.
//
// requires:  daslib/logger, daslib/json_boost
// Run: daslang.exe tutorials/language/58_logger.das

options gen2

require daslib/logger
require daslib/json_boost

let LOG_PATH = "logger_tutorial_demo.log"

// Close the current handle and delete the file so each section starts from
// an empty log — the next logger_* call lazily reopens it.
def reset_log() {
    logger_close()
    remove_result(LOG_PATH)
}

def show_log(title : string) {
    print("--- {title} ---\n")
    print("{fread(LOG_PATH)}")
}

// === Basic logging ===
// Point the logger at a file, then call the level helpers. Each call is
// (category, message). Categories are free-form dotted strings.

def basic_logging() {
    print("=== Basic logging ===\n")
    logger_set_path(LOG_PATH)
    reset_log()

    logger_info("app", "starting up")
    logger_warning("app.net", "connection retry")
    logger_error("app.db", "query failed")

    show_log("file contents")
    // Each line is a complete JSON object:
    //   {"ts":"...","level":"info","cat":"app","msg":"starting up"}
}

// === Structured fields ===
// Pass a JsonValue? as the optional 4th argument to attach machine-readable
// context. The json_boost named-tuple form JV((k=v, ...)) is the shortest
// way to build that object.

def structured_fields() {
    print("\n=== Structured fields ===\n")
    reset_log()

    logger_info("app.req", "handled request",
        JV((method = "GET", path = "/users", status = 200, ms = 12)))

    show_log("with fields")
    // {"ts":"...","level":"info","cat":"app.req","msg":"handled request",
    //  "fields":{"method":"GET","path":"/users","status":200,"ms":12}}
}

// === Global level filtering ===
// Records below the minimum level are dropped before formatting. The
// default minimum is LOG_INFO, so logger_debug is silent unless you lower
// it. Raise the bar to LOG_WARNING and info-level records disappear.

def level_filtering() {
    print("\n=== Level filtering ===\n")
    reset_log()

    logger_set_min_level(LOG_WARNING)
    logger_info("app", "info is now below the bar — dropped")
    logger_warning("app", "warning is kept")
    logger_error("app", "error is kept")

    show_log("only warning + error")
    logger_set_min_level(LOG_INFO)   // restore default for later sections
}

// === Per-category overrides ===
// logger_set_category_level lifts (or lowers) the bar for one category and
// everything beneath it in the dotted hierarchy. An override on "app.trace"
// also governs "app.trace.sql", "app.trace.http", etc. Anything outside
// that prefix still uses the global minimum.

def category_levels() {
    print("\n=== Per-category overrides ===\n")
    reset_log()

    logger_set_category_level("app.trace", LOG_DEBUG)
    logger_debug("app.trace.sql", "kept — under the app.trace override")
    logger_debug("app.other", "dropped — global minimum is still INFO")
    logger_info("app.other", "kept — info meets the global minimum")

    show_log("category override in effect")
    logger_clear_category_levels()
}

// === The diversion hook (advanced) ===
// logger_install_hook plugs a global debug agent into the runtime so that
// print() and to_log() — from this context or any future thread — are
// redirected into the log file instead of stdout/stderr. This is the whole
// point of the module for stdio-transport servers.
//
// print() arrives at the hook as a DEBUG-level record, so we lower the
// minimum to LOG_DEBUG first — otherwise the default LOG_INFO bar would
// drop it. to_log carries whatever level you pass.
//
// Note the consequence: once the hook is installed, print() no longer
// reaches the terminal. To DISPLAY the captured lines we write straight to
// the stdout FILE with fwrite, which bypasses the hook — and in doing so
// proves the print() above really was diverted.

def diversion_hook() {
    print("\n=== Diversion hook ===\n")
    reset_log()
    logger_set_min_level(LOG_DEBUG)

    logger_install_hook()
    print("this print is now diverted into the log file\n")
    to_log(LOG_WARNING, "this to_log is diverted too\n")
    logger_flush()

    let captured = fread(LOG_PATH)
    fwrite(fstdout(), "--- captured (shown via fwrite, which is not hooked) ---\n")
    fwrite(fstdout(), captured)
}

[export]
def main() {
    basic_logging()
    structured_fields()
    level_filtering()
    category_levels()
    diversion_hook()

    // Clean up the demo log file.
    logger_close()
    remove_result(LOG_PATH)
    fwrite(fstdout(), "\ndone\n")
}

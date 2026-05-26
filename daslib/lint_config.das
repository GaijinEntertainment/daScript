options gen2
options indenting = 4
options no_unused_function_arguments = false

module lint_config shared private

//! Repo-level lint enable/disable config.
//!
//! Loads ``{get_das_root()}/.lint_config`` and folds its directives into a
//! ``disabled_codes`` set consumed by the three ``[lint_macro]`` pass-macros
//! (``daslib/lint``, ``daslib/perf_lint``, ``daslib/style_lint``).
//!
//! Format (one directive per line, blank lines and ``#``-comments ignored,
//! malformed lines silently skipped):
//!
//!   STYLE005 on    # re-enable a default-disabled rule
//!   PERF007  off   # disable a default-enabled rule
//!
//! If the file is missing or unreadable the call is a no-op — built-in
//! defaults stand.

require fio
require strings
require daslib/strings_boost

//! Loads ``{get_das_root()}/.lint_config`` into ``disabled_codes``.
//! No-op when the file is missing or unreadable.
def public load_lint_config(var disabled_codes : table<string>) : void {
    load_lint_config_from_path("{get_das_root()}/.lint_config", disabled_codes)
}

//! Seeds ``disabled_codes`` with the canonical default-off rule set
//! (currently just STYLE005). Call before ``load_lint_config`` so that
//! a ``STYLE005 on`` directive in ``.lint_config`` can re-enable it.
//! Single source of truth used by the three ``[lint_macro]`` paths,
//! ``utils/lint/main.das``, and the MCP ``lint`` subtool.
def public seed_default_disabled(var disabled_codes : table<string>) : void {
    if (!key_exists(disabled_codes, "STYLE005")) {
        disabled_codes |> insert("STYLE005")
    }
}

//! Parser entry point. Reads ``path`` and folds directives into
//! ``disabled_codes``. Public so tests can exercise the parser hermetically
//! without writing into the repo root.
def public load_lint_config_from_path(path : string; var disabled_codes : table<string>) : void {
    let body = fread(path)
    if (empty(body)) return
    for (raw in split(body, "\n")) {
        let hash_pos = find(raw, "#")
        let no_comment = hash_pos < 0 ? raw : slice(raw, 0, hash_pos)
        let line = strip(no_comment)
        if (empty(line)) continue
        let toks <- split_by_chars(line, " \t")
        if (length(toks) != 2) continue
        let code = toks[0]
        let action = toks[1]
        if (action == "off") {
            if (!key_exists(disabled_codes, code)) {
                disabled_codes |> insert(code)
            }
        } elif (action == "on" && key_exists(disabled_codes, code)) {
            disabled_codes |> erase(code)
        }
    }
}

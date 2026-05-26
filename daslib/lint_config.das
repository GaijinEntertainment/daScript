options gen2
options indenting = 4
options no_unused_function_arguments = false

module lint_config shared private

//! Repo-level lint enable/disable config.
//!
//! Loads ``{get_das_root()}/.lint_config`` (a TOML file) and folds its
//! ``[rules]`` table into a ``disabled_codes`` set consumed by the three
//! ``[lint_macro]`` pass-macros (``daslib/lint``, ``daslib/perf_lint``,
//! ``daslib/style_lint``), by the standalone runner ``utils/lint/main.das``,
//! and by the MCP ``lint`` subtool.
//!
//! Format (TOML 1.0). One ``[rules]`` table; each entry sets a rule to
//! ``true`` (on) or ``false`` (off):
//!
//!   [rules]
//!   STYLE005 = true     # re-enable a default-off rule
//!   PERF007  = false    # disable a default-on rule
//!
//! Anything that is not a boolean entry under ``[rules]`` is ignored.
//! If the file is missing, unreadable, or fails to parse the call is a
//! no-op — built-in defaults stand.

require fio
require daslib/json
require daslib/toml

//! Loads ``{get_das_root()}/.lint_config`` into ``disabled_codes``.
//! No-op when the file is missing or unreadable.
def public load_lint_config(var disabled_codes : table<string>) : void {
    load_lint_config_from_path("{get_das_root()}/.lint_config", disabled_codes)
}

//! Parser entry point. Reads ``path`` (TOML) and folds the ``[rules]``
//! table into ``disabled_codes``. Public so tests can exercise the parser
//! hermetically without writing into the repo root.
def public load_lint_config_from_path(path : string; var disabled_codes : table<string>) : void {
    let body = fread(path)
    if (empty(body)) return
    var error : string
    var root : JsonValue? = read_toml(body, error)
    // bad TOML or no [rules] table → defaults stand
    if (root == null || !(root.value is _object)) return
    var root_obj & = unsafe(root.value as _object)   // nolint:LINT003 — table [] requires mutable binding
    if (!key_exists(root_obj, "rules")) return
    var rules_obj : JsonValue? = root_obj["rules"]
    if (rules_obj == null || !(rules_obj.value is _object)) return
    var rules & = unsafe(rules_obj.value as _object)   // nolint:LINT003 — iteration requires mutable view
    for (code, val_jv in keys(rules), values(rules)) {
        if (val_jv == null || !(val_jv.value is _bool)) continue
        let enabled = val_jv.value as _bool
        if (enabled) {
            // CODE = true → on → remove from default-disabled
            if (key_exists(disabled_codes, code)) {
                disabled_codes |> erase(code)
            }
        } else {
            // CODE = false → off → add to disabled
            if (!key_exists(disabled_codes, code)) {
                disabled_codes |> insert(code)
            }
        }
    }
}

//! Seeds ``disabled_codes`` with the canonical default-off rule set
//! (currently just STYLE005). Call before ``load_lint_config`` so that
//! a ``STYLE005 = true`` directive in ``.lint_config`` can re-enable it.
//! Single source of truth used by the three ``[lint_macro]`` paths,
//! ``utils/lint/main.das``, and the MCP ``lint`` subtool.
def public seed_default_disabled(var disabled_codes : table<string>) : void {
    if (!key_exists(disabled_codes, "STYLE005")) {
        disabled_codes |> insert("STYLE005")
    }
}

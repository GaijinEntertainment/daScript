options gen2

require dastest/testing_boost public
require daslib/fio
require strings
require daslib/strings_boost

// Drift detector for the dasllama facade (the documented public API). Three invariants:
//  1. every facade `def` carries a `//!` docstring INSIDE its body — the doc extractor
//     (daslib/rst_comment) attaches only body-leading `//!`; a `//!` above the def is silently
//     invisible, so that pattern is rejected outright;
//  2. facade stubs and the engine's trailing-underscore spellings stay 1:1 — adding `foo_` to
//     the engine without a documented `foo` stub (or vice versa) fails here;
//  3. the examples stay facade-only: their one dasllama require is dasllama/dasllama.

let FACADE = "modules/dasLLAMA/dasllama/dasllama.das"
let ENGINE_FILES = [
    "modules/dasLLAMA/dasllama/dasllama_common.das",
    "modules/dasLLAMA/dasllama/dasllama_chat.das"
]
let EXAMPLES = ["examples/dasLLAMA/run.das", "examples/dasLLAMA/chat.das"]

def private read_lines(path : string) : array<string> {
    let text = fread("{get_das_root()}/{path}")
    if (empty(text)) {
        panic("test_facade_docs: can't read {path}")
    }
    // Windows checkouts are CRLF (no *.das eol rule in .gitattributes); the parsing here is line-exact
    return <- split(replace(text, "\r\n", "\n"), "\n")
}

// "def name(" / "def name_(" at column 0 => "name"/"name_"; "" for anything else (incl. private)
def private toplevel_def_name(line : string) : string {
    if (!starts_with(line, "def ")) {
        return ""
    }
    let rest = slice(line, 4, length(line))
    let paren = find(rest, "(")
    if (paren <= 0) {
        return ""
    }
    let name = slice(rest, 0, paren)
    return find(name, " ") >= 0 ? "" : name   // "def private x(", "def operator ..." etc. drop out
}

[test]
def test_facade_docstrings_inside_body(t : T?) {
    t |> run("every facade def opens with a body-leading //! docstring") @(t : T?) {
        let lines <- read_lines(FACADE)
        let n = length(lines)
        var checked = 0
        for (i in range(n)) {
            let name = toplevel_def_name(lines[i])
            continue if (empty(name))
            // the body opens at the first line of this def ending in "{" (signatures may wrap)
            var open = i
            while (open < n && !ends_with(lines[open], "\{")) {
                open++
            }
            let has_doc = open + 1 < n && starts_with(strip(lines[open + 1]), "//!")
            t |> success(has_doc, "facade `{name}` has a //! docstring as the first body line")
            checked++
        }
        t |> success(checked > 0, "facade defs found ({checked})")
    }
    t |> run("no inert above-def //! blocks in the facade") @(t : T?) {
        let lines <- read_lines(FACADE)
        for (i in range(length(lines) - 1)) {
            let inert = starts_with(strip(lines[i]), "//!") && starts_with(lines[i + 1], "def ")
            t |> success(!inert, "line {i + 1}: //! directly above a def is invisible to the doc extractor — move it inside the body")
        }
    }
}

[test]
def test_facade_engine_names_one_to_one(t : T?) {
    t |> run("facade stubs <-> engine underscore spellings are 1:1") @(t : T?) {
        var facade : table<string>
        for (line in read_lines(FACADE)) {
            let name = toplevel_def_name(line)
            if (!empty(name)) {
                facade |> insert(name)
            }
        }
        var engine : table<string>
        for (path in ENGINE_FILES) {
            for (line in read_lines(path)) {
                let name = toplevel_def_name(line)
                if (!empty(name) && ends_with(name, "_")) {
                    engine |> insert(slice(name, 0, length(name) - 1))
                }
            }
        }
        for (name in keys(facade)) {
            t |> success(key_exists(engine, name), "facade `{name}` wraps an engine `{name}_`")
        }
        for (name in keys(engine)) {
            t |> success(key_exists(facade, name), "engine `{name}_` has a documented facade `{name}`")
        }
    }
}

[test]
def test_examples_are_facade_only(t : T?) {
    t |> run("run.das / chat.das require only the facade from dasLLAMA") @(t : T?) {
        for (path in EXAMPLES) {
            var dasllama_requires : array<string>
            for (line in read_lines(path)) {
                if (starts_with(line, "require dasllama/")) {
                    let rest = slice(line, length("require "), length(line))
                    let sp = find(rest, " ")
                    dasllama_requires |> push(sp < 0 ? rest : slice(rest, 0, sp))
                }
            }
            t |> equal(length(dasllama_requires), 1)
            t |> equal(dasllama_requires[0], "dasllama/dasllama")
        }
    }
}

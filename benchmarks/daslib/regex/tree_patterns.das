options gen2
options persistent_heap

require daslib/regex
require daslib/regex_boost
require dastest/testing_boost
require strings
require daslib/strings_boost

var private g_sink = 0

let private BLOCKS = 40
let private TINY_REPEATS = 1000

def private source_lines : array<string> {
    var lines : array<string>
    lines |> reserve(BLOCKS * 6)
    for (i in range(BLOCKS)) {
        lines |> push("[arch(at=\"daslib/ARCHITECTURE.md#section-{i}\")]")
        lines |> push("def private helper_{i}(var arr : array<int>; key : string) : int \{")
        lines |> push("    let total = length(arr) + {i}  //! sums the entries of block {i}")
        lines |> push("#define DAS_FLAG_{i} 0x{i}F0")
        lines |> push("    // run `bin/tool_{i}.exe` before"
            + " \"LINT0{i % 10}{i % 7}: the rule fires\"")
        lines |> push("struct private Shape{i} \{")
    }
    return <- lines
}

def private function_names : array<string> {
    return <- ["sign", "acos", "smoothstep", "length", "push", "visit_expr", "try_to_int",
               "emplace", "finalize", "degrees", "strudel_play", "tray_icon",
               "each", "u8x4_gather_store", "is_nan", "cbrt", "hash", "sort",
               "find_index", "remainder"]
}

def private run_foreach(b : B?; name : string; var re : Regex; text : string) {
    b |> run(name, 1) {
        regex_foreach(re, text) $(at) {
            g_sink += at.x
            return true
        }
    }
}

[benchmark]
def regex_over_source_text(b : B?) {
    let lines <- source_lines()
    let text = join(lines, "\n")
    var arch_cite <- %regex~arch\s*\(\s*at\s*=\s*"([^"]*)"%%
    b |> run_foreach("foreach/arch_citation", arch_cite, text)
    var rule_id <- %regex~"((LINT|PERF|STYLE)[0-9][0-9][0-9]):%%
    b |> run_foreach("foreach/rule_id_alternation", rule_id, text)
    var lazy_quote <- %regex~".*?"%%
    b |> run_foreach("foreach/lazy_quoted_string", lazy_quote, text)
    var bin_exe <- %regex~`([A-Za-z0-9_./-]+)\.exe`%%
    b |> run("search/backticked_exe", 1) {
        var from = 0
        while (true) {
            let m = regex_search(bin_exe, text, from)
            if (m.x < 0) break
            g_sink += m.y
            from = m.y
        }
    }
    var spaces <- %regex~\s+%%
    b |> run("replace/collapse_whitespace", 1) {
        g_sink += length(regex_replace(spaces, text, " "))
    }
}

[benchmark]
def regex_per_source_line(b : B?) {
    let lines <- source_lines()
    let nlines = length(lines)
    var private_decl <- %regex~^\s*(?:def|struct|enum|variant|class)\s+private\b%%
    b |> run("match_lines/anchored_keyword_alternation", nlines) {
        for (ln in lines) {
            g_sink += regex_match(private_decl, ln)
        }
    }
    var trailing_doc <- %regex~\s*//!<?[^\n]*$%%
    b |> run("search_lines/trailing_doc_comment", nlines) {
        for (ln in lines) {
            g_sink += regex_search(trailing_doc, ln).x
        }
    }
    var define_hex <- %regex~#define\s+(\w+)\s+(0x[0-9A-Za-z]+)%%
    b |> run("match_lines/define_hex", nlines) {
        for (ln in lines) {
            g_sink += regex_match(define_hex, ln)
        }
    }
}

[benchmark]
def regex_per_name(b : B?) {
    let names <- function_names()
    let math_alternatives = ("sign|abs|acos|asin|safe_acos|safe_asin|atan|atan2|cos|sin|tan"
        + "|sinh|cosh|tanh|asinh|acosh|atanh|exp|log|exp2|expm1|log2|log10|log1p|pow|sqrt|cbrt"
        + "|hypot|fmod|remainder|trunc|rcp|ceil|floor|abs|saturate|sincos|is_finite"
        + "|is_nan|radians|degrees|step|smoothstep")
    var math_names <- regex_compile("({math_alternatives})$")
    b |> run("match_names/wide_alternation_anchored", length(names)) {
        for (n in names) {
            g_sink += regex_match(math_names, n)
        }
    }
    var visit_prefix <- %regex~visit.*%%
    b |> run("match_names/prefix_dot_star", length(names) * TINY_REPEATS) {
        for (_ in range(TINY_REPEATS)) {
            for (n in names) {
                g_sink += regex_match(visit_prefix, n)
            }
        }
    }
    var kebab <- %regex~^[a-z0-9]+(-[a-z0-9]+)*$%%
    b |> run("match/kebab_group_star", 4) {
        g_sink += regex_match(kebab, "dasllama-bench-house-replay")
        g_sink += regex_match(kebab, "tree-patterns")
        g_sink += regex_match(kebab, "Not_Kebab")
        g_sink += regex_match(kebab, "a-b-c-d-e-f-g-h-i-j-k-l")
    }
    var signature <- %regex~(.+)\s*\((.+)\)%%
    b |> run("match/backtracking_dot_plus", 2) {
        g_sink += regex_match(signature,
            "das::TypeDecl * das::makeTypeDecl(das::Context * ctx, int baseType, bool isConst)")
        g_sink += regex_match(signature, "void (*)(const char *, int, float)")
    }
}

[benchmark]
def regex_backtracking_shapes(b : B?) {
    var shared_prefix <- %regex~x(aba|ab|b)*c%%
    let shared_input = repeat("abab b ", 200) + "xabac" + repeat(" ba", 100)
    b |> run("search/shared_prefix_alternation_star", 1) {
        g_sink += regex_search(shared_prefix, shared_input).x
    }
    var nested_empty <- %regex~(a*)*b%%
    b |> run("match/nested_star_tiny", TINY_REPEATS) {
        for (_ in range(TINY_REPEATS)) {
            g_sink += regex_match(nested_empty, "aab")
        }
    }
}

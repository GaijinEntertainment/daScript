options gen2

module arch_common

require common public
require outline public
require ../../common/arch_citations.das public
require daslib/fio
require strings
require daslib/strings_boost

//! What the `arch_of` and `arch_sites` tools add to the shared `[arch]` scanner: the text under a
//! cited anchor, the symbol a citation annotates, and the .das tree the census walks.
//!
//! Resolution itself - the citation regex, the failure strings, the anchor rule - is
//! `utils/common/arch_citations.das`, the same code LINT026 runs, so a tool and the rule never
//! disagree about whether a citation resolves.

// The name a declaration line introduces, "" when the line declares nothing. Keyword plus the
// modifiers `outline` already knows how to skip, which is all a navigation aid needs.
def private declared_name(t : string) : string {
    return extract_name_from_text(t, "function") if (t |> starts_with("def "))
    return extract_name_from_text(t, "struct") if (t |> starts_with("struct ") || t |> starts_with("class "))
    return ""
}

// The symbol a citation on `line` (1-based) annotates, "" when the source does not say. The
// marker sits ABOVE its declaration, so the answer is the first declaration below it, past the
// rest of the annotation block; a citation found anywhere else - a comment, a string literal -
// falls back to the declaration above it, which is the enclosing one.
def annotated_symbol(lines : array<string>; line : int) : string {
    var i = line - 1
    while (i < length(lines)) {
        let t = lines[i] |> strip
        let name = declared_name(t)
        return name if (!empty(name))
        break if (!(empty(t) || t |> starts_with("//") || t |> starts_with("[") || t |> ends_with("]") || t |> ends_with(",")))
        i ++
    }
    var j = line - 2
    while (j >= 0) {
        let name = declared_name(lines[j] |> strip)
        return name if (!empty(name))
        j --
    }
    return ""
}

// Every .das under `path`, pruned the way LINT026's reverse walk prunes: `_`-prefixed and
// dot-prefixed names are fixtures and tooling, and `builtin.das` is not source anyone cites.
def scan_das_tree(path : string; var out : array<string>) {
    fio::dir(path) $(name) {
        return if (arch_walk_skips(name))
        let full = "{path}/{name}"
        let fst = stat(full)
        return if (!fst.is_valid)
        if (fst.is_dir) {
            scan_das_tree(full, out)
        } elif (fst.is_reg && name |> ends_with(".das")) {
            out |> push(full)
        }
    }
}

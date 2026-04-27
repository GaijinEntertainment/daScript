options gen2

require strings
require daslib/strings_boost
require canonical


// PatternHit is the verdict of `classify`. A non-empty `name` flags
// the function as matching a known "boring" shape (dispatcher, repeated
// init, etc.) — the caller drops it from clustering unless the user
// explicitly asked to keep this pattern via `--keep <name>`.
struct public PatternHit {
    name : string
    note : string
}


// Single entry point for "is this function carrying real signal?"
// Returns `name == ""` when the function is interesting; non-empty
// otherwise. Patterns are tried in priority order — the first match
// wins, which means more specific shapes should be checked before
// more general ones.
def public classify(canonical : string) : PatternHit {
    var hit = PatternHit()
    if (try_dispatch(canonical, hit)) {
        return hit
    }
    return hit
}


// Dispatch shape: the function body is N >= 2 byte-identical top-level
// statement chunks. Catches dastest's `t |> run("X") @(t) { … }` lists
// and any other "uniform call list" outer function. Lambda bodies are
// collapsed to a single `ADDR` token upstream by the canonicalizer, so
// two `run` statements look identical regardless of what the lambdas
// actually do — meaning the outer function carries zero unique
// structure beyond "this many calls".
def try_dispatch(canonical : string; var hit : PatternHit) : bool {
    let stmts <- split_top_level_stmts(canonical)
    if (length(stmts) < 2) {
        return false
    }
    for (i in range(1, length(stmts))) {
        if (stmts[i] != stmts[0]) {
            return false
        }
    }
    hit.name := "dispatch"
    hit.note := "{length(stmts)}x repeated"
    return true
}


// Split a canonical token stream into its top-level statement chunks.
// "Top-level" means within the outer `BODY BLK … ENDBLK` block, NOT
// nested inside any inner BLK/ENDBLK pair (loop bodies, if/then/else
// bodies, lambda bodies via MK_BLK, with/assume blocks).
//
// Tracks BLK depth on a flat token walk. `STMT` and `FIN_STMT` are
// both treated as boundary markers so a function body that ends in
// a final-expression block doesn't merge its last two statements.
//
// Returns an empty array for forward-declarations and other
// body-less canonicals (no `BODY BLK` pair found).
def public split_top_level_stmts(canonical : string) : array<string> {
    var out : array<string>
    let toks <- tokenize_canonical(canonical)
    let n = length(toks)
    var i = 0
    while (i + 1 < n && !(toks[i] == "BODY" && toks[i + 1] == "BLK")) {
        i++
    }
    if (i + 1 >= n) {
        return <- out
    }
    i += 2
    var depth = 0
    var cur_start = -1
    while (i < n) {
        let tok = toks[i]
        if (depth == 0 && (tok == "STMT" || tok == "FIN_STMT")) {
            if (cur_start >= 0) {
                out |> push(join_tokens(toks, cur_start, i))
            }
            cur_start = i
        } elif (tok == "BLK") {
            depth++
        } elif (tok == "ENDBLK") {
            if (depth == 0) {
                if (cur_start >= 0) {
                    out |> push(join_tokens(toks, cur_start, i))
                }
                return <- out
            }
            depth--
        }
        i++
    }
    return <- out
}


def join_tokens(toks : array<string>; lo : int; hi : int) : string {
    var sb : array<string>
    sb |> reserve(hi - lo)
    for (k in range(lo, hi)) {
        sb |> push(toks[k])
    }
    return sb |> join(" ")
}

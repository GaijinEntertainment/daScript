options gen2
options _comment_hygiene = true

module dasllama_pretok shared public

require dasllama/dasllama_unicode

// ===== BPE pre-tokenizer (port of llama.cpp unicode_regex_split_custom_llama3 / _qwen2 / _qwen35) =====
// llama3 chunks digits \p{N}{1,3}, qwen2 emits one digit per token — `group_digits` selects between
// them; qwen35 = qwen2 digits plus marks absorbed into letter runs ([\p{L}\p{M}]+).

def private tolower_ascii(c : int) : int {
    return (c >= 'A' && c <= 'Z') ? c + 32 : c
}

def private uL(cpts : array<int>; pos : int) : bool {
    return pos >= 0 && pos < length(cpts) && cpt_is_letter(cpts[pos])
}
// letter-or-mark: the qwen35 letter class [\p{L}\p{M}] when marks, plain \p{L} otherwise
def private is_lm(cpt : int; marks : bool) : bool {
    return cpt_is_letter(cpt) || (marks && cpt_is_mark(cpt))
}
def private uLM(cpts : array<int>; pos : int; marks : bool) : bool {
    return pos >= 0 && pos < length(cpts) && is_lm(cpts[pos], marks)
}
def private uN(cpts : array<int>; pos : int) : bool {
    return pos >= 0 && pos < length(cpts) && cpt_is_number(cpts[pos])
}
def private uW(cpts : array<int>; pos : int) : bool {
    return pos >= 0 && pos < length(cpts) && cpt_is_whitespace(cpts[pos])
}
def private uC(cpts : array<int>; pos : int) : int {
    return (pos >= 0 && pos < length(cpts)) ? cpts[pos] : -1
}

def pretok_split(cpts : array<int>; group_digits : bool; marks : bool = false) : array<int> {   // nolint:STYLE037,STYLE038 - one arm per alternation of the llama3 pre-tokenizer regex; splitting hides the regex shape
    var offs : array<int>
    let n = length(cpts)
    var prev_end = 0
    var pos = 0
    while (pos < n) {
        let cpt = cpts[pos]

        // (?i:'s|'t|'re|'ve|'m|'ll|'d)
        if (cpt == 39 && pos + 1 < n) {
            let c1 = tolower_ascii(cpts[pos + 1])
            if (c1 == 's' || c1 == 't' || c1 == 'm' || c1 == 'd') {
                pos += 2
                offs |> push(pos - prev_end); prev_end = pos
                continue
            }
            if (pos + 2 < n) {
                let c2 = tolower_ascii(cpts[pos + 2])
                if ((c1 == 'r' && c2 == 'e') || (c1 == 'v' && c2 == 'e') || (c1 == 'l' && c2 == 'l')) {
                    pos += 3
                    offs |> push(pos - prev_end); prev_end = pos
                    continue
                }
            }
        }

        // [^\r\n\p{L}\p{N}]?\p{L}+ (marks widens run class to [\p{L}\p{M}]+)
        if (!(cpt == 13 || cpt == 10 || cpt_is_number(cpt))) {
            if (is_lm(cpt, marks) || uLM(cpts, pos + 1, marks)) {
                pos++
                while (uLM(cpts, pos, marks)) {
                    pos++
                }
                offs |> push(pos - prev_end); prev_end = pos
                continue
            }
        }

        // \p{N}{1,3} (llama3, chunks of up to three) | \p{N} (qwen2, one digit per token)
        if (cpt_is_number(cpt)) {
            if (group_digits) {
                var ini = pos
                while (uN(cpts, pos)) {
                    pos++
                    if (pos - ini >= 3) {
                        offs |> push(pos - prev_end); prev_end = pos
                        ini = pos
                    }
                }
            } else {
                pos++
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // <space>?[^\s\p{L}\p{N}]+[\r\n]* (marks also excludes \p{M} from the run class)
        let probe = (cpt == 32) ? pos + 1 : pos
        if (!(uW(cpts, probe) || uLM(cpts, probe, marks) || uN(cpts, probe))) {
            if (cpt == 32) {
                pos++
            }
            while (!(uW(cpts, pos) || uLM(cpts, pos, marks) || uN(cpts, pos)) && pos < n) {
                pos++
            }
            var c2 = uC(cpts, pos)
            while (c2 == 13 || c2 == 10) {
                pos++
                c2 = uC(cpts, pos)
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // remaining whitespace rules: \s*[\r\n]+ | \s+(?!\S) | \s+
        var num_ws = 0
        var last_rn = 0
        while (uW(cpts, pos + num_ws)) {
            let c2 = uC(cpts, pos + num_ws)
            if (c2 == 13 || c2 == 10) {
                last_rn = pos + num_ws + 1
            }
            num_ws++
        }
        if (last_rn > 0) {
            pos = last_rn
        } elif (num_ws > 1 && uC(cpts, pos + num_ws) != -1) {
            pos += num_ws - 1
        } elif (num_ws > 0) {
            pos += num_ws
        } else {
            pos++  // no match — consume one codepoint
        }
        offs |> push(pos - prev_end); prev_end = pos
    }
    return <- offs
}

// gpt-2 pre-split (llama.cpp LLAMA_VOCAB_PRE_TYPE_GPT2). Differs from llama3: lowercase-only
// contractions, the optional run prefix is exactly one space (not any non-letter), digit runs
// are UNBOUNDED and space-attached, and no newline special-casing on punct/whitespace arms.
def pretok_split_gpt2(cpts : array<int>) : array<int> {   // nolint:STYLE037,STYLE038 - one arm per alternation of the gpt2 pre-tokenizer regex; splitting hides the regex shape
    var offs : array<int>
    let n = length(cpts)
    var prev_end = 0
    var pos = 0
    while (pos < n) {
        let cpt = cpts[pos]

        // 's|'t|'re|'ve|'m|'ll|'d (lowercase only — gpt2's alternation carries no (?i:))
        if (cpt == 39 && pos + 1 < n) {
            let c1 = cpts[pos + 1]
            if (c1 == 's' || c1 == 't' || c1 == 'm' || c1 == 'd') {
                pos += 2
                offs |> push(pos - prev_end); prev_end = pos
                continue
            }
            if (pos + 2 < n) {
                let c2 = cpts[pos + 2]
                if ((c1 == 'r' && c2 == 'e') || (c1 == 'v' && c2 == 'e') || (c1 == 'l' && c2 == 'l')) {
                    pos += 3
                    offs |> push(pos - prev_end); prev_end = pos
                    continue
                }
            }
        }

        // <space>?\p{L}+
        let probe = (cpt == 32) ? pos + 1 : pos
        if (uL(cpts, probe)) {
            pos = probe + 1
            while (uL(cpts, pos)) {
                pos++
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // <space>?\p{N}+ (unbounded, space attached — where gpt2 diverges observably from llama3)
        if (uN(cpts, probe)) {
            pos = probe + 1
            while (uN(cpts, pos)) {
                pos++
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // <space>?[^\s\p{L}\p{N}]+
        if (probe < n && !(uW(cpts, probe) || uL(cpts, probe) || uN(cpts, probe))) {
            pos = probe + 1
            while (pos < n && !(uW(cpts, pos) || uL(cpts, pos) || uN(cpts, pos))) {
                pos++
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // \s+(?!\S) | \s+
        var num_ws = 0
        while (uW(cpts, pos + num_ws)) {
            num_ws++
        }
        if (num_ws > 1 && uC(cpts, pos + num_ws) != -1) {
            pos += num_ws - 1
        } elif (num_ws > 0) {
            pos += num_ws
        } else {
            pos++  // no match — consume one codepoint
        }
        offs |> push(pos - prev_end); prev_end = pos
    }
    return <- offs
}

// llama.cpp GPT4O letter-case classes: "uppercase" = \p{L} minus ASCII a-z, "lowercase" = \p{L}
// minus ASCII A-Z (its regex spells (?=[\p{L}])[^a-z] / (?=[\p{L}])[^A-Z]), so non-cased scripts
// (CJK, ...) land in BOTH classes. We match llama.cpp — the oracle — not tiktoken's true \p{Lu}/\p{Ll}.
def private uUp(cpts : array<int>; pos : int) : bool {
    if (pos < 0 || pos >= length(cpts)) {
        return false
    }
    let c = cpts[pos]
    return cpt_is_letter(c) && !(c >= 'a' && c <= 'z')
}
def private uLo(cpts : array<int>; pos : int) : bool {
    if (pos < 0 || pos >= length(cpts)) {
        return false
    }
    let c = cpts[pos]
    return cpt_is_letter(c) && !(c >= 'A' && c <= 'Z')
}

// gpt-4o / o200k pre-split (llama.cpp LLAMA_VOCAB_PRE_TYPE_GPT4O — gpt-oss). Differs from llama3:
// contractions suffix letter runs (no standalone arm), letter runs are case-structured
// (upper* lower+ | upper+ lower*), and punctuation runs may also trail '/'.
def pretok_split_gpt4o(cpts : array<int>) : array<int> {   // nolint:STYLE037,STYLE038 - one arm per alternation of the o200k pre-tokenizer regex; splitting hides the regex shape
    var offs : array<int>
    let n = length(cpts)
    var prev_end = 0
    var pos = 0
    while (pos < n) {
        let cpt = cpts[pos]

        // [^\r\n\p{L}\p{N}]?(upper* lower+ | upper+ lower*) then optional contraction suffix
        if (!(cpt == 13 || cpt == 10 || cpt_is_number(cpt))) {
            if (cpt_is_letter(cpt) || uL(cpts, pos + 1)) {
                if (!cpt_is_letter(cpt)) {
                    pos++   // the optional one-char non-letter prefix
                }
                while (uUp(cpts, pos)) {
                    pos++
                }
                while (uLo(cpts, pos)) {
                    pos++
                }
                if (uC(cpts, pos) == 39 && pos + 1 < n) {   // contraction suffix, case-insensitive
                    let c1 = tolower_ascii(cpts[pos + 1])
                    if (c1 == 's' || c1 == 't' || c1 == 'm' || c1 == 'd') {
                        pos += 2
                    } elif (pos + 2 < n) {
                        let c2 = tolower_ascii(cpts[pos + 2])
                        if ((c1 == 'r' && c2 == 'e') || (c1 == 'v' && c2 == 'e') || (c1 == 'l' && c2 == 'l')) {
                            pos += 3
                        }
                    }
                }
                offs |> push(pos - prev_end); prev_end = pos
                continue
            }
        }

        // \p{N}{1,3} (chunks of up to three, same as llama3)
        if (cpt_is_number(cpt)) {
            var ini = pos
            while (uN(cpts, pos)) {
                pos++
                if (pos - ini >= 3) {
                    offs |> push(pos - prev_end); prev_end = pos
                    ini = pos
                }
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // <space>?[^\s\p{L}\p{N}]+[\r\n/]*  (the '/' in the trailing run is gpt-4o's addition)
        let probe = (cpt == 32) ? pos + 1 : pos
        if (!(uW(cpts, probe) || uL(cpts, probe) || uN(cpts, probe))) {
            if (cpt == 32) {
                pos++
            }
            while (!(uW(cpts, pos) || uL(cpts, pos) || uN(cpts, pos)) && pos < n) {
                pos++
            }
            var c2 = uC(cpts, pos)
            while (c2 == 13 || c2 == 10 || c2 == 47) {
                pos++
                c2 = uC(cpts, pos)
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // remaining whitespace rules: \s*[\r\n]+ | \s+(?!\S) | \s+ (same as llama3)
        var num_ws = 0
        var last_rn = 0
        while (uW(cpts, pos + num_ws)) {
            let c2 = uC(cpts, pos + num_ws)
            if (c2 == 13 || c2 == 10) {
                last_rn = pos + num_ws + 1
            }
            num_ws++
        }
        if (last_rn > 0) {
            pos = last_rn
        } elif (num_ws > 1 && uC(cpts, pos + num_ws) != -1) {
            pos += num_ws - 1
        } elif (num_ws > 0) {
            pos += num_ws
        } else {
            pos++  // no match — consume one codepoint
        }
        offs |> push(pos - prev_end); prev_end = pos
    }
    return <- offs
}

// tekken pre-split (Mistral/Voxtral). llama.cpp has no custom splitter — it runs generic std::wregex
// TRUE ECMAScript alternation with greedy backtracking (up*lo+ tried first), so "你A" splits "你"|"A"
// unlike gpt-4o's merged-run reduction. No contraction arm; single-digit \p{N}; '/' in punct tail.
def pretok_split_tekken(cpts : array<int>) : array<int> {   // nolint:STYLE037,STYLE038 - one arm per alternation of the tekken pre-tokenizer regex; splitting hides the regex shape
    var offs : array<int>
    let n = length(cpts)
    var prev_end = 0
    var pos = 0
    while (pos < n) {
        let cpt = cpts[pos]

        // [^\r\n\p{L}\p{N}]?(up* lo+ | up+ lo*)
        if (!(cpt == 13 || cpt == 10 || cpt_is_number(cpt))) {
            if (cpt_is_letter(cpt) || uL(cpts, pos + 1)) {
                var q = pos
                if (!cpt_is_letter(cpt)) {
                    q++   // the optional one-char non-letter prefix
                }
                var up = 0
                while (uUp(cpts, q + up)) {
                    up++
                }
                // alt1 up* lo+: greedy up-run backtracks until a lo char can start lo+
                var split = up
                while (split >= 0 && !uLo(cpts, q + split)) {
                    split--
                }
                var e = q
                if (split >= 0) {
                    e = q + split
                    while (uLo(cpts, e)) {
                        e++
                    }
                } else {   // alt2 up+ lo*: up >= 1 guaranteed (first letter failed uLo => it's up)
                    e = q + up
                    while (uLo(cpts, e)) {
                        e++
                    }
                }
                pos = e
                offs |> push(pos - prev_end); prev_end = pos
                continue
            }
        }

        // \p{N} — one digit per token
        if (cpt_is_number(cpt)) {
            pos++
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // <space>?[^\s\p{L}\p{N}]+[\r\n/]*
        let probe = (cpt == 32) ? pos + 1 : pos
        if (!(uW(cpts, probe) || uL(cpts, probe) || uN(cpts, probe))) {
            if (cpt == 32) {
                pos++
            }
            while (!(uW(cpts, pos) || uL(cpts, pos) || uN(cpts, pos)) && pos < n) {
                pos++
            }
            var c2 = uC(cpts, pos)
            while (c2 == 13 || c2 == 10 || c2 == 47) {
                pos++
                c2 = uC(cpts, pos)
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // remaining whitespace rules: \s*[\r\n]+ | \s+(?!\S) | \s+ (same as llama3)
        var num_ws = 0
        var last_rn = 0
        while (uW(cpts, pos + num_ws)) {
            let c2 = uC(cpts, pos + num_ws)
            if (c2 == 13 || c2 == 10) {
                last_rn = pos + num_ws + 1
            }
            num_ws++
        }
        if (last_rn > 0) {
            pos = last_rn
        } elif (num_ws > 1 && uC(cpts, pos + num_ws) != -1) {
            pos += num_ws - 1
        } elif (num_ws > 0) {
            pos += num_ws
        } else {
            pos++  // no match — consume one codepoint
        }
        offs |> push(pos - prev_end); prev_end = pos
    }
    return <- offs
}

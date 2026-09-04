options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of src/ast/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang src/ast/REVIEW.das.

let private SOURCE = "src/ast/ast_parse.cpp"
let private READER = "trySerializeProgramModule"
let private GUARD = "quietCache"
let private PRINT = "logs <<"

def private count_char(line : string; ch : int) : int {
    var n = 0
    for (c in line) {
        if (c == ch) {
            n++
        }
    }
    return n
}

// Every print inside the reader sits under a quietCache guard - on the guard's own line, or in
// the block a guard line opens. Brace depth is tracked from the reader's opening line to its
// closing brace; a guard line that opens a block guards every line until the depth drops back.
def private check_reader_prints {
    let text = fread(SOURCE)
    if (empty(text)) {
        gate_finding(SOURCE, "cannot read the file the gate scans")
        return
    }
    var inscope lines <- split(text, "\n")
    var start = -1
    for (ln, i in lines, count()) {
        if (find(ln, READER) >= 0 && find(ln, "(") >= 0 && find(ln, ";") < 0 && find(ln, "return") < 0 && find(ln, "if") < 0) {
            start = i
            break
        }
    }
    if (start < 0) {
        gate_finding(SOURCE, "no definition of {READER} found - the gate scans its body for module-cache diagnostics that print without a {GUARD} guard")
        return
    }
    var depth = 0
    var opened = false
    var guardDepth = -1
    for (i in range(start, length(lines))) {
        let ln = lines[i]
        let guardLine = find(ln, GUARD) >= 0 && find(ln, "if") >= 0
        if (find(ln, PRINT) >= 0 && !guardLine && guardDepth < 0) {
            gate_finding("{SOURCE}:{i + 1}", "a module-cache read diagnostic prints without a {GUARD} guard - the default cache is on for every run that executes, so this line is output every user sees; wrap it in `if ( !serializer_read->{GUARD} )`")
        }
        let opens = count_char(ln, '{')
        let closes = count_char(ln, '}')
        depth += opens - closes
        if (opens > 0) {
            opened = true
        }
        if (guardLine && opens > closes) {
            guardDepth = depth
        }
        if (guardDepth >= 0 && depth < guardDepth) {
            guardDepth = -1
        }
        if (opened && depth <= 0) {
            break
        }
    }
}

[export]
def main {
    check_reader_prints()
    return gate_verdict("src/ast")
}

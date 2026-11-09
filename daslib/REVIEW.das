options gen2

require strings
require daslib/fio
require dastest/review_gate

// The mechanical half of daslib/REVIEW.md (contract: REVIEW_COMMON.md
// at the repo root). Run from the repo root: bin/daslang
// daslib/REVIEW.das - exit 0 clean, 1 with findings, 2 when not run
// from the repo root.

let private READER = "daslib/rst_comment"

// The daslib modules allowed in the //! comment reader's require
// closure (ARCHITECTURE_RST.md#comment-reader-closure); a require
// without a path is a builtin.
let private CLOSURE_ALLOWED <- {
    "daslib/ast",
    "daslib/rtti",
    "daslib/rst_label"
}

def private check_reader_closure {
    var closure <- [READER]
    var next = 0
    while (next < length(closure)) {
        let module_path = closure[next++]
        let path = "{module_path}.das"
        var inscope requires <- das_requires(path)
        if (empty(requires)) {
            gate_finding(path, "missing, unreadable, or has no require")
            continue
        }
        for (req in requires) {
            continue if (find(req, "/") < 0)
            if (!key_exists(CLOSURE_ALLOWED, req)) {
                gate_finding(path, find_line(fread(path), "require {req}"),
                    "requires {req}, which is then parsed before the //! comment reader exists - "
                    + "every //! comment of {req} is lost; the reader's closure stays builtin "
                    + "modules, daslib/ast, daslib/rtti and daslib/rst_label")
                continue
            }
            continue if (find_index(closure, req) >= 0)
            closure |> push(req)
        }
    }
}

[export]
def main() : int {
    if (!fexist("{READER}.das")) {
        to_log(LOG_ERROR, "daslib/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_reader_closure()
    return gate_verdict("daslib")
}

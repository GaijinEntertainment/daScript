options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of modules/dasMetal/REVIEW.md (contract: REVIEW_COMMON.md at the repo
// root). Run from the repo root: bin/daslang modules/dasMetal/REVIEW.das — exit 0 clean,
// 1 with findings.

let private EMITTER = "modules/dasMetal/metal/msl_emit.das"

// every matmul2d_descriptor the emitter writes sets relaxed_precision = true (arg 6) — an
// RP = false descriptor runs the op off the tensor-unit fast path
def private check_rp_law {
    var line_no = 0
    for (line in split(fread(EMITTER), "\n")) {
        line_no++
        let at = find(line, "matmul2d_descriptor(")
        continue if (at < 0)
        var inscope args <- split(slice(line, at + length("matmul2d_descriptor(")), ",")
        if (length(args) < 6) {
            gate_finding(EMITTER, line_no, "matmul2d_descriptor with fewer than 6 arguments - the relaxed_precision position is missing")
            continue
        }
        if (strip(args[5]) != "true" && !(strip(args[5]) |> starts_with("true)"))) {
            gate_finding(EMITTER, line_no, "matmul2d_descriptor with relaxed_precision != true - RP = false keeps the op off the tensor-unit fast path")
        }
    }
}

let private FC_DIR = "tests/msl/_fail_closed"
let private FC_TEST = "tests/msl/test_msl_fail_closed.das"

def private check_fail_closed_sync {
    var fixtures : table<string>
    dir(FC_DIR) $(name) {
        if (name |> ends_with(".das")) {
            fixtures |> insert(slice(name, 0, length(name) - 4))
        }
    }
    var asserted : table<string; int>
    var line_no = 0
    for (line in split(fread(FC_TEST), "\n")) {
        line_no++
        let at = find(line, "check_rejects(t, \"")
        continue if (at < 0)
        let rest = slice(line, at + length("check_rejects(t, \""))
        let q = find(rest, "\"")
        continue if (q <= 0)
        asserted[slice(rest, 0, q)] = line_no
    }
    for (fx in keys(fixtures)) {
        if (!key_exists(asserted, fx)) {
            gate_finding("{FC_DIR}/{fx}.das", 0, "fixture has no check_rejects assert in {FC_TEST} - its rejection is unpinned")
        }
    }
    for (name, ln in keys(asserted), values(asserted)) {
        if (!key_exists(fixtures, name)) {
            gate_finding(FC_TEST, ln, "check_rejects(\"{name}\") has no {FC_DIR}/{name}.das fixture - the assert is stranded")
        }
    }
}

let private SHIM = "modules/dasMetal/src/dasMetal.mm"

//! the getters a read outside a pool may send: each returns a scalar, so ARC autoreleases nothing behind it
let private SCALAR_GETTERS <- {"hasUnifiedMemory", "maxTotalThreadsPerThreadgroup", "threadExecutionWidth", "maxBufferLength"}

//! the line with its `//` comment and its string literals' text dropped
def private shim_code(line : string) : string {
    var out = ""
    var in_str = false
    var prev = 0
    peek_data(line) $(d) {
        for (i in range(length(d))) {
            let ch = int(d[i])
            if (!in_str && ch == '/' && prev == '/') {
                out = slice(out, 0, length(out) - 1)
                break
            }
            if (ch == '"' && prev != '\\') {
                in_str = !in_str
            } elif (!in_str) {
                out += to_char(ch)
            }
            prev = ch
        }
    }
    return out
}

//! the Objective-C object a declaration on `code` names (`id<P> x`, `id x`, `NSFoo * x`, `MTLFoo * x`), "" on any other line
def private objc_decl(code : string) : string {
    let s = strip(code)
    var rest = ""
    if (s |> starts_with("id<")) {
        let close = find(s, ">")
        rest = close > 0 ? slice(s, close + 1) : ""
    } elif (s |> starts_with("id ")) {
        rest = slice(s, 3)
    } elif ((s |> starts_with("NS") || s |> starts_with("MTL")) && find(s, " * ") > 0) {
        rest = slice(s, find(s, " * ") + 3)
    }
    let r = strip(rest)
    var n = 0
    peek_data(r) $(d) {
        while (n < length(d) && (is_alnum(int(d[n])) || int(d[n]) == '_')) {
            n++
        }
    }
    return slice(r, 0, n)
}

//! the parameters of a function header typed as an Objective-C object (`NSError * err`)
def private objc_params(code : string; var out : table<string>) {
    let open = find(code, "(")
    return if (open < 0)
    for (p in split(slice(code, open + 1), ",")) {
        let n = objc_decl(p)
        if (!empty(n)) {
            out |> insert(n)
        }
    }
}

//! the property `code` reads off `obj` (`obj.prop` not assigned, `[obj prop]` with no argument), "" when none
def private getter_read(code, obj : string) : string {
    var found = ""
    peek_data(code) $(d) {
        var at = find(d, "{obj}.")
        while (at >= 0 && empty(found)) {
            let before_ok = at == 0 || !(is_alnum(int(d[at - 1])) || int(d[at - 1]) == '_')
            let from = at + length(obj) + 1
            var n = from
            while (n < length(d) && is_alnum(int(d[n]))) {
                n++
            }
            let after = strip(slice(d, n))
            if (before_ok && n > from && !(after |> starts_with("=") && !(after |> starts_with("==")))) {
                found = slice(d, from, n)
            }
            at = find(d, "{obj}.", at + 1)
        }
    }
    return found if (!empty(found))
    let send = find(code, "[{obj} ")
    if (send >= 0) {
        let rest = slice(code, send + length(obj) + 2)
        let close = find(rest, "]")
        if (close > 0 && find(slice(rest, 0, close), ":") < 0) {
            return strip(slice(rest, 0, close))
        }
    }
    return ""
}

struct private ShimLine {
    code : string
    fn : string
    is_static : bool
    pooled : bool
}

//! every shim line with the function it sits in, whether that function is a `static` helper, and whether an
//! `@autoreleasepool` block holds the line
def private shim_lines : array<ShimLine> {
    var out : array<ShimLine>
    var depth = 0
    var inscope pools : array<int>
    var fn = ""
    var is_static = false
    var inscope raws <- split(fread(SHIM), "\n")
    out |> reserve(length(raws))
    for (raw in raws) {
        let code = shim_code(raw)
        if (depth <= 1 && find(code, "(") > 0 && strip(code) |> ends_with("\{")) {
            let head = slice(code, 0, find(code, "("))
            let sp = rfind(strip(head), " ")
            fn = sp >= 0 ? strip(slice(strip(head), sp + 1)) : strip(head)
            is_static = strip(code) |> starts_with("static ")
        }
        if (find(code, "@autoreleasepool") >= 0) {
            pools |> push(depth)
        }
        out |> push(ShimLine(code = code, fn = fn, is_static = is_static, pooled = !empty(pools)))
        peek_data(code) $(d) {
            for (ch in d) {
                depth += int(ch) == '{' ? 1 : int(ch) == '}' ? -1 : 0
            }
        }
        while (!empty(pools) && depth <= pools[length(pools) - 1]) {
            pools |> pop()
        }
    }
    return <- out
}

//! true when every call of `fn` on the shim's lines, its own header aside, sits inside a pool
def private called_only_pooled(fn : string; lines : array<ShimLine>) : bool {
    var seen = false
    for (l in lines) {
        continue if (l.fn == fn || find(l.code, "{fn}(") < 0)
        seen = true
        return false if (!l.pooled)
    }
    return seen
}

//! a shim binding reads an Objective-C property only inside an `@autoreleasepool`
def private check_shim_getters_pooled {
    return if (!fexist(SHIM))
    var inscope lines <- shim_lines()
    var inscope objs : table<string>
    var fn = ""
    for (i in range(length(lines))) {
        let l = lines[i]
        if (l.fn != fn) {
            fn = l.fn
            objs |> clear()
            objc_params(l.code, objs)
        }
        let decl = objc_decl(l.code)
        if (!empty(decl)) {
            objs |> insert(decl)
        }
        continue if (l.pooled || (l.is_static && called_only_pooled(l.fn, lines)))
        for (obj in keys(objs)) {
            let prop = getter_read(l.code, obj)
            if (!empty(prop) && !key_exists(SCALAR_GETTERS, prop)) {
                gate_finding(SHIM, i + 1, "`{fn}` reads `{obj}.{prop}` outside an `@autoreleasepool` - a getter that returns an object or an inner pointer autoreleases, and no pool drains on a daslang host thread, so the object leaks silently; read it inside the function's `@autoreleasepool` (modules/dasMetal/ARCHITECTURE.md, the extern surface); a getter returning a scalar is named in REVIEW.das's SCALAR_GETTERS")
            }
        }
    }
}

[export]
def main() : int {
    if (!fexist("modules/dasMetal/REVIEW.das") || !fexist(EMITTER)) {
        to_log(LOG_ERROR, "modules/dasMetal/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_rp_law()
    check_fail_closed_sync()
    check_shim_getters_pooled()
    return gate_verdict("dasmetal")
}

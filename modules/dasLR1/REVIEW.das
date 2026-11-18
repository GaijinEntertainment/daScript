options gen2

require strings
require daslib/strings_boost
require daslib/strings_convert
require daslib/fio
require dastest/review_gate

let private PORT_FILE = "json_schema.das"
let private PORT_LIMITS <- { "_cyclomatic_complexity" => 50, "_function_length" => 170 }

//! names of the builtin modules the C++ under src/ registers
def private core_builtin_modules() : table<string> {
    var names : table<string>
    for (src in tracked_files(["src/*.cpp"], "core builtin modules")) {
        var first = true
        for (part in split(fread(src), "Module(\"")) {
            let close = first ? -1 : find(part, "\"")
            first = false
            if (close > 0) {
                names |> insert(slice(part, 0, close))
            }
        }
    }
    return <- names
}

def private allowed_require(req : string; builtins : table<string>) : bool {
    return true if (starts_with(req, "lr1/") || starts_with(req, "daslib/"))
    return find(req, "/") < 0 && (key_exists(builtins, req) || fexist("daslib/{req}.das"))
}

def private check_requires(path : string; builtins : table<string>) {
    let fix = "meet the need at the caller: programs that load nothing under modules/ reuse lr1/"
    for (req in das_requires(path)) {
        continue if (allowed_require(req, builtins))
        gate_finding(path, "requires `{req}`, not in lr1/, daslib/ or the C++ under src/ - {fix}")
    }
}

def private check_limits(path : string) {
    let in_port = base_name(path) == PORT_FILE
    var line_no = 0
    for (raw in split(fread(path), "\n")) {
        line_no++
        var line = strip(raw)
        continue if (!starts_with(line, "options "))
        let cut = find(line, "//")
        if (cut >= 0) {
            line = slice(line, 0, cut)
        }
        for (opt in split(trim_prefix(line, "options "), ",")) {
            var inscope kv <- split(opt, "=")
            continue if (length(kv) != 2)
            let name = strip(kv[0])
            let limit = PORT_LIMITS?[name] ?? -1
            continue if (limit < 0)
            let value = try_to_int(strip(kv[1])) ?? (limit + 1)
            continue if (in_port && value <= limit)
            let port = "only {PORT_FILE} raises `{name}`, to {limit}, for its llama.cpp port"
            let raise = "`{strip(opt)}` raises a lint limit under lr1/"
            gate_finding(path, line_no, "{raise} - split the function instead; {port}")
        }
    }
}

[export]
def main() : int {
    var inscope builtins <- core_builtin_modules()
    for (path in tracked_files(["modules/dasLR1/lr1/*.das"], "the lr1 sources")) {
        check_requires(path, builtins)
        check_limits(path)
    }
    return gate_verdict("modules/dasLR1")
}

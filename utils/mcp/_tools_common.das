options gen2
options rtti

module _tools_common public

require dastest/testing_boost public
require daslib/json
require daslib/json_boost
require strings
require daslib/fio
require tools/common public
require tools/grep_usage public

def parse_result(result : string; var text : string&; var is_error : bool&) : bool {
    var err : string
    var parsed = read_json(result, err)
    if (parsed == null) return false
    let content = parsed?["content"]
    if (content == null) {
        unsafe { delete parsed; }
        return false
    }
    let first = content?[0]
    if (first == null) {
        unsafe { delete parsed; }
        return false
    }
    let text_val = first?["text"]
    if (text_val != null) {
        text = text_val.value as _string
    }
    let err_val = parsed?["isError"]
    if (err_val != null) {
        is_error = err_val.value as _bool
    }
    unsafe { delete parsed; }
    return true
}

def fixture_path(name : string) : string => "utils/mcp/tests/{name}"

def require_ast_grep(t : T?) : bool {
    if (!empty(get_ast_grep_cmd())) return true
    t->log("ast-grep not installed — skipping test (install @ast-grep/cli to enable)")
    return false
}

def json_array_has(arr : JsonValue?; wanted : string) : bool {
    if (arr == null || !(arr.value is _array)) return false
    for (m in arr.value as _array) {
        if (m != null && m.value is _string && (m.value as _string) == wanted) return true
    }
    return false
}

def make_policy_dir(name, config : string; var err : string&) : string {
    let dir = create_temp_directory(name, err)
    return "" if (empty(dir))
    mkdir(path_join(dir, ".git"))
    if (!empty(config)) {
        fwrite(path_join(dir, ".lint_config"), config)
    }
    return dir
}

def cpp_diag(text : string) : string {
    let trimmed = length(text) > 800 ? slice(text, 0, 800) : text
    return "envelope: {trimmed}"
}

let PROOT_CONSUMER = "_pretend_root/consumer.das"
let PROOT = "_pretend_root"
let PROOT_MISSING = "missing prerequisite 'pretend_mod"

def assert_prereq_pair(t : T?; what : string; without_it, with_it : string) {
    var t1 : string; var e1 = false
    parse_result(without_it, t1, e1)
    t |> success(e1, "without {what}: should be error -- text={t1}")
    t |> success(find(t1, PROOT_MISSING) >= 0,
                 "without {what}: should mention missing prerequisite -- text={t1}")
    var t2 : string; var e2 = false
    parse_result(with_it, t2, e2)
    t |> success(!e2, "with {what}: should NOT be error -- text={t2}")
}

let PRETEND_LM = "_pretend_root/modules/pretend_mod"

def make_lm_array(p : string) : array<string> {
    var arr : array<string>
    arr |> push(p)
    return <- arr
}

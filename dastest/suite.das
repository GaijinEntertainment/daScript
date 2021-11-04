options indenting = 4

module suite shared

require rtti
require ast
require strings
require log
require debugapi
require daslib/json_boost
require fio

struct SuiteResult
    passed: int
    failed: int
    errors: int
    total: int


def SuiteResult(passed: int; failed: int; total: int)
    return [[SuiteResult passed=passed, failed=failed, total=total]]


def operator +=(var a: SuiteResult; b: SuiteResult)
    a.passed += b.passed
    a.failed += b.failed
    a.total += b.total
    a.errors += b.errors


def SuiteResult(val: JsonValue?)
    if val == null
        return [[SuiteResult]]
    let obj & = val as _object
    return [[SuiteResult
        passed=int((obj?["passed"] ?? JV(0lf)) as _number),
        failed=int((obj?["failed"] ?? JV(0lf)) as _number),
        total=int((obj?["total"] ?? JV(0lf)) as _number),
        errors=int((obj?["errors"] ?? JV(0lf)) as _number)
    ]]


def test_file(fileName: string): SuiteResult
    // log::info("\n===> {fileName}", get_line_info())
    var res: SuiteResult
    var access := make_file_access("")
    using <| $(var mg:ModuleGroup#)
        using <| $(var cop:CodeOfPolicies#)
            cop.aot_module = true
            compile_file(fileName, access, addr(mg), cop) <| $(ok, program, errors)
                if !ok
                    res.total += 1
                    res.errors += 1
                    log::error("Failed to compile {fileName}\n{errors}", get_line_info())
                    if program != null
                        for err in program.errors
                            log::error("{describe(err.at)}: {int(err.cerr)}: {err.what} {err.extra} {err.fixme}", get_line_info())
                    return
                simulate(program) <| $ (sok; context; serrors)
                    if !sok
                        res.total += 1
                        res.errors += 1
                        log::error("Failed to compile {fileName}\n{serrors}", get_line_info())
                        return
                    let mod = program |> get_this_module()
                    mod |> module_for_each_function <| $(f)
                        if f.hash == 0u
                            return
                        let func = *context |> get_function_by_mnh(f.hash)
                        let fn <- mod |> find_module_function_via_rtti(func)
                        if fn == null
                            return
                        for ann in fn.annotations
                            if ann.annotation.name == "test"
                                var name = "'{fn.name}'"
                                for arg in ann.arguments
                                    if arg.name == "name"
                                        name = "'{arg.sValue}'"
                                log::info("\n=== RUN {name}", get_line_info())
                                res.total += 1
                                var dt: int
                                unsafe
                                    try
                                        var t0 = ref_time_ticks()
                                        context |> invoke_in_context(func)
                                        dt = get_time_usec(t0)
                                    recover
                                        res.failed += 1
                                        log::info("\n--- FAIL {name} ({double(dt)/1000000.0lf}s)", get_line_info())
                                        log::info(context.exception, get_line_info())
                                        *context |> stackwalk(context.exceptionAt)
                                        return
                                res.passed += 1
                                log::info("\n--- PASS {name} ({double(dt)/1000000.0lf}s)", get_line_info())
                                return
    access := null
    return res

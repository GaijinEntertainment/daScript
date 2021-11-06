options indenting = 4

module suite shared

require rtti
require ast
require strings
require log
require debugapi
require daslib/json_boost
require fio
require testing

struct SuiteResult
    passed: int // number of passed tests
    failed: int // number of failed tests
    errors: int // compile, runtime error
    total: int // total number of tests
    skipped: int // number of skipped tests


def operator +=(var a: SuiteResult; b: SuiteResult)
    a.passed += b.passed
    a.failed += b.failed
    a.total += b.total
    a.errors += b.errors
    a.skipped += b.skipped


def SuiteResult(val: JsonValue?)
    let obj & = val as _object
    return [[SuiteResult
        passed=int((obj?["passed"] ?? JV(0lf)) as _number),
        failed=int((obj?["failed"] ?? JV(0lf)) as _number),
        total=int((obj?["total"] ?? JV(0lf)) as _number),
        errors=int((obj?["errors"] ?? JV(0lf)) as _number),
        skipped=int((obj?["skipped"] ?? JV(0lf)) as _number)
    ]]

struct SuiteCtx
    dastestRoot: string

def test_file(file_name: string; ctx: SuiteCtx): SuiteResult
    // log::info("\n===> {fileName}", get_line_info())
    var res: SuiteResult
    var access := make_file_access("")
    access |> add_file_access_root("dastest", ctx.dastestRoot)
    using <| $(var mg:ModuleGroup#)
        using <| $(var cop:CodeOfPolicies#)
            cop.aot_module = true
            compile_file(file_name, access, addr(mg), cop) <| $(ok, program, errors)
                if !ok
                    res.total += 1
                    res.errors += 1
                    log::error("Failed to compile {file_name}\n{errors}", get_line_info())
                    if program != null
                        for err in program.errors
                            log::error("{describe(err.at)}: {int(err.cerr)}: {err.what}", get_line_info())
                            if !empty(err.extra)
                                log::info("{err.extra}", get_line_info())
                            if !empty(err.fixme)
                                log::info("{err.fixme}", get_line_info())
                    return
                simulate(program) <| $ (sok; context; serrors)
                    if !sok
                        res.total += 1
                        res.errors += 1
                        log::error("Failed to compile {file_name}\n{serrors}", get_line_info())
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
                                var name = "{fn.name}"
                                for arg in ann.arguments
                                    if arg.name == "name"
                                        name = "{arg.sValue}"
                                log::info("\n=== RUN '{name}'", get_line_info())
                                res.total += 1
                                let t0 = ref_time_ticks()
                                var dt = 0
                                var deliberateRecover = false
                                var failed = false
                                var skipped = false
                                unsafe
                                    try
                                        if length(fn.arguments) == 0
                                            context |> invoke_in_context(func)
                                        else
                                            var testing = new T(name)
                                            testing.onFail <- @ <| [[&testing, &failed]]()
                                                testing.failed = true
                                                failed = true

                                            testing.onFailNow <- @ <| [[&testing, &failed, &deliberateRecover]]()
                                                testing.failed = true
                                                failed = true
                                                deliberateRecover = true

                                            testing.onSkipNow <- @ <| [[&testing, &skipped, &deliberateRecover]]()
                                                testing.skipped = true
                                                skipped = true
                                                deliberateRecover = true

                                            testing.onLog <- @ <| (msg: string)
                                                log::info("{name}: {msg}", get_line_info())

                                            context |> invoke_in_context(func, testing)
                                            delete testing
                                        dt = get_time_usec(t0)
                                    recover
                                        if !deliberateRecover
                                            dt = get_time_usec(t0)
                                            res.errors += 1
                                            log::info("\n--- FAIL '{name}' ({double(dt)/1000000.0lf}s)", get_line_info())
                                            log::info(context.exception, get_line_info())
                                            *context |> stackwalk(context.exceptionAt)
                                            // TODO: context |> reset()
                                            return
                                if failed
                                    res.failed += 1
                                    log::info("\n--- FAIL '{name}' ({double(dt)/1000000.0lf}s)", get_line_info())
                                elif skipped
                                    res.skipped += 1
                                    log::info("\n--- SKIPPED '{name}' ({double(dt)/1000000.0lf}s)", get_line_info())
                                else
                                    res.passed += 1
                                    log::info("\n--- PASS '{name}' ({double(dt)/1000000.0lf}s)", get_line_info())
                                return
    access := null
    return res

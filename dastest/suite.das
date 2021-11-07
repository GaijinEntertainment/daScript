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
require uriparser

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
    uriPaths: bool = false
    testNames: array<string>

struct SuiteFileCtx
    context: smart_ptr<rtti::Context>
    uriPaths: bool = false
    indenting: string = ""


def private match_test_name(name: string; ctx: SuiteCtx)
    if length(ctx.testNames) == 0
        return true
    for match in ctx.testNames
        if name |> starts_with(match)
            return true
    return false


def test_file(file_name: string; ctx: SuiteCtx): SuiteResult
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
                    log::error("Failed to compile {file_name}\n{errors}")
                    if program != null
                        for err in program.errors
                            log::error("{describe(err.at)}: {int(err.cerr)}: {err.what}")
                            if !empty(err.extra)
                                log::info("{err.extra}")
                            if !empty(err.fixme)
                                log::info("{err.fixme}")
                    return
                simulate(program) <| $ (sok; context; serrors)
                    if !sok
                        res.total += 1
                        res.errors += 1
                        log::error("Failed to compile {file_name}\n{serrors}")
                        return
                    var fileCtx <- [[SuiteFileCtx context:=context, uriPaths=ctx.uriPaths]]
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
                                if match_test_name(name, ctx)
                                    test_any_func(name, func, length(fn.arguments), fileCtx, res)
                                return
    access := null
    return res


def test_func(name: string; func: function<(t: T?): void>; var context: SuiteFileCtx): SuiteResult
    var res: SuiteResult
    test_any_func(name, func, 1, context, res)
    return res


def test_func(name: string; func: function<(): void>; var context: SuiteFileCtx): SuiteResult
    var res: SuiteResult
    test_any_func(name, func, 0, context, res)
    return res


def private test_any_func(name: string; func: function; args_num: int; var context: SuiteFileCtx; var res: SuiteResult&)
    log::info("{context.indenting}=== RUN '{name}'")
    let beforeFailed = res.failed
    res.total += 1
    var dt = 0
    var deliberateRecover = false
    var failed = false
    var skipped = false
    unsafe
        let t0 = ref_time_ticks()
        try
            if args_num == 0
                *context.context |> invoke_in_context(func)
                dt = get_time_usec(t0)
            else
                var testing = new T(name, t0)
                testing.onFail <- @ <| [[&testing, &failed, &deliberateRecover]](now: bool)
                    testing.failed = true
                    failed = true
                    deliberateRecover = now

                testing.onSkipNow <- @ <| [[&testing, &skipped, &deliberateRecover]]()
                    testing.skipped = true
                    skipped = true
                    deliberateRecover = true

                var selfCtx & = this_context()
                testing.onRun <- @ <| [[&selfCtx, &context, &res]](test_name: string; f: function<(t: T?): void>)
                    var subContext := context
                    subContext.indenting = "\t" + subContext.indenting
                    unsafe
                        selfCtx |> invoke_in_context("test_any_func", test_name, f, 1, subContext, res)

                testing.onLog <- @ <| [[&context]](msg: string; at: LineInfo)
                    let uri = at.fileInfo == null ? "" : context.uriPaths ? "{file_name_to_uri(string(at.fileInfo.name))}#" : "{at.fileInfo.name}:"
                    log::info("{context.indenting}{uri}{int(at.line)}: {msg}")

                *context.context |> invoke_in_context(func, testing)
                dt = get_time_usec(t0)
                delete testing
        recover
            if !deliberateRecover
                dt = get_time_usec(t0)
                res.errors += 1
                log::info("{context.indenting}--- FAIL '{name}' ({double(dt)/1000000.0lf}s)")
                log::info("{context.indenting}{context.context.exception}")
                *context.context |> stackwalk(context.context.exceptionAt)
                return
    if failed
        res.failed += 1
    elif skipped
        res.skipped += 1
        log::info("{context.indenting}--- SKIPPED '{name}' ({double(dt)/1000000.0lf}s)")
    else
        res.passed += 1

    let status = beforeFailed < res.failed ? "FAIL" : "PASS"
    log::info("{context.indenting}--- {status} '{name}' ({double(dt)/1000000.0lf}s)")

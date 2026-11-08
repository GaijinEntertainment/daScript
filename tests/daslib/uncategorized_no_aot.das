options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio
require strings
require daslib/ast
require daslib/cpp_gen
require daslib/ast_boost
require daslib/rtti
require daslib/command_line

[test(description = "a host binary copied under another name finds itself by an absolute path")]
def test_get_das_exe_host_not_named_daslang(t : T?) {
    if (get_platform_name() == "windows") {
        t |> skip("a copied host would lose the DLLs next to it")
        return
    }
    let host = path_join(test_temp_dir(), "das_host")
    var err : string
    let copied = copy_file(get_command_line_arguments()[0], host, true, err)
    t |> success(copied, "copied the host to '{host}': {err}")
    let rel = relative(host, getcwd(), err)
    if (!empty(err) || is_absolute(rel)) {
        t |> skip("the temp directory has no path relative to the current one: {err}")
        return
    }
    var output : string
    let root = get_das_root()
    let rc = unsafe(
                    popen_argv(
                               [rel, "-dasroot", root,
                                path_join(root, "tests/daslib/_das_exe_probe.das")], 0.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
    t |> equal(rc, 0, "the host starts by '{rel}': {output}")
    let child = strip(output)
    t |> success(is_absolute(child) && fexist(child), "a host with no daslang next to it finds itself by an absolute path, not '{child}'")
}

[test(description = "a simulate macro still has its program after an init function compiled another script")]
def test_simulate_macros_run_after_a_compile_in_init(t : T?) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            compile_file("{get_das_root()}/tests/daslib/_nested_compile_outer.das", access,
                         unsafe(addr(mg)), cop) $(ok; program; issues) {
                t |> success(ok, "the outer program compiles: {issues}")
                if (!ok) {
                    return
                }
                simulate(program) $(sok; _context; serrors) {
                    t |> success(sok, "a simulate macro still has its program after an [init] compiled another script: {serrors}")
                }
            }
        }
    }
}

let HEADER_TEXT = ("#define TCG_HEX 0x10\n" +
    "#define TCG_DEC 42\n" +
    "#define TCG_U8 UINT8_C(0x7f)\n" +
    "#define TCG_U16 UINT16_C(0x1234)\n" +
    "#define TCG_U32 UINT32_C(0xdeadbeef)\n" +
    "#define TCG_U64 UINT64_C(0x1122334455667788)\n" +
    "#define TCG_DEC 43\n" +
    "#define TCG_TEXT \"not a constant\"\n")

def private write_header(t : T?; header : string) {
    fopen(header, "wb") $(fw) {
        if (fw == null) {
            t |> failure("cannot write {header}")
            return
        }
        fwrite(fw, HEADER_TEXT)
    }
}

[test(description = "genDefineConstants emits typed constants for the usable defines and skips the text and redefined ones")]
def test_cpp_gen_defines(t : T?) {
    var tmp_err : string
    let tmp_dir = create_temp_directory("cpp_gen_defines", tmp_err)
    if (!empty(tmp_err)) {
        t |> failure("cannot create temp directory: {tmp_err}")
        return
    }
    let header = path_join(tmp_dir, "constants.h")
    write_header(t, header)

    let prefix = path_join(tmp_dir, "constants")
    var headers <- [ header ]
    genDefineConstants(headers, prefix)
    delete headers

    var generated = ""
    fopen("{prefix}.const_inc", "rb") $(fr) {
        if (fr != null) {
            generated = fread(fr)
        }
    }
    t |> equal(generated, ("addConstant<uint32_t>(*this,\"TCG_HEX\",uint32_t(0x10u));\n" +
        "addConstant<int32_t>(*this,\"TCG_DEC\",int32_t(42));\n" +
        "addConstant<uint8_t>(*this,\"TCG_U8\",uint8_t(0x7fu));\n" +
        "addConstant<uint16_t>(*this,\"TCG_U16\",uint16_t(0x1234u));\n" +
        "addConstant<uint32_t>(*this,\"TCG_U32\",uint32_t(0xdeadbeefu));\n" +
        "addConstant<uint64_t>(*this,\"TCG_U64\",uint64_t(0x1122334455667788ul));\n"))

    remove("{prefix}.const_inc")
    remove(header)
    rmdir(tmp_dir)
}

[test(description = "break and continue before a var inscope leave it zeroed for the finalizer")]
def test_inscope_after_nested_early_out(t : T?) {
    t |> run("break and continue before a var inscope leave it zeroed for the finalizer") @(t : T?) {
        var output : string
        let root = get_das_root()
        let rc = unsafe(popen_argv([get_command_line_arguments()[0], "-dasroot", root,
                                    path_join(root, "tests/daslib/_inscope_early_out_probe.das")],
                                    0.0, $(f) {
            if (f != null) {
                output = fread(f)
            }
        }))
        t |> equal(rc, 0, "the probe runs: {output}")
        t |> equal(strip(output), "6 3")
    }
}

def only_nttp_fixture_path(name : string) : string => "{get_das_root()}/tests/daslib/{name}.das"

def only_nttp_compile_fixture(name : string; blk : block<(ok : bool; issues : string) : void>) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            compile_file(only_nttp_fixture_path(name), access, unsafe(addr(mg)),
                         cop) $(ok; _program; issues) {
                invoke(blk, ok, "{issues}")
            }
        }
    }
}

[test(description = "an only_nttp kernel compiles with inline-bound calls and fails naming the member-bound call")]
def test_only_nttp_annotation(t : T?) {
    t |> run("kernel calling only inline-bound externs compiles") @(t : T?) {
        only_nttp_compile_fixture("_only_nttp_fixture_good") $(ok, issues) {
            t |> success(ok, "expected clean compile, got: {issues}")
        }
    }
    t |> run("kernel calling a member-bound extern fails naming the call") @(t : T?) {
        only_nttp_compile_fixture("cant_only_nttp_fixture") $(ok, issues) {
            t |> success(!ok, "expected compile failure, fixture compiled clean")
            t |> success(issues |> find("only_nttp") >= 0,
                         "diagnostic must name the annotation: {issues}")
            t |> success(issues |> find("get_env_variable") >= 0,
                         "diagnostic must name the offending call: {issues}")
        }
    }
}

//! The memoized recursion walk takes ~2 ms on the fan-out fixture and
//! ~12 s without the memo.

let private BUDGET_USEC = 5000000

def private verify_fixture_path(name : string) : string =>
    "{get_das_root()}/tests/daslib/_verify_completion_{name}_fixture.das"

[test(description = "the recursion walk memoizes so a wide fan-out graph compiles within budget")]
def test_recursion_walk_visits_each_function_once(t : T?) {
    t |> run("wide fan-out graph compiles within budget") @(t : T?) {
        var inscope access <- make_file_access("")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.threadlock_context = true
                let started = ref_time_ticks()
                compile_file(verify_fixture_path("fanout"), access, unsafe(addr(mg)),
                             cop) $(ok; program; issues) {
                    let elapsed = get_time_usec(started)
                    t |> success(ok, "fixture must compile: {issues}")
                    t |> success(elapsed < BUDGET_USEC,
                                 "compiled in {elapsed}us, budget {BUDGET_USEC}us")
                }
            }
        }
    }
}

[test(description = "a cycle closing through a shared function is still reported as recursion")]
def test_recursion_through_a_shared_callee_is_still_reported(t : T?) {
    t |> run("cycle closing through a shared function fails to compile") @(t : T?) {
        var inscope access <- make_file_access("")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.threadlock_context = true
                compile_file(verify_fixture_path("cycle"), access, unsafe(addr(mg)),
                             cop) $(ok; program; issues) {
                    t |> success(!ok, "cyclic fixture must not compile")
                    t |> success(find("{issues}", "recursive calls are not allowed") >= 0,
                                 "expected a recursion error, got: {issues}")
                }
            }
        }
    }
}

//! build_const end-to-end: the fixture child inherits this process's
//! env; set-but-empty counts as unset, so cleanup sets "".

def private run_fixture(t : T?; tmp_dir, tag, extra_args : string;
                        expect_ok : bool = true) : string {
    let fixture = path_join(get_das_root(), "tests/daslib/_fixture_build_const.das")
    let out_path = path_join(tmp_dir, "{tag}.txt")
    // exe and fixture unquoted: a leading quote breaks Windows _popen
    let cmd = "{get_das_exe()} -dasroot {get_das_root()} {fixture} -- {extra_args} --out {out_path} 2>&1"
    var child_output : string
    let rc = unsafe(popen(cmd) $(f) {
        if (f != null) {
            child_output = fread(f)
        }
    })
    feint("{child_output}")
    if (expect_ok) {
        t |> equal(0, rc)
    } else {
        t |> success(rc != 0, "expected the child compile to fail")
    }
    var content = ""
    fopen(out_path, "rb") $(f) {
        if (f != null) {
            content = fread(f)
        }
    }
    return content
}

def private clear_bct_env() {
    // "" counts as unset for every accessor
    for (name in ["BCT_PROF", "BCT_NAME", "BCT_LEVEL", "BCT_EFLAG", "BCT_EVAL", "BCT_EINT"]) {
        set_env_variable(name, "")
    }
}

[test(description = "build constants read argv and env carriers with the documented precedence, spellings and compile errors", timeout = 1200)]
def test_build_const(t : T?) {
    var tmp_err : string
    let tmp_dir = create_temp_directory("bct", tmp_err)
    if (!empty(tmp_err)) {
        t |> failure("cannot create temp directory: {tmp_err}")
        return
    }
    clear_bct_env()

    t |> run("defaults with no carriers") @(t : T?) {
        let got = run_fixture(t, tmp_dir, "defaults", "")
        t |> equal("PROF=false;NAME=fallback;LEVEL=7;HAS=false;AVAL=adef;AINT=1;EFLAG=false;EVAL=edef;EINT=2;GATE=off", got)
    }

    t |> run("argv carriers, both spellings") @(t : T?) {
        let got = run_fixture(t, tmp_dir, "argv", "--bct-prof --bct-name cli --bct-level=3 --bct-has --bct-aval=av --bct-aint 5")
        t |> equal("PROF=true;NAME=cli;LEVEL=3;HAS=true;AVAL=av;AINT=5;EFLAG=false;EVAL=edef;EINT=2;GATE=on", got)
    }

    t |> run("flag accepts the =value spelling with the shared bool rule") @(t : T?) {
        let got_on = run_fixture(t, tmp_dir, "flageq_on", "--bct-prof=1")
        t |> equal("PROF=true;NAME=fallback;LEVEL=7;HAS=false;AVAL=adef;AINT=1;EFLAG=false;EVAL=edef;EINT=2;GATE=on", got_on)
        let got_off = run_fixture(t, tmp_dir, "flageq_off", "--bct-prof=0")
        t |> equal("PROF=false;NAME=fallback;LEVEL=7;HAS=false;AVAL=adef;AINT=1;EFLAG=false;EVAL=edef;EINT=2;GATE=off", got_off)
    }

    t |> run("last occurrence wins") @(t : T?) {
        let got = run_fixture(t, tmp_dir, "lastwins", "--bct-name first --bct-name=second")
        t |> equal("PROF=false;NAME=second;LEVEL=7;HAS=false;AVAL=adef;AINT=1;EFLAG=false;EVAL=edef;EINT=2;GATE=off", got)
    }

    t |> run("env carriers") @(t : T?) {
        set_env_variable("BCT_PROF", "yes")
        set_env_variable("BCT_NAME", "envn")
        set_env_variable("BCT_LEVEL", "9")
        set_env_variable("BCT_EFLAG", "1")
        set_env_variable("BCT_EVAL", "ev")
        set_env_variable("BCT_EINT", "4")
        let got = run_fixture(t, tmp_dir, "env", "")
        t |> equal("PROF=true;NAME=envn;LEVEL=9;HAS=false;AVAL=adef;AINT=1;EFLAG=true;EVAL=ev;EINT=4;GATE=on", got)
        clear_bct_env()
    }

    t |> run("argv beats env") @(t : T?) {
        set_env_variable("BCT_PROF", "0")
        set_env_variable("BCT_NAME", "envn")
        set_env_variable("BCT_LEVEL", "9")
        let got = run_fixture(t, tmp_dir, "precedence", "--bct-prof --bct-name cli --bct-level 3")
        t |> equal("PROF=true;NAME=cli;LEVEL=3;HAS=false;AVAL=adef;AINT=1;EFLAG=false;EVAL=edef;EINT=2;GATE=on", got)
        clear_bct_env()
    }

    t |> run("env false spellings") @(t : T?) {
        set_env_variable("BCT_PROF", "off")
        set_env_variable("BCT_EFLAG", "No")
        let got = run_fixture(t, tmp_dir, "envfalse", "")
        t |> equal("PROF=false;NAME=fallback;LEVEL=7;HAS=false;AVAL=adef;AINT=1;EFLAG=false;EVAL=edef;EINT=2;GATE=off", got)
        clear_bct_env()
    }

    t |> run("set-but-empty env counts as unset") @(t : T?) {
        clear_bct_env()
        let got = run_fixture(t, tmp_dir, "emptyenv", "")
        t |> equal("PROF=false;NAME=fallback;LEVEL=7;HAS=false;AVAL=adef;AINT=1;EFLAG=false;EVAL=edef;EINT=2;GATE=off", got)
    }

    t |> run("garbage int is a compile error") @(t : T?) {
        let got = run_fixture(t, tmp_dir, "garbage", "--bct-aint xyz", false)
        t |> equal("", got)
    }

    // cleanup: out files + the temp dir itself
    for (
         tag in ["defaults", "argv", "lastwins", "env", "precedence", "envfalse", "emptyenv",
                 "garbage"]) {
        remove(path_join(tmp_dir, "{tag}.txt"))
    }
    rmdir(tmp_dir)
}

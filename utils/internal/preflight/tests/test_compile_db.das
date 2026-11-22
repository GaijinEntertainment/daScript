options gen2
options indenting = 4

require dastest/testing_boost public
require daslib/strings_boost
require daslib/fio

require ../compile_db.das


// The tokenizer's failure mode is silent: a mangled path yields a token that simply does not
// name the file it should, and the gate then reports an error against the wrong input - or a
// spurious one. Windows is the case the author is least likely to be running, so it is pinned
// here rather than left to a red CI lane.
[test(description = "the compile-command tokenizer keeps backslash paths intact, a trailing backslash included")]
def test_cc_tokenize_windows_paths(t : T?) {
    t |> run("a backslash path survives intact") @(t : T?) {
        let tok <- cc_tokenize("clang-cl -IC:\\src\\daScript\\include C:\\src\\x.cpp")
        t |> equal(length(tok), 3)
        t |> equal(tok[0], "clang-cl")
        t |> equal(tok[1], "-IC:\\src\\daScript\\include")
        t |> equal(tok[2], "C:\\src\\x.cpp")
    }
    t |> run("a trailing backslash is kept") @(t : T?) {
        let tok <- cc_tokenize("cl -IC:\\src\\")
        t |> equal(length(tok), 2)
        t |> equal(tok[1], "-IC:\\src\\")
    }
}


// A per-file output switch that survives into the grouped argv makes clang-cl reject the
// sweep for naming one output for many inputs, and the gate reports that as if the source
// were broken. The joined MSVC spellings are the ones a POSIX-only author misses.
[test(description = "per-file output switches in MSVC and GNU spellings, joined forms included, are stripped and real flags are kept")]
def test_cc_is_compile_switch(t : T?) {
    t |> run("msvc spellings are stripped") @(t : T?) {
        t |> success(cc_is_compile_switch("/c", false), "/c")
        t |> success(cc_is_compile_switch("/FoC:\\build\\x.obj", false), "joined /Fo")
        t |> success(cc_is_compile_switch("-FoC:\\build\\x.obj", false), "joined -Fo")
        t |> success(cc_is_compile_switch("/Fdvc140.pdb", false), "joined /Fd")
        t |> success(cc_is_compile_switch("/showIncludes", false), "/showIncludes")
        t |> success(cc_is_compile_switch("/YuC:/build/cmake_pch.hxx", false), "joined /Yu")
        t |> success(cc_is_compile_switch("/YcC:/build/cmake_pch.hxx", false), "joined /Yc")
        t |> success(cc_is_compile_switch("-YuC:/build/cmake_pch.hxx", false), "joined -Yu")
        t |> success(cc_is_compile_switch("-YcC:/build/cmake_pch.hxx", false), "joined -Yc")
    }
    t |> run("gnu spellings are stripped") @(t : T?) {
        t |> success(cc_is_compile_switch("-c", false), "-c")
        t |> success(cc_is_compile_switch("-MD", false), "-MD")
    }
    t |> run("cl keeps its runtime library switches") @(t : T?) {
        t |> success(!cc_is_compile_switch("-MD", true), "-MD under cl")
        t |> success(cc_is_compile_switch("/c", true), "/c under cl")
        t |> success(cc_is_compile_switch("/FoC:/build/x.obj", true), "joined /Fo under cl")
    }
    t |> run("real flags are kept") @(t : T?) {
        t |> success(!cc_is_compile_switch("/I../include", false), "include dir")
        t |> success(!cc_is_compile_switch("-DFOO=1", false), "define")
        t |> success(!cc_is_compile_switch("/std:c++17", false), "std")
        t |> success(!cc_is_compile_switch("-fno-rtti", false), "codegen flag")
    }
}


[test(description = "a sweep inherits a compile command's flags without the compiler, the file, its output switches or a precompiled header's binary, and keeps the header the binary stands for")]
def test_cc_sweep_flags(t : T?) {
    let argv <- ["/usr/bin/clang++", "-DFOO=1", "-Xclang", "-include-pch", "-Xclang",
        "/b/cmake_pch.hxx.pch", "-Xclang", "-include", "-Xclang", "/b/cmake_pch.hxx", "-MD",
        "-MT", "x.o", "-MF", "x.o.d", "-o", "x.o", "-c", "/src/x.cpp", "-fno-rtti"]
    let flags <- cc_sweep_flags(argv, "x.cpp", "/src/x.cpp", false)
    let kept = "-DFOO=1 -Xclang -include -Xclang /b/cmake_pch.hxx -fno-rtti"
    t |> equal(join(flags, " "), kept, "the inherited flags")
    let no_path <- ["cc", "-Xclang", "-include-pch", "-Xclang"]
    let cut <- cc_sweep_flags(no_path, "x.cpp", "/src/x.cpp", false)
    t |> equal(join(cut, " "), "-Xclang -include-pch -Xclang", "a switch with no path stays")
    let cl_argv <- ["clang-cl", "-MT", "/EHsc", "-MD", "/c", "-o", "x.obj", "x.cpp"]
    let cl_flags <- cc_sweep_flags(cl_argv, "x.cpp", "/src/x.cpp", true)
    t |> equal(join(cl_flags, " "), "-MT /EHsc -MD", "cl's -MT and -MD are the runtime library")
}


// A configure done through a differently-cased spelling of the checkout (macOS and Windows
// fold case) writes that spelling into the database; a case-honoring lookup then misses every
// entry and the gate sweeps with its approximate include list instead, silently.
[test(description = "the database key folds case on Windows and macOS and keeps it elsewhere, so each file has one spelling")]
def test_cc_key_folds_case_where_the_filesystem_does(t : T?) {
    t |> run("the key is one spelling per file") @(t : T?) {
        let platform = get_platform_name()
        let folded = platform == "windows" || platform == "darwin"
        t |> equal(cc_key("/Users/x/Work/daScript/src/a.cpp") == cc_key("/Users/x/work/daScript/src/a.cpp"), folded)
        t |> equal(cc_key("/tmp/A.cpp"), folded ? "/tmp/a.cpp" : "/tmp/A.cpp")
    }
    t |> run("Windows folds the separator too, the others keep it") @(t : T?) {
        t |> equal(cc_key_on("windows", "D:\\W\\src/A.cpp"), "d:/w/src/a.cpp")
        t |> equal(cc_key_on("darwin", "/Users/X\\a.cpp"), "/users/x\\a.cpp")
        t |> equal(cc_key_on("linux", "/tmp/X\\a.cpp"), "/tmp/X\\a.cpp")
    }
}


[test(description = "the tokenizer unescapes quotes and backslashes, keeps a quoted token's spaces, and mints no empty token from whitespace runs")]
def test_cc_tokenize_quoting(t : T?) {
    t |> run("an escaped quote in a define becomes a literal quote") @(t : T?) {
        let tok <- cc_tokenize("clang++ -DDAS_INSTALL_BINDIR=\\\"/usr/local/bin\\\" -c a.cpp")
        t |> equal(length(tok), 4)
        t |> equal(tok[1], "-DDAS_INSTALL_BINDIR=\"/usr/local/bin\"")
    }
    t |> run("a quoted token keeps its spaces") @(t : T?) {
        let tok <- cc_tokenize("clang++ \"-IC:/Program Files/x\" a.cpp")
        t |> equal(length(tok), 3)
        t |> equal(tok[1], "-IC:/Program Files/x")
    }
    t |> run("an escaped backslash collapses to one") @(t : T?) {
        let tok <- cc_tokenize("clang++ -DX=\\\\ a.cpp")
        t |> equal(length(tok), 3)
        t |> equal(tok[1], "-DX=\\")
    }
    t |> run("runs of whitespace do not mint empty tokens") @(t : T?) {
        let tok <- cc_tokenize("  clang++   -c    a.cpp  ")
        t |> equal(length(tok), 3)
        t |> equal(tok[0], "clang++")
        t |> equal(tok[2], "a.cpp")
    }
}


def private reply_dir(build : string) : string => path_join(build, ".cmake/api/v1/reply")


def private one_group(language, fragment, define, inc_dir : string;
                      sources : array<int>) : CmCompileGroup {
    var g = CmCompileGroup(language = language)
    g.compileCommandFragments |> push(CmFragment(fragment = fragment))
    g.defines |> push(CmDefine(define = define))
    g.includes |> push(CmInclude(path = inc_dir))
    g.sourceIndexes := sources
    return <- g
}


//! a reply under `build`: two targets in configuration Release, one
//! in Debug, an index that names the code model and, with a compiler,
//! the toolchains
def private write_reply(t : T?; build, index_name, cxx_compiler : string) {
    let reply = reply_dir(build)
    t |> success(mkdir_rec(reply), "the reply folder")
    let first <- CmTarget(
        sources <- [CmSource(path = "src/a.cpp"), CmSource(path = "src/c.c"),
            CmSource(path = path_join(build, "gen/g.cpp"))],
        compileGroups <- [one_group("CXX", "/EHsc /fp:fast", "FIRST=1", "inc/first", [0, 2]),
            one_group("C", "/TC", "C_ONLY", "inc/c", [1])])
    let second <- CmTarget(
        sources <- [CmSource(path = "src/a.cpp"), CmSource(path = "src/b.cpp")],
        compileGroups <- [one_group("CXX", "/EHsc", "SECOND=1", "inc/second", [0, 1])])
    let debug <- CmTarget(
        sources <- [CmSource(path = "src/a.cpp")],
        compileGroups <- [one_group("CXX", "/Od", "DEBUG_TREE", "inc/debug", [0])])
    var model = CmCodeModel(paths = CmPaths(source = build))
    model.configurations <- [
        CmConfiguration(name = "Debug", targets <- [CmTargetRef(jsonFile = "target-debug.json")]),
        CmConfiguration(name = "Release", targets <- [CmTargetRef(jsonFile = "target-first.json"),
            CmTargetRef(jsonFile = "target-second.json")])]
    var index : CmIndex
    index.objects |> push(CmObject(kind = "codemodel", jsonFile = "codemodel.json"))
    if (!empty(cxx_compiler)) {
        let tools <- CmToolchains(toolchains <- [
            CmToolchain(language = "CXX", compiler = CmCompiler(path = cxx_compiler)),
            CmToolchain(language = "C", compiler = CmCompiler(path = "/usr/bin/cc"))])
        index.objects |> push(CmObject(kind = "toolchains", jsonFile = "toolchains.json"))
        fwrite(path_join(reply, "toolchains.json"), sprint_json(tools, false))
    }
    fwrite(path_join(reply, "target-first.json"), sprint_json(first, false))
    fwrite(path_join(reply, "target-second.json"), sprint_json(second, false))
    fwrite(path_join(reply, "target-debug.json"), sprint_json(debug, false))
    fwrite(path_join(reply, "codemodel.json"), sprint_json(model, false))
    fwrite(path_join(reply, index_name), sprint_json(index, false))
}


def private with_build_dir(t : T?; blk : block<(build : string) : void>) {
    var err : string
    let build = create_temp_directory("das_code_model", err)
    if (empty(build)) {
        t |> failure("create_temp_directory: {err}")
        return
    }
    invoke(blk, build)
    rmdir_rec(build, err)
}


def private flags_of(var flags : table<string; array<string>>; build, file : string) : string {
    let key = cc_file_key(build, file)
    return flags |> key_exists(key) ? join(flags[key], " ") : "<none>"
}


[test(description = "a compile group's flags are its fragments without the precompiled-header switches, then its defines and include dirs, with the language standard added only when no fragment sets one")]
def test_cm_group_flags(t : T?) {
    t |> run("fragments, defines and includes in order, PCH switches dropped") @(t : T?) {
        let pch = "/YuC:/b/pch.hxx /FpC:/b/pch.pch /FIC:/b/pch.hxx"
        var g <- one_group("CXX", "/EHsc -MD /std:c++17 {pch}", "A=1", "C:/src/include", [0])
        g.compileCommandFragments |> push(CmFragment(fragment = "\"-DNAME=two words\""))
        g.languageStandard.standard = "20"
        t |> equal(join(cm_group_flags(g, true), "|"),
            "/EHsc|-MD|/std:c++17|/FIC:/b/pch.hxx|-DNAME=two words|-DA=1|-IC:/src/include")
    }
    t |> run("a standard no fragment carries is added in the compiler's spelling") @(t : T?) {
        var g <- one_group("CXX", "-fno-rtti", "A=1", "/src/include", [0])
        g.languageStandard.standard = "17"
        t |> equal(join(cm_group_flags(g, false), "|"), "-fno-rtti|-std=c++17|-DA=1|-I/src/include")
        t |> equal(join(cm_group_flags(g, true), "|"), "-fno-rtti|/std:c++17|-DA=1|-I/src/include")
    }
    t |> run("a gnu standard in a fragment is kept as written") @(t : T?) {
        var g <- one_group("CXX", "-std=gnu++20", "A=1", "/src/include", [0])
        g.languageStandard.standard = "20"
        t |> equal(join(cm_group_flags(g, false), "|"), "-std=gnu++20|-DA=1|-I/src/include")
    }
    t |> run("no standard anywhere adds none") @(t : T?) {
        let g <- one_group("CXX", "-fno-rtti", "A=1", "/src/include", [0])
        t |> equal(join(cm_group_flags(g, false), "|"), "-fno-rtti|-DA=1|-I/src/include")
    }
}


[test(description = "the newest index of a file-API reply names the code model and the toolchains, and a tree without a reply names neither")]
def test_code_model_reply(t : T?) {
    t |> run("a tree without a reply names neither file") @(t : T?) {
        with_build_dir(t) $(build) {
            let reply = code_model_reply(build)
            t |> equal(reply.codemodel_file, "")
            t |> equal(reply.toolchains_file, "")
        }
    }
    t |> run("the newest index wins, and a kind it lacks stays empty") @(t : T?) {
        with_build_dir(t) $(build) {
            write_reply(t, build, "index-2026-01-01T00-00-00-0000.json", "cl.exe")
            write_reply(t, build, "index-2026-02-01T00-00-00-0000.json", "")
            let reply = code_model_reply(build)
            t |> equal(reply.codemodel_file, "codemodel.json")
            t |> equal(reply.toolchains_file, "")
        }
    }
    t |> run("an index that is not JSON names neither file") @(t : T?) {
        with_build_dir(t) $(build) {
            write_reply(t, build, "index-2026-01-01T00-00-00-0000.json", "cl.exe")
            fwrite(path_join(reply_dir(build), "index-2026-03-01T00-00-00-0000.json"), "not json")
            t |> equal(code_model_reply(build).codemodel_file, "")
        }
    }
}


[test(description = "the code model gives each C++ source of the chosen configuration the first owning target's flags, and nothing when the tree's compiler takes another flag spelling")]
def test_load_code_model(t : T?) {
    t |> run("the named configuration, C++ groups only, the first target of several") @(t : T?) {
        with_build_dir(t) $(build) {
            write_reply(t, build, "index-2026-01-01T00-00-00-0000.json", "C:/vs/bin/cl.exe")
            var flags <- load_code_model(build, "Release", true)
            t |> equal(length(flags), 3)
            t |> equal(flags_of(flags, build, "src/a.cpp"), "/EHsc /fp:fast -DFIRST=1 -Iinc/first")
            t |> equal(flags_of(flags, build, "src/b.cpp"), "/EHsc -DSECOND=1 -Iinc/second")
            t |> equal(flags_of(flags, build, "gen/g.cpp"), "/EHsc /fp:fast -DFIRST=1 -Iinc/first",
                "a source with an absolute path")
            t |> equal(flags_of(flags, build, "src/c.c"), "<none>")
        }
    }
    t |> run("a configuration the tree lacks reads the first one") @(t : T?) {
        with_build_dir(t) $(build) {
            write_reply(t, build, "index-2026-01-01T00-00-00-0000.json", "C:/vs/bin/cl.exe")
            var flags <- load_code_model(build, "RelWithDebInfo", true)
            t |> equal(flags_of(flags, build, "src/a.cpp"), "/Od -DDEBUG_TREE -Iinc/debug")
        }
    }
    t |> run("a compiler of the other flag spelling gives nothing") @(t : T?) {
        with_build_dir(t) $(build) {
            write_reply(t, build, "index-2026-01-01T00-00-00-0000.json", "C:/vs/bin/cl.exe")
            t |> equal(length(load_code_model(build, "Release", false)), 0)
            t |> success(cc_is_cl("clang-cl") && cc_is_cl("C:/vs/bin/cl.exe"), "the cl spellings")
            t |> success(cc_is_cl("C:/VS/BIN/CL.EXE"), "case does not matter")
            t |> success(!cc_is_cl("/usr/bin/clang++"), "a gnu driver")
            t |> success(!cc_is_cl("/usr/bin/oracl"), "a name that only ends in cl")
        }
    }
    t |> run("a reply without the toolchains gives nothing") @(t : T?) {
        with_build_dir(t) $(build) {
            write_reply(t, build, "index-2026-01-01T00-00-00-0000.json", "")
            t |> equal(length(load_code_model(build, "Release", true)), 0)
        }
    }
    t |> run("toolchains without a C++ compiler give nothing") @(t : T?) {
        with_build_dir(t) $(build) {
            write_reply(t, build, "index-2026-01-01T00-00-00-0000.json", "/usr/bin/c++")
            let c_only <- CmToolchains(toolchains <- [
                CmToolchain(language = "C", compiler = CmCompiler(path = "/usr/bin/cc"))])
            fwrite(path_join(reply_dir(build), "toolchains.json"), sprint_json(c_only, false))
            t |> equal(length(load_code_model(build, "Release", false)), 0)
        }
    }
    t |> run("a code model with no configuration gives nothing") @(t : T?) {
        with_build_dir(t) $(build) {
            write_reply(t, build, "index-2026-01-01T00-00-00-0000.json", "C:/vs/bin/cl.exe")
            let bare = CmCodeModel(paths = CmPaths(source = build))
            fwrite(path_join(reply_dir(build), "codemodel.json"), sprint_json(bare, false))
            t |> equal(length(load_code_model(build, "Release", true)), 0)
        }
    }
    t |> run("a target file that is gone is passed over") @(t : T?) {
        with_build_dir(t) $(build) {
            write_reply(t, build, "index-2026-01-01T00-00-00-0000.json", "C:/vs/bin/cl.exe")
            remove(path_join(reply_dir(build), "target-first.json"))
            var flags <- load_code_model(build, "Release", true)
            t |> equal(flags_of(flags, build, "src/a.cpp"), "/EHsc -DSECOND=1 -Iinc/second")
        }
    }
}


[test(description = "a build tree's flags come from compile_commands.json when it is there, else from the code model, and a tree with a full reply is not configured")]
def test_load_build_flags(t : T?) {
    t |> run("no build tree gives no flags and no source") @(t : T?) {
        var configures = 0
        let built <- load_build_flags("", "Release", true) $ {
            configures++
            return 0
        }
        t |> equal(built.origin, "")
        t |> equal(length(built.flags), 0)
        t |> equal(configures, 0)
    }
    t |> run("compile_commands.json wins and nothing is configured") @(t : T?) {
        with_build_dir(t) $(build) {
            let file = path_join(build, "src/a.cpp")
            let command = "clang-cl /EHsc -DDB=1 /c \"{file}\""
            let entry = CcEntry(directory = build, command = command, file = file)
            fwrite(path_join(build, "compile_commands.json"), sprint_json([entry], false))
            var configures = 0
            var built <- load_build_flags(build, "Release", true) $ {
                configures++
                return 0
            }
            t |> equal(built.origin, "compile_commands")
            t |> equal(flags_of(built.flags, build, "src/a.cpp"), "/EHsc -DDB=1")
            t |> equal(configures, 0)
            let query = path_join(build, ".cmake/api/v1/query/client-preflight")
            t |> success(!fexist(path_join(query, "codemodel-v2")), "no query is written")
        }
    }
    t |> run("a tree with no reply gets the query and one configure") @(t : T?) {
        with_build_dir(t) $(build) {
            var configures = 0
            var built <- load_build_flags(build, "Release", true) $ {
                configures++
                let query = path_join(build, ".cmake/api/v1/query/client-preflight")
                t |> success(fexist(path_join(query, "codemodel-v2")), "the code model query")
                t |> success(fexist(path_join(query, "toolchains-v1")), "the toolchains query")
                write_reply(t, build, "index-2026-01-01T00-00-00-0000.json", "C:/vs/bin/cl.exe")
                return 0
            }
            t |> equal(configures, 1)
            t |> equal(built.configure_error, "")
            t |> equal(built.origin, "the CMake code model")
            t |> equal(flags_of(built.flags, build, "src/b.cpp"), "/EHsc -DSECOND=1 -Iinc/second")
        }
    }
    t |> run("a tree with a reply is read as it is") @(t : T?) {
        with_build_dir(t) $(build) {
            write_reply(t, build, "index-2026-01-01T00-00-00-0000.json", "C:/vs/bin/cl.exe")
            var configures = 0
            let built <- load_build_flags(build, "Release", true) $ {
                configures++
                return 0
            }
            t |> equal(configures, 0)
            t |> equal(built.configure_error, "")
            t |> equal(length(built.flags), 3)
        }
    }
}


[test(description = "the configure step runs again for a reply without the toolchains, its failure is named with the exit code, and nothing is configured when the query cannot be written")]
def test_load_build_flags_configure(t : T?) {
    t |> run("a reply without the toolchains is asked for again") @(t : T?) {
        with_build_dir(t) $(build) {
            write_reply(t, build, "index-2026-01-01T00-00-00-0000.json", "")
            var configures = 0
            let built <- load_build_flags(build, "Release", true) $ {
                configures++
                return 0
            }
            t |> equal(configures, 1)
            t |> equal(length(built.flags), 0)
        }
    }
    t |> run("a configure that fails is named with its exit code") @(t : T?) {
        with_build_dir(t) $(build) {
            let built <- load_build_flags(build, "Release", true) $ {
                return 3
            }
            let said = "configuring {build} for its code model failed (rc=3)"
            t |> equal(built.configure_error, said)
            t |> equal(length(built.flags), 0)
        }
    }
    t |> run("a query that cannot be written configures nothing") @(t : T?) {
        with_build_dir(t) $(build) {
            fwrite(path_join(build, ".cmake"), "a file where the folder should be")
            var configures = 0
            let built <- load_build_flags(build, "Release", true) $ {
                configures++
                return 0
            }
            t |> equal(configures, 0)
            t |> equal(built.configure_error, "cannot write the code model query under {build}")
        }
    }
}

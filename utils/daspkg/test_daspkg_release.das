options gen2
options no_global_variables = false

require dastest/testing_boost public

require daslib/fio
require daslib/clargs
require daslib/defer
require strings
require daslib/strings_boost

require daslib/json_boost


require utils
require commands
require daslib/result
require package_runner public
require _daspkg_test_common

// On Darwin daspkg release emits a clickable `.app`; everything else is the
// flat `<out_dir>/<name>/...` layout. These helpers let release tests assert
// against the right paths without per-platform branches at every check site.

def bundle_root_dir(out_dir, name : string) : string {
    if (get_platform_name() == "darwin") {
        return path_join(out_dir, "{name}.app")
    }
    return path_join(out_dir, name)
}

def bundle_artifacts_dir(out_dir, name : string) : string {
    if (get_platform_name() == "darwin") {
        return path_join(path_join(out_dir, "{name}.app"), "Contents/MacOS")
    }
    return path_join(out_dir, name)
}

def bundle_exe_path(out_dir, name : string) : string {
    let dir = bundle_artifacts_dir(out_dir, name)
    if (get_platform_name() == "darwin") {
        return path_join(dir, name)
    }
    return path_join(dir, "{name}.exe")
}

// Path of the exe AFTER the bundle root has been moved/renamed to `moved_root`.
// On Darwin the moved directory IS the .app — the exe still lives at
// Contents/MacOS/<name>. On Linux/Windows it's just <moved_root>/<name>.exe.
def moved_bundle_exe_path(moved_root, name : string) : string {
    if (get_platform_name() == "darwin") {
        return path_join(moved_root, "Contents/MacOS/{name}")
    }
    return path_join(moved_root, "{name}.exe")
}

[test(description = "ship_tools finds a tool in bin/ then bin/Release/, fails for a missing tool and refuses a name with a path separator")]
def test_ship_tools(t : T?) {
    t |> run("a tool ships by bare name from bin/, then bin/Release/, with the platform's exe suffix; a missing tool fails the release") @(t : T?) {
        let tmp = create_temp_directory_result("daspkg tools ")
        if (!(tmp is value)) {
            t |> failure("could not create temp directory: {tmp as error}")
            return
        }
        let root = tmp as value
        let das_root = path_join(root, "root")
        let bundle = path_join(root, "bundle")
        let ext = get_platform_name() == "windows" ? ".exe" : ""
        mkdir(das_root)
        mkdir(path_join(das_root, "bin"))
        mkdir(path_join(path_join(das_root, "bin"), "Release"))
        mkdir(bundle)
        fwrite(path_join(das_root, "bin/flat{ext}"), "flat")
        fwrite(path_join(das_root, "bin/Release/msvc{ext}"), "msvc")
        var shipped : array<string>
        t |> equal(ship_tools("pkg", bundle, ["flat", "msvc"], shipped, das_root), 0, "both tools resolve")
        t |> equal(length(shipped), 2)
        t |> success(fexist(path_join(bundle, "flat{ext}")), "bin/<tool> lands at the bundle root")
        t |> success(fexist(path_join(bundle, "msvc{ext}")), "bin/Release/<tool> lands at the bundle root")
        var none : array<string>
        t |> equal(ship_tools("pkg", bundle, ["absent"], none, das_root), 1, "a tool no build produced fails the release")
        t |> equal(length(none), 0)
        fwrite(path_join(root, "escaped{ext}"), "outside")
        var unsafe_name : array<string>
        t |> equal(ship_tools("pkg", bundle, ["../escaped"], unsafe_name, das_root), 1, "a name with a path separator is refused before any path is built")
        t |> success(!fexist(path_join(bundle, "escaped{ext}")), "nothing was copied for the refused name")
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test(description = "cmd_release bundles a pure-daslang project end to end, and --paranoid with --quick warns instead of refusing", timeout = 1200)]
def test_cmd_release_pure_daslang(t : T?) {
    t |> run("bundles a pure-daslang project end-to-end") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_pure"
        let out_dir = "{get_das_root()}/_test_release_pure_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(tmp_root)

        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\n[export]\ndef main() \{\n    print(\"hello\\n\")\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"smoke\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n    release_include(\"*.txt\")\n    release_include_if_missing(\"config.toml\")\n\}\n")
        }
        fopen("{tmp_root}/data.txt", "wb") $(f) {
            fwrite(f, "asset")
        }
        fopen("{tmp_root}/config.toml", "wb") $(f) {
            fwrite(f, "value = \"template\"\n")
        }

        let rc = cmd_release(tmp_root, out_dir)
        tt |> equal(0, rc, "cmd_release returns 0")
        let exe_path = bundle_exe_path(out_dir, "smoke")
        let artifacts = bundle_artifacts_dir(out_dir, "smoke")
        tt |> success(fexist(exe_path), "exe shipped at {exe_path}")
        tt |> success(fexist(path_join(artifacts, "data.txt")), "asset shipped")
        let deployed_config = path_join(artifacts, "config.toml")
        tt |> equal("value = \"template\"\n", fread(deployed_config), "starter config shipped on first release")
        tt |> success(!fexist(path_join(artifacts, "smoke.exe.o")), ".o build artifact dropped")

        fopen(deployed_config, "wb") $(f) {
            fwrite(f, "value = \"user-edited\"\n")
        }
        // Simulate upgrading from the old release_include("config.toml")
        // behavior: the previous manifest still claims ownership. The new
        // if-missing declaration must protect the deployed edit during that
        // one-time migration cleanup.
        let old_manifest = path_join(artifacts, ".daspkg_release.manifest")
        fopen(old_manifest, "ab") $(f) {
            fwrite(f, "config.toml\n")
        }
        let rc2 = cmd_release(tmp_root, out_dir)
        tt |> equal(0, rc2, "second cmd_release returns 0")
        tt |> equal("value = \"user-edited\"\n", fread(deployed_config),
            "release_include_if_missing preserves deployed edits")
        let manifest = fread(old_manifest)
        tt |> success(find(manifest, "config.toml") < 0,
            "user-owned starter config is omitted from release manifest")

        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
    t |> run("paranoid with quick warns and releases (no refusal)") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_pq"
        let out_dir = "{get_das_root()}/_test_release_pq_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(tmp_root)
        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\n[export]\ndef main() \{\n    print(\"hello\\n\")\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"smokepq\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n\}\n")
        }
        let rc = cmd_release(tmp_root, out_dir, true, true)
        tt |> equal(0, rc, "the pair is a warning, not a refusal")
        tt |> success(fexist(bundle_exe_path(out_dir, "smokepq")), "exe shipped")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
}

// the tool release_launcher names must be a real build product: the watchdog the tree builds
// where dasHV, dasStdDlg and dasStbImage are in
def private built_watchdog_path() : string {
    let ext = get_platform_name() == "windows" ? ".exe" : ""
    for (rel in ["bin/watchdog{ext}", "bin/Release/watchdog{ext}"]) {
        let candidate = path_join(get_das_root(), rel)
        if (fexist(candidate)) return candidate
    }
    return ""
}

[test(description = "release_launcher names the tool the macOS app opens and refuses an unshipped tool before building, and without one the app opens the exe itself", timeout = 1200)]
def test_cmd_release_launcher(t : T?) {
    t |> run("release_launcher names the shipped tool the macOS .app opens; a tool not shipped refuses before the build") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_launcher"
        let out_dir = "{get_das_root()}/_test_release_launcher_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(tmp_root)
        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\n[export]\ndef main() \{\n    print(\"hello\\n\")\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"launched\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n    release_launcher(\"watchdog\")\n\}\n")
        }
        let refused = cmd_release(tmp_root, out_dir)
        tt |> equal(1, refused, "a launcher no release_include_tool ships is refused")
        tt |> success(!fexist(bundle_exe_path(out_dir, "launched")), "and nothing was built")
        if (empty(built_watchdog_path())) {
            force_rmdir(tmp_root)
            force_rmdir(out_dir)
            tt |> skip("no built watchdog in this tree - the shipped-tool half of the launcher test needs bin/watchdog")
            return
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"launched\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n    release_include_tool(\"watchdog\")\n    release_launcher(\"watchdog\")\n\}\n")
        }
        let rc = cmd_release(tmp_root, out_dir)
        tt |> equal(0, rc, "cmd_release returns 0 with the tool shipped")
        let artifacts = bundle_artifacts_dir(out_dir, "launched")
        let ext = get_platform_name() == "windows" ? ".exe" : ""
        tt |> success(fexist(path_join(artifacts, "watchdog{ext}")), "the launcher tool sits beside the exe")
        tt |> success(fexist(bundle_exe_path(out_dir, "launched")), "the exe itself still ships")
        if (get_platform_name() == "darwin") {
            let plist = fread(path_join(bundle_root_dir(out_dir, "launched"), "Contents/Info.plist"))
            let key_at = find(plist, "<key>CFBundleExecutable</key>")
            tt |> success(key_at >= 0, "the plist names an executable")
            let value_at = find(plist, "<string>", key_at)
            tt |> success(value_at >= 0 && starts_with(slice(plist, value_at, length(plist)), "<string>watchdog</string>"),
                "CFBundleExecutable is the launcher, so a double click on the .app starts the supervisor:\n{plist}")
            tt |> success(find(plist, "<key>LSUIElement</key>") >= 0, "a launcher app is menu-bar-only - no Dock tile, no application menu:\n{plist}")
        }
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
    t |> run("without a launcher the .app opens the exe itself and is an ordinary application") @(tt : T?) {
        if (get_platform_name() != "darwin") {
            tt |> skip("the Info.plist exists only in the macOS .app layout")
            return
        }
        let tmp_root = "{get_das_root()}/_test_release_nolauncher"
        let out_dir = "{get_das_root()}/_test_release_nolauncher_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(tmp_root)
        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\n[export]\ndef main() \{\n    print(\"hello\\n\")\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"plainapp\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n\}\n")
        }
        tt |> equal(0, cmd_release(tmp_root, out_dir), "cmd_release returns 0")
        let plist = fread(path_join(bundle_root_dir(out_dir, "plainapp"), "Contents/Info.plist"))
        tt |> success(find(plist, "<string>plainapp</string>") >= 0, "CFBundleExecutable is the exe:\n{plist}")
        tt |> success(find(plist, "LSUIElement") < 0, "and it is not marked menu-bar-only")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
}

[test(description = "release_program bakes a companion exe beside the main one, lists it in the manifest and shares the runtime", timeout = 1200)]
def test_cmd_release_program(t : T?) {
    t |> run("release_program bakes a companion exe beside the main one, in the manifest, sharing the runtime") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_program"
        let out_dir = "{get_das_root()}/_test_release_program_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(tmp_root)
        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\n[export]\ndef main() \{\n    print(\"hello\\n\")\n\}\n")
        }
        // the companion alone requires a shared module (stbimage): the bundle must ship it for the
        // companion's sake, where the main program never asked for it
        let stb_module = path_join(get_das_root(), "modules/dasStbImage/dasModuleStbImage.shared_module")
        let stb_built = fexist(stb_module)
        let tool_require = stb_built ? "require stbimage\n" : ""
        fopen("{tmp_root}/tool.das", "wb") $(f) {
            fwrite(f, "options gen2\n{tool_require}[export]\ndef main() \{\n    print(\"companion\\n\")\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"withtool\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n    release_program(\"side-tool\", \"tool.das\")\n\}\n")
        }
        tt |> equal(0, cmd_release(tmp_root, out_dir), "cmd_release returns 0 with a companion")
        let artifacts = bundle_artifacts_dir(out_dir, "withtool")
        let companion_rel = get_platform_name() == "darwin" ? "side-tool" : "side-tool.exe"
        tt |> success(fexist(bundle_exe_path(out_dir, "withtool")), "the main exe ships")
        tt |> success(fexist(path_join(artifacts, companion_rel)), "the companion exe sits beside it as {companion_rel}")
        tt |> success(!fexist(path_join(artifacts, "side-tool.exe.o")),
            "the companion's object file is dropped")
        for (stem in ["withtool", "side-tool"]) {
            for (suffix in [".map", ".lib", ".exp"]) {
                tt |> success(!fexist(path_join(artifacts, "{stem}{suffix}")), "no linker residue {stem}{suffix} rides beside the exe: nothing at run time reads it")
            }
        }
        let manifest = fread(path_join(artifacts, ".daspkg_release.manifest"))
        tt |> success(find(manifest, companion_rel) >= 0, "the manifest records the companion, so a re-release cleans it:\n{manifest}")
        if (stb_built) {
            tt |> success(fexist(path_join(artifacts, "modules/dasStbImage/dasModuleStbImage.shared_module")), "the shared module only the companion requires ships")
        } else {
            tt |> success(true, "not covered here: the companion-only shared module needs a built dasStbImage")
        }
        tt |> success(fexist(path_join(artifacts, "side-tool.tune.json")) == fexist(path_join(artifacts, "withtool.tune.json")), "the companion carries the sidecar exactly when the main exe does")
        var runtime_names : table<string>
        var runtime_dupes = 0
        fio::dir(artifacts) $(name) {
            if (find(name, "libDaScriptDyn") >= 0) {
                runtime_dupes += key_exists(runtime_names, name) ? 1 : 0
                runtime_names |> insert(name)
            }
        }
        tt |> success(!empty(runtime_names) && runtime_dupes == 0, "one set of runtime libraries serves both exes, none twice")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
}

[test(description = "a companion that fails to build leaves nothing behind, and an unsafe, clashing or missing companion is refused before the build", timeout = 1200)]
def test_cmd_release_program_refusals(t : T?) {
    t |> run("a companion that fails to build ends the release with nothing of it left behind") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_program_broken"
        let out_dir = "{get_das_root()}/_test_release_program_broken_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(tmp_root)
        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\n[export]\ndef main() \{\n    print(\"hello\\n\")\n\}\n")
        }
        fopen("{tmp_root}/tool.das", "wb") $(f) {
            fwrite(f, "options gen2\n[export]\ndef main() \{\n    this is not daslang\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"withbroken\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n    release_program(\"side-tool\", \"tool.das\")\n\}\n")
        }
        tt |> equal(1, cmd_release(tmp_root, out_dir), "a companion that does not compile fails the release")
        let artifacts = bundle_artifacts_dir(out_dir, "withbroken")
        let left_exe = (fexist(path_join(artifacts, "side-tool.exe"))
            || fexist(path_join(artifacts, "side-tool")))
        let left_obj = fexist(path_join(artifacts, "side-tool.exe.o"))
        tt |> success(!left_exe && !left_obj, "no half-built companion stays in the bundle")
        var leftover = 0
        fio::dir(get_das_root()) $(name) {
            if (name |> starts_with("_daspkg_release_deps_")) {
                leftover ++
            }
        }
        tt |> equal(0, leftover, "no deps JSON is left in the tree")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
    t |> run("a companion with an unsafe name, the bundle's own name, a shipped tool's name, a twin's name, or a missing script is refused before the build") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_program_bad"
        let out_dir = "{get_das_root()}/_test_release_program_bad_out"
        for (bad in ["release_program(\"bad name\", \"main.das\")", "release_program(\"withtool\", \"main.das\")", "release_program(\"side-tool\", \"absent.das\")",
                     "release_program(\"\", \"main.das\")", "release_program(\"side-tool.exe\", \"main.das\")", "release_program(\"a..b\", \"main.das\")",
                     "release_program(\"side-tool\", \"main.das; rm -rf x\")",
                     "release_include_tool(\"watchdog\")\n    release_program(\"watchdog\", \"main.das\")",
                     "release_program(\"side-tool\", \"main.das\")\n    release_program(\"side-tool\", \"main.das\")"]) {
            force_rmdir(tmp_root)
            force_rmdir(out_dir)
            mkdir(tmp_root)
            fopen("{tmp_root}/main.das", "wb") $(f) {
                fwrite(f, "options gen2\n[export]\ndef main() \{\n    print(\"hello\\n\")\n\}\n")
            }
            fopen("{tmp_root}/.das_package", "wb") $(f) {
                fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"withtool\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n    {bad}\n\}\n")
            }
            tt |> equal(1, cmd_release(tmp_root, out_dir), "refused: {bad}")
            tt |> success(!fexist(bundle_exe_path(out_dir, "withtool")), "and nothing was built for {bad}")
        }
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
}

[test(description = "release_requires_jit refuses the release before anything builds", timeout = 1200)]
def test_cmd_release_refuses_jit_only(t : T?) {
    t |> run("release_requires_jit() refuses the release before anything builds") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_jitonly"
        let out_dir = "{get_das_root()}/_test_release_jitonly_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(tmp_root)
        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\n[export]\ndef main() \{\n    print(\"hello\\n\")\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"jitonly\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n    release_requires_jit()\n\}\n")
        }
        tt |> equal(1, cmd_release(tmp_root, out_dir), "a JIT-only package is refused")
        tt |> success(!fexist(bundle_exe_path(out_dir, "jitonly")), "and nothing was built")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
}

[test(description = "cmd_release refuses a bundle name that could escape the output directory", timeout = 1200)]
def test_cmd_release_refuses_unsafe_bundle_name(t : T?) {
    // the bundle name is a .das_package string that reaches path_join, force_rmdir and shell
    // command lines; the guard admits letters, digits, `_`, `-` and `.` only - a space or a
    // shell metacharacter is refused before anything is built or removed
    for (bad in ["bad name", "bad;name", "bad$(name)", "bad\"name"]) {
        t |> run("refuses bundle name `{bad}`") @(tt : T?) {
            let tmp_root = "{get_das_root()}/_test_release_badname"
            let out_dir = "{get_das_root()}/_test_release_badname_out"
            force_rmdir(tmp_root)
            force_rmdir(out_dir)
            mkdir(tmp_root)
            fopen("{tmp_root}/main.das", "wb") $(f) {
                fwrite(f, "options gen2\n[export]\ndef main() \{\n    print(\"hello\\n\")\n\}\n")
            }
            let escaped = replace(replace(bad, "\\", "\\\\"), "\"", "\\\"")
            fopen("{tmp_root}/.das_package", "wb") $(f) {
                fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"{escaped}\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n\}\n")
            }
            let rc = cmd_release(tmp_root, out_dir)
            tt |> equal(1, rc, "cmd_release refuses the name")
            tt |> success(!fexist(out_dir), "nothing was built or removed under {out_dir}")
            force_rmdir(tmp_root)
            force_rmdir(out_dir)
        }
    }
}

// a tune-scope release fixture whose tuner is a STUB: a real tuner
// races candidates for minutes under the noise gate; the stub copies
// a canned sidecar into DAS_TUNE_MANIFEST, so the mint and ship paths
// test at unit speed
def private write_tunestub_fixture(tmp_root : string) {
    mkdir(tmp_root)
    let payload = ("\{\"kernels\":\{\"stub_kernel\":\{\"suffix\":\"stub\"\}\}," +
        "\"provenance\":\{\"mode\":\"stub\"\}\}")
    fopen("{tmp_root}/stub_payload.json", "wb") $(f) {
        fwrite(f, payload)
    }
    fopen("{tmp_root}/main.das", "wb") $(f) {
        fwrite(f, "options gen2\nrequire daslib/tune\nrequire llvm/daslib/llvm_tune\n" +
            "[tune_scope(name=\"stubscope\", tuner=\"stub_tuner.das\")]\n" +
            "struct StubScope \{\n    unused : int\n\}\n" +
            "[export]\ndef main() \{\n    print(\"hi\\n\")\n\}\n")
    }
    fopen("{tmp_root}/stub_tuner.das", "wb") $(f) {
        fwrite(f, "options gen2\nrequire daslib/fio\n[export]\ndef main() \{\n" +
            "    fopen(get_env_variable(\"DAS_TUNE_MANIFEST\"), \"wb\") $(mf) \{\n" +
            "        fwrite(mf, fread(\"{tmp_root}/stub_payload.json\"))\n    \}\n\}\n")
    }
    fopen("{tmp_root}/.das_package", "wb") $(f) {
        fwrite(f, "options gen2\nrequire daslib/daspkg\n" +
            "[export]\ndef package() \{\n    package_name(\"tunestub\")\n\}\n" +
            "[export]\ndef release() \{\n    release_main(\"main.das\")\n\}\n")
    }
}

[test(description = "a tune scope ships its resolved manifest beside the exe, and an exported DAS_TUNE_MANIFEST takes neither the mint nor the ship", timeout = 1200)]
def test_cmd_release_ships_minted_scope_manifest(t : T?) {
    t |> run("a tune scope's RESOLVED manifest ships beside the exe") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_tunescope"
        let out_dir = "{get_das_root()}/_test_release_tunescope_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        write_tunestub_fixture(tmp_root)
        let rc = cmd_release(tmp_root, out_dir)
        tt |> equal(0, rc, "cmd_release returns 0")
        // the mint ran: the stub wrote the resolved manifest beside main.das
        let src_manifest = "{tmp_root}/main.tune.json"
        tt |> success(fexist(src_manifest), "the scope's manifest was minted at {src_manifest}")
        // the ship: <bundle>/<name>.tune.json is that SAME manifest, byte-for-byte — shipping
        // a next-to-root guess instead put a stale July sidecar beside a fresh M4 exe once
        let bundle_dir = dir_name(bundle_exe_path(out_dir, "tunestub"))
        let shipped = path_join(bundle_dir, "tunestub.tune.json")
        tt |> success(fexist(shipped), "the bundle ships a tune sidecar at {shipped}")
        tt |> equal(fread(src_manifest), fread(shipped), "shipped sidecar == the resolved manifest")
        tt |> success(find(fread(shipped), "stub_kernel") >= 0, "the shipped winners are the stub mint's")
        // the ship contract: STRICTLY newer than the exe, or the runtime staleness rule drops
        // every kernel to its fallback and the artifact cannot name its winners
        var exe_fs, side_fs : FStat
        tt |> success(stat(bundle_exe_path(out_dir, "tunestub"), exe_fs), "exe stats")
        tt |> success(stat(shipped, side_fs), "shipped sidecar stats")
        tt |> success(int64(side_fs.mtime) > int64(exe_fs.mtime), "shipped sidecar mtime is strictly newer than the exe")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
    t |> run("an exported DAS_TUNE_MANIFEST takes neither the mint nor the ship") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_tunescope_env"
        let out_dir = "{get_das_root()}/_test_release_tunescope_env_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        write_tunestub_fixture(tmp_root)
        let elsewhere = "{tmp_root}/elsewhere.tune.json"
        set_env_variable("DAS_TUNE_MANIFEST", elsewhere)
        let rc = cmd_release(tmp_root, out_dir)
        set_env_variable("DAS_TUNE_MANIFEST", "")
        tt |> equal(0, rc, "cmd_release returns 0")
        tt |> success(!fexist(elsewhere), "the redirect never received the mint")
        let src_manifest = "{tmp_root}/main.tune.json"
        tt |> success(fexist(src_manifest), "the scope's manifest was minted beside the app")
        let bundle_dir = dir_name(bundle_exe_path(out_dir, "tunestub"))
        let shipped = path_join(bundle_dir, "tunestub.tune.json")
        tt |> success(fexist(shipped) && find(fread(shipped), "stub_kernel") >= 0,
            "the bundle ships the app's own mint")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
}

// a fat release's fixture: a scope shipping class profiles under ./defaults and one [tune]
// kernel over the add_plus_k selftest generator; `with_profile` decides whether the host's
// lowest class gets a profile entry for the kernel
def private write_fat_fixture(tmp_root : string; with_profile : bool) {
    mkdir(tmp_root)
    mkdir("{tmp_root}/defaults")
    let arch = get_architecture_name()
    let low = arch == "x86_64" ? "x86-avx2" : "arm-neon"
    let features = arch == "x86_64" ? "avx2;f16c;fma;sse4.2" : "neon;dotprod;fullfp16;lse"
    if (with_profile) {
        fwrite("{tmp_root}/defaults/{low}.tune-defaults.json",
            "\{ \"kernels\" : \{ \"fat_kernel\" : \"k2\" \}, \"provenance\" : \{ \"features\" : \"{features}\" \} \}")
    }
    fwrite("{tmp_root}/main.das", "options gen2\nrequire daslib/tune\nrequire llvm/daslib/llvm_tune\n[tune_scope(name=\"fatscope\", defaults=\"./defaults\")]\nstruct FatScope \{\n    unused : int\n\}\n[tune_perm(k = 0), tune_perm(k = 1), tune_perm(k = 2), tune(gen = \"llvm_code_selftest::add_plus_k\", fallback = \"k1\")]\ndef fat_kernel(a, b : int) : int \{\n    return a + b\n\}\n[export, tune_policy(missing = \"fallback\")]\ndef main() \{\n    print(\"R \{fat_kernel(length(get_command_line_arguments()), 3)}\\n\")\n\}\n")
    fwrite("{tmp_root}/.das_package", "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"fatstub\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n\}\n")
}

[test(description = "--fat ships the class profile clones and refuses a kernel without a profile entry or a bad class name before building", timeout = 1200)]
def test_cmd_release_fat(t : T?) {
    let arch = get_architecture_name()
    let low = arch == "x86_64" ? "x86-avx2" : "arm-neon"
    t |> run("--fat ships the class profile's clones, mints nothing, and ships no sidecar") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_fat"
        let out_dir = "{get_das_root()}/_test_release_fat_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        write_fat_fixture(tmp_root, true)
        let rc = cmd_release(tmp_root, out_dir, false, false, low)
        tt |> equal(0, rc, "cmd_release --fat {low} returns 0")
        tt |> success(!fexist("{tmp_root}/main.tune.json"), "a fat release mints no sidecar beside the script")
        let bundle_dir = dir_name(bundle_exe_path(out_dir, "fatstub"))
        tt |> success(!fexist(path_join(bundle_dir, "fatstub.tune.json")), "the bundle ships no tune sidecar")
        tt |> success(fexist(bundle_exe_path(out_dir, "fatstub")), "the fat exe is in the bundle")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
    t |> run("--fat refuses a kernel the baseline class has no profile entry for") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_fat_none"
        let out_dir = "{get_das_root()}/_test_release_fat_none_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        write_fat_fixture(tmp_root, false)
        let rc = cmd_release(tmp_root, out_dir, false, false, low)
        tt |> success(rc != 0, "a fallback-stamped baseline clone refuses the release")
        let bundle_dir = dir_name(bundle_exe_path(out_dir, "fatstub"))
        tt |> success(!fexist(path_join(bundle_dir, RELEASE_MANIFEST)), "the refused release never finishes its bundle")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
    t |> run("--fat refuses a class name that is not a name before anything builds") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_fat_unsafe"
        let out_dir = "{get_das_root()}/_test_release_fat_unsafe_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        write_fat_fixture(tmp_root, true)
        for (bad in ["x86-avx2 DAS_X=1", "x86-avx2;echo", "x86-avx2\"", "../x86-avx2"]) {
            tt |> success(cmd_release(tmp_root, out_dir, false, false, bad) != 0, "'{bad}' is refused")
        }
        tt |> success(!fexist(bundle_exe_path(out_dir, "fatstub")), "no exe was built for a refused class name")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
    t |> run("the gate names the kernel and the class, and passes a fully profiled scope") @(tt : T?) {
        let refusal = release_fat_refusal("fatscope", "fat_kernel;other_kernel", low)
        tt |> success(find(refusal, "scope `fatscope` has no {low} profile entry for fat_kernel, other_kernel") >= 0, "the refusal names the scope, the class and the kernels: {refusal}")
        tt |> equal(release_fat_refusal("fatscope", "", low), "", "a scope whose baseline clones all came from a profile passes")
    }
}

[test(description = "a release with a native dependency bundles it and runs from a moved location", timeout = 1200)]
def test_cmd_release_native_dep(t : T?) {
    var dasSQLITE_present = false
    for_each_registered_dynamic_module() $(_path, _mod_name, das_name) {
        if (das_name == "sqlite") {
            dasSQLITE_present = true
        }
    }
    if (!dasSQLITE_present) {
        t |> run("skipped — dasSQLITE not available") @(tt : T?) {
            to_log(LOG_INFO, "test_cmd_release_native_dep: dasSQLITE not loaded; skipping native-dep test\n")
            tt |> success(true)
        }
        return
    }
    t |> run("bundles a native-dep project, runnable from a moved location") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_native"
        let out_dir = "{get_das_root()}/_test_release_native_out"
        let moved_dir = "{get_das_root()}/_test_release_native_moved"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        force_rmdir(moved_dir)
        mkdir(tmp_root)

        // Tiny project: open in-memory sqlite, run a query, print row count.
        // Using stderr-free output so we can match it exactly against the run.
        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\nrequire sqlite/sqlite_boost\n[export]\ndef main() \{\n    let db = open_sqlite(\":memory:\")\n    db |> exec(\"CREATE TABLE t(x INT)\")\n    db |> exec(\"INSERT INTO t VALUES(42)\")\n    print(\"rows: \{db |> changes()\}\\n\")\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"sqs\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n\}\n")
        }

        let rc = cmd_release(tmp_root, out_dir)
        tt |> equal(0, rc, "cmd_release returns 0")
        let exe_path = bundle_exe_path(out_dir, "sqs")
        let artifacts = bundle_artifacts_dir(out_dir, "sqs")
        let bundle_root = bundle_root_dir(out_dir, "sqs")
        tt |> success(fexist(exe_path), "exe shipped at {exe_path}")
        tt |> success(fexist(path_join(artifacts, "modules/dasSQLITE/dasModuleSQLITE.shared_module")),
            "dylib shipped at expected relative path")

        // Move the whole bundle to a new location and run it. PR #2579's
        // exe-relative resolution is what makes this work — the dylib needs
        // to be found via <exe_dir>/modules/dasSQLITE/... at the new path.
        // Use `rename` (cross-platform via libc rename(2)/Win CRT) rather
        // than shell `cp -r` so the test runs on Windows CI; rename also
        // preserves the exe permission bit by definition.
        let moved = rename(bundle_root, moved_dir)
        tt |> success(moved, "bundle moved to new location")
        let moved_exe = moved_bundle_exe_path(moved_dir, "sqs")
        var output : string
        let run_rc = run_cmd("\"{moved_exe}\"", output)
        tt |> equal(0, run_rc, "moved bundle runs cleanly")
        tt |> success(find(output, "rows: 1") >= 0, "moved bundle prints expected sqlite output (got: {output})")

        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        force_rmdir(moved_dir)
    }
}

// release_include_dll() declares a native shared library to ship next to the
// owning .shared_module. Verify the negative path: a project that declares a
// nonexistent DLL must fail loudly, not silently produce an incomplete bundle.
[test(description = "cmd_release fails when release_include_dll names a missing file", timeout = 1200)]
def test_cmd_release_include_dll_missing_fails(t : T?) {
    t |> run("cmd_release returns non-zero when release_include_dll names a missing file") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_include_dll_missing"
        let out_dir = "{get_das_root()}/_test_release_include_dll_missing_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(tmp_root)

        // Empty main, but the project declares a native dep that can't be
        // resolved under <das_root>/{bin,lib}/. ship_native_dlls runs after
        // the deps copy and must abort the release.
        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\n[export]\ndef main() \{\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"missdll\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n    release_include_dll(\"this_file_does_not_exist_anywhere_4f8c.dll\")\n\}\n")
        }

        let rc = cmd_release(tmp_root, out_dir)
        tt |> success(rc != 0, "cmd_release rejects nonexistent native dep (got rc={rc})")

        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
}

// False-positive test: every registered dynamic module that the program does NOT
// require must NOT show up in the bundle. Skips when the registry has only
// dasSQLITE (no other module to verify isn't shipped).
[test(description = "a release ships only the native modules the program requires, not registered but unused ones", timeout = 1200)]
def test_cmd_release_no_false_positive_shared(t : T?) {
    var dasSQLITE_present = false
    var other_pkg_dir = ""
    for_each_registered_dynamic_module() $(path, _mod_name, das_name) {
        if (das_name == "sqlite") {
            dasSQLITE_present = true
        } elif (empty(other_pkg_dir)) {
            // First non-sqlite registered module: extract package dir name.
            let g = to_generic_path(path)
            let idx = rfind(g, "/modules/")
            if (idx >= 0) {
                let after = slice(g, idx + length("/modules/"))
                let next_slash = find(after, "/")
                other_pkg_dir = next_slash > 0 ? slice(after, 0, next_slash) : after
            }
        }
    }
    if (!dasSQLITE_present || empty(other_pkg_dir)) {
        t |> run("skipped — needs dasSQLITE plus at least one other registered module") @(tt : T?) {
            to_log(LOG_INFO, "test_cmd_release_no_false_positive_shared: insufficient registered modules; skipping\n")
            tt |> success(true)
        }
        return
    }
    let other_for_capture = other_pkg_dir
    t |> run("ships only modules the program requires; not registered-but-unused dylibs") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_fp_shared"
        let out_dir = "{get_das_root()}/_test_release_fp_shared_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(tmp_root)

        // Project requires only sqlite — no other registered modules.
        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\nrequire sqlite/sqlite_boost\n[export]\ndef main() \{\n    let db = open_sqlite(\":memory:\")\n    print(\"db opened\\n\")\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"fpshared\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n\}\n")
        }

        let rc = cmd_release(tmp_root, out_dir)
        tt |> equal(0, rc, "cmd_release returns 0")
        let artifacts = bundle_artifacts_dir(out_dir, "fpshared")
        tt |> success(fexist(path_join(artifacts, "modules/dasSQLITE/dasModuleSQLITE.shared_module")),
            "dasSQLITE dylib shipped (program required it)")
        // Pick one registered-but-unused package and confirm it's NOT shipped.
        let other_dir = path_join(artifacts, "modules/{other_for_capture}")
        tt |> equal(false, fexist(other_dir),
            "registered-but-unused package must NOT be shipped (false positive)")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
}

// False-positive test: a project with no daspkg deps must NOT pull anything from
// daslib stdlib (`require strings`, `require fio`) or from the project's own
// non-main .das files into the bundle's modules/ tree. Only the exe + the
// project's own assets (release_include) should be shipped.
[test(description = "daslib and project-local .das files do not create modules entries in the bundle", timeout = 1200)]
def test_cmd_release_no_false_positive_das(t : T?) {
    t |> run("daslib stdlib + project-local .das do NOT create modules/ entries") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_fp_das"
        let out_dir = "{get_das_root()}/_test_release_fp_das_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(tmp_root)

        // Project: main.das requires fio (stdlib) + a project-local module.
        fopen("{tmp_root}/helper.das", "wb") $(f) {
            fwrite(f, "options gen2\nmodule helper shared public\n\ndef public greet(name : string) : string \{\n    return \"hello, \{name\}\"\n\}\n")
        }
        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/fio\nrequire helper\n\n[export]\ndef main() \{\n    print(\"\{greet(\"world\")\}\\n\")\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"fpdas\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n\}\n")
        }

        let rc = cmd_release(tmp_root, out_dir)
        tt |> equal(0, rc, "cmd_release returns 0")
        let exe_path = bundle_exe_path(out_dir, "fpdas")
        let artifacts = bundle_artifacts_dir(out_dir, "fpdas")
        tt |> success(fexist(exe_path), "exe shipped at {exe_path}")
        // No modules/ tree at all — project has no shared modules and no daspkg deps.
        tt |> equal(false, fexist(path_join(artifacts, "modules")),
            "no modules/ tree (no shared deps, no daspkg deps; daslib stdlib + project-local .das must NOT show up)")

        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
}

// Transitive release: project requires a synthetic dep at <root>/modules/<DepName>/
// whose own .das_package has release_include for asset files. Bundle must place
// dep assets under <bundle>/modules/<DepName>/<rel>.
[test(description = "a dependency release_include files ship under modules/<DepName>/", timeout = 1200)]
def test_cmd_release_transitive_dep(t : T?) {
    t |> run("dep release_include files are shipped under modules/<DepName>/") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_transitive"
        let out_dir = "{get_das_root()}/_test_release_transitive_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(tmp_root)
        mkdir("{tmp_root}/modules")
        mkdir("{tmp_root}/modules/fakedep")
        mkdir("{tmp_root}/modules/fakedep/daslib")
        mkdir("{tmp_root}/modules/fakedep/data")

        // Synthetic dep: package + module + asset.
        fopen("{tmp_root}/modules/fakedep/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"fakedep\")\n\}\n[export]\ndef release() \{\n    release_include(\"data/**\")\n\}\n")
        }
        fopen("{tmp_root}/modules/fakedep/.das_module", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/fio\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"fakedep\", \"fakedep_lib\", \"\{project_path\}/daslib/fakedep_lib.das\")\n\}\n")
        }
        fopen("{tmp_root}/modules/fakedep/daslib/fakedep_lib.das", "wb") $(f) {
            fwrite(f, "options gen2\nmodule fakedep_lib shared public\n\ndef public hello() : string \{\n    return \"hello from fakedep\"\n\}\n")
        }
        fopen("{tmp_root}/modules/fakedep/data/asset.bin", "wb") $(f) {
            fwrite(f, "asset-content")
        }
        // Excluded file — must NOT ship.
        fopen("{tmp_root}/modules/fakedep/data/internal.bin", "wb") $(f) {
            fwrite(f, "secret")
        }

        // Parent project pulls fakedep in.
        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\nrequire fakedep/fakedep_lib\n\n[export]\ndef main() \{\n    print(\"\{hello()\}\\n\")\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"transit\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n\}\n")
        }

        let rc = cmd_release(tmp_root, out_dir)
        tt |> equal(0, rc, "cmd_release returns 0")
        let exe_path = bundle_exe_path(out_dir, "transit")
        let artifacts = bundle_artifacts_dir(out_dir, "transit")
        tt |> success(fexist(exe_path), "parent exe shipped at {exe_path}")
        tt |> success(fexist(path_join(artifacts, "modules/fakedep/data/asset.bin")),
            "dep asset shipped via transitive release()")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
}

// Walk `path` via `commands::list_lc_rpaths` + `commands::is_external_rpath`
// (the same parser the production scrubber uses) and append every external
// LC_RPATH entry to `leaks`. Thin wrapper so the test exercises the shared
// helpers instead of paraphrasing the parser locally.
def private add_external_lc_rpaths(path : string; var leaks : array<string>) {
    for (rp in list_lc_rpaths(path)) {
        if (is_external_rpath(rp)) {
            leaks |> push("{path}: {rp}")
        }
    }
}

// macOS: a bundle copy is portable only if (1) every dylib's install_name alias
// resolves to an actual file in the bundle, and (2) no shipped binary carries
// an absolute LC_RPATH pointing back into the build tree. Without (1) dyld
// silently falls back through the build-tree LC_RPATH; without (2) the bundle
// "works" on the build machine and breaks on a clean recipient. Both are the
// hard-to-diagnose ship blocker — exercise them explicitly.
//
// Requires dasGlfw because its dylib's install_name (e.g. `@rpath/libglfw.3.dylib`)
// differs from the shipped filename (e.g. `libglfw.3.4.dylib`) — that mismatch
// is what makes the symlink alias load-bearing. The test discovers both names
// at runtime rather than hardcoding versions, so it survives GLFW bumps.
[test(description = "on macOS the install_name alias exists and no LC_RPATH points outside the bundle", timeout = 1200)]
def test_cmd_release_portable_on_macos(t : T?) { // nolint:STYLE038
    if (get_platform_name() != "darwin") {
        t |> run("skipped — macOS-only audit (otool/install_name_tool)") @(tt : T?) {
            tt |> success(true)
        }
        return
    }
    var dasGlfw_present = false
    for_each_registered_dynamic_module() $(_path, _mod_name, das_name) {
        if (das_name == "glfw") {
            dasGlfw_present = true
        }
    }
    if (!dasGlfw_present) {
        t |> run("skipped — dasGlfw not registered; can't exercise install_name-alias path") @(tt : T?) {
            to_log(LOG_INFO, "test_cmd_release_portable_on_macos: dasGlfw not loaded; skipping\n")
            tt |> success(true)
        }
        return
    }
    t |> run("install_name alias is created and no LC_RPATH points outside the bundle") @(tt : T?) {
        let tmp_root = "{get_das_root()}/_test_release_portable_macos"
        let out_dir = "{get_das_root()}/_test_release_portable_macos_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(tmp_root)

        // Smallest valid project that pulls dasGlfw into the bundle. The exe
        // doesn't run — we only need it on disk to inspect.
        fopen("{tmp_root}/main.das", "wb") $(f) {
            fwrite(f, "options gen2\nrequire glfw/glfw_boost\n[export]\ndef main() \{\n    print(\"glfw\\n\")\n\}\n")
        }
        fopen("{tmp_root}/.das_package", "wb") $(f) {
            fwrite(f, "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"glfwbn\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n\}\n")
        }

        let rc = cmd_release(tmp_root, out_dir)
        tt |> equal(0, rc, "cmd_release returns 0")
        let artifacts = bundle_artifacts_dir(out_dir, "glfwbn")

        // Discover the real dylib (a regular file under modules/dasGlfw/)
        // rather than hardcoding `libglfw.3.4.dylib` — that filename moves
        // every time GLFW bumps and would break the test silently.
        //
        // Asserts exactly one regular `.dylib` entry: if a future dep ever
        // lands a second dylib under `modules/dasGlfw/`, the test should
        // fail loudly so we revisit the discovery logic rather than picking
        // whichever entry the filesystem yields last.
        let glfw_dir = path_join(artifacts, "modules/dasGlfw")
        var real_dylib = ""
        var dylib_count = 0
        dir(glfw_dir) $(filename) {
            if (filename == "." || filename == ".." || !ends_with(filename, ".dylib")) return
            let p = path_join(glfw_dir, filename)
            var sym_err : string
            if (is_symlink(p, sym_err)) return
            real_dylib = p
            dylib_count++
        }
        tt |> equal(1, dylib_count, "exactly one real GLFW dylib in {glfw_dir} (got {dylib_count})")
        var id_out : string
        let id_rc = run_cmd("otool -D \"{real_dylib}\"", id_out)
        tt |> equal(0, id_rc, "otool -D runs")
        var install_name = ""
        for (line in split(id_out, "\n")) {
            let s = strip(line)
            if (empty(s) || ends_with(s, ":")) continue
            install_name = s
        }
        tt |> success(starts_with(install_name, "@rpath/"),
            "install_name is @rpath-relative (got: {install_name})")
        let alias_name = base_name(install_name)
        let alias_path = path_join(glfw_dir, alias_name)
        if (alias_name != base_name(real_dylib)) {
            tt |> success(fexist(alias_path),
                "install_name alias `{alias_name}` materialized at {alias_path}")
            var eq_err : string
            tt |> success(equivalent(alias_path, real_dylib, eq_err),
                "alias resolves to the real dylib (equivalent => true; err=`{eq_err}`)")
        }

        // No shipped binary may keep an absolute LC_RPATH — that's the leak
        // that hides the install_name-alias bug on the build machine. Skip
        // symlinks during the glob walk so the install_name alias doesn't
        // get scanned a second time alongside its real-file target (which
        // would double-report any future leak in the same inode).
        var leaks : array<string>
        add_external_lc_rpaths(bundle_exe_path(out_dir, "glfwbn"), leaks)
        glob(artifacts, "**/*.dylib") $(rel, is_dir) {
            if (is_dir) return
            let p = path_join(artifacts, rel)
            var sym_err : string
            if (is_symlink(p, sym_err)) return
            add_external_lc_rpaths(p, leaks)
        }
        glob(artifacts, "**/*.shared_module") $(rel, is_dir) {
            if (is_dir) return
            let p = path_join(artifacts, rel)
            var sym_err : string
            if (is_symlink(p, sym_err)) return
            add_external_lc_rpaths(p, leaks)
        }
        let leaks_msg = join(leaks, "; ")
        tt |> equal(0, length(leaks),
            "no absolute LC_RPATH in shipped binaries (leaks: {leaks_msg})")

        force_rmdir(tmp_root)
        force_rmdir(out_dir)
    }
}

def private push_text(var bytes : array<uint8>; text : string) {
    bytes |> reserve(length(bytes) + length(text))
    for (c in text) {
        bytes |> push(uint8(c))
    }
}

//! a 60-byte ar member header: name, then the decimal size in its column, then the two-byte terminator (a backtick and a newline)
def private push_ar_header(var bytes : array<uint8>; name : string; size : int) {
    let digits = "{size}"
    push_text(bytes, "{name}{repeat(" ", 48 - length(name))}{digits}{repeat(" ", 10 - length(digits))}`\n")
}

//! a fake `!<arch>` file: a symbol-table member of `index_bytes` zeros, a GNU long-name table when the
//! object's name is `/0` (the shape llvm-ar writes for a long file name), then the object opening with the magic
def private write_archive(path : string; magic : array<uint8>; index_bytes : int = 0; long_name : bool = false) {
    var bytes : array<uint8>
    bytes |> reserve(index_bytes + 256)
    push_text(bytes, "!<arch>\n")
    if (index_bytes > 0) {
        push_ar_header(bytes, "/", index_bytes)
        for (_i in range(index_bytes + (index_bytes & 1))) {
            bytes |> push(0u8)
        }
    }
    if (long_name) {
        push_ar_header(bytes, "//", 20)
        push_text(bytes, "a_long_member_name/\n")
    }
    push_ar_header(bytes, long_name ? "/0" : "member.o/", 64)
    bytes |> push_from(magic)
    push_text(bytes, "padding past the magic so the member has a body")
    fopen(path, "wb") $(f) {
        fwrite(f, bytes)
    }
}

//! the BSD shape macOS's ar writes: a `__.SYMDEF SORTED` table, then a member whose `#1/NN` name leads its body
def private write_bsd_archive(path : string; magic : array<uint8>) {
    var bytes : array<uint8>
    bytes |> reserve(256)
    push_text(bytes, "!<arch>\n")
    push_ar_header(bytes, "__.SYMDEF SORTED", 16)
    push_text(bytes, "0000000000000000")
    let name = "a_long_member_name.o"
    push_ar_header(bytes, "#1/{length(name)}", length(name) + 64)
    push_text(bytes, name)
    bytes |> push_from(magic)
    push_text(bytes, "padding past the magic so the member has a body")
    fopen(path, "wb") $(f) {
        fwrite(f, bytes)
    }
}

[test(description = "build --wasm stages an archive only when a member carries the wasm magic, and refuses a native archive of the same name")]
def test_wasm_archive_detection(t : T?) {
    t |> run("build --wasm stages an archive only when a member carries the wasm magic; a native archive under the same name is refused") @(t : T?) {
        let tmp = create_temp_directory_result("daspkg archives ")
        if (!(tmp is value)) {
            t |> failure("could not create temp directory: {tmp as error}")
            return
        }
        let root = tmp as value
        let wasm_path = path_join(root, "liblibDasModuleWasm.a")
        let indexed_path = path_join(root, "liblibDasModuleIndexed.a")
        let native_path = path_join(root, "liblibDasModuleNative.a")
        let wasm_magic <- [0x00u8, 0x61u8, 0x73u8, 0x6du8, 0x01u8, 0x00u8, 0x00u8, 0x00u8]   // "\0asm" + version 1
        write_archive(wasm_path, wasm_magic)
        write_archive(indexed_path, wasm_magic, 200000)   // a symbol table far past any fixed scan window
        let long_named_path = path_join(root, "liblibDasModuleLongName.a")
        write_archive(long_named_path, wasm_magic, 1024, true)   // llvm-ar's shape: `/` table, `//` names, the object as `/0`
        let odd_path = path_join(root, "liblibDasModuleOdd.a")
        write_archive(odd_path, wasm_magic, 1023)   // an odd-sized table: the next header sits past its padding byte
        let bsd_path = path_join(root, "liblibDasModuleBsd.a")
        write_bsd_archive(bsd_path, wasm_magic)
        let bsd_native_path = path_join(root, "liblibDasModuleBsdNative.a")
        write_bsd_archive(bsd_native_path, [0xcfu8, 0xfau8, 0xedu8, 0xfeu8, 0x0cu8, 0x00u8, 0x00u8, 0x01u8])
        write_archive(native_path, [0xcfu8, 0xfau8, 0xedu8, 0xfeu8, 0x0cu8, 0x00u8, 0x00u8, 0x01u8])   // a 64-bit Mach-O member
        let text_path = path_join(root, "liblibDasModuleText.a")
        fopen(text_path, "wb") $(f) {
            fwrite(f, "not an archive at all, whatever the name says; long enough to read a header from\n")
        }
        t |> success(looks_like_wasm_archive(wasm_path), "an archive whose first object opens with the wasm magic is a wasm archive")
        t |> success(looks_like_wasm_archive(indexed_path), "a 200 KB symbol table ahead of that object changes nothing: members are walked, not scanned")
        t |> success(looks_like_wasm_archive(long_named_path), "an object named through the long-name table is an object, not a table")
        t |> success(looks_like_wasm_archive(odd_path), "an odd-sized table's padding byte is stepped over")
        t |> success(looks_like_wasm_archive(bsd_path), "BSD ar: the __.SYMDEF table is skipped and the #1/NN name is stepped over")
        t |> success(!looks_like_wasm_archive(bsd_native_path), "BSD ar around a Mach-O member is native")
        t |> success(!looks_like_wasm_archive(native_path), "an archive of native objects is not, whatever its name")
        t |> success(!looks_like_wasm_archive(text_path), "a file without the !<arch> header is not an archive")
        t |> success(!looks_like_wasm_archive(path_join(root, "missing.a")), "a file that is not there is not a wasm archive")
        force_rmdir(root)
    }
}

[test(description = "build --wasm stages the runtime and module archives of the web lib dir and nothing else")]
def test_wasm_archive_staging(t : T?) {
    t |> run("build --wasm stages the runtime and module archives of the web build's lib dir and nothing else") @(t : T?) {
        let tmp = create_temp_directory_result("daspkg staging ")
        if (!(tmp is value)) {
            t |> failure("could not create temp directory: {tmp as error}")
            return
        }
        let root = tmp as value
        let web_lib = path_join(root, "web_lib")
        let dst = path_join(root, "dst")
        mkdir_rec(web_lib)
        let wasm_magic <- [0x00u8, 0x61u8, 0x73u8, 0x6du8, 0x01u8, 0x00u8, 0x00u8, 0x00u8]
        let native_magic <- [0xcfu8, 0xfau8, 0xedu8, 0xfeu8, 0x0cu8, 0x00u8, 0x00u8, 0x01u8]
        write_archive(path_join(web_lib, "liblibDaScript_runtime.a"), wasm_magic)
        write_archive(path_join(web_lib, "liblibDasModuleGlfw.a"), wasm_magic)
        write_archive(path_join(web_lib, "libfreetype.a"), native_magic)   // a third-party bake beside them
        t |> equal(stage_wasm_archives(web_lib, dst), 2, "the runtime and the module archive are staged")
        t |> success(fexist(path_join(dst, "liblibDaScript_runtime.a")) && fexist(path_join(dst, "liblibDasModuleGlfw.a")), "both land in the lib dir")
        t |> success(!fexist(path_join(dst, "libfreetype.a")), "an archive that is neither runtime nor module is left where it is")
        t |> equal(stage_wasm_archives(web_lib, web_lib), 2, "the same directory counts without copying")
        write_archive(path_join(web_lib, "liblibDasModuleNative.a"), native_magic)
        t |> equal(stage_wasm_archives(web_lib, dst), -1, "a native archive among them refuses the stage")
        force_rmdir(root)
    }
}

[test(description = "external files declared by a release ship into the bundle")]
def test_ship_external_files(t : T?) {
    let tmp = create_temp_directory_result("daspkg external ")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return
    }
    let root = tmp as value
    let bundle = path_join(root, "bundle")
    mkdir(bundle)
    var shipped : array<string>
    t |> equal(ship_external_files("pkg", bundle, ["README.md@nested/readme.txt", "README.md@", "utils\\daspkg\\README.md@windows\\nested\\readme.txt"], shipped), 0)
    t |> equal(fread(path_join(bundle, "nested/readme.txt")), fread(path_join(get_das_root(), "README.md")))
    t |> success(fexist(path_join(bundle, "README.md")), "default destination uses the source filename")
    t |> equal(length(shipped), 3)
    t |> success(fexist(path_join(bundle, "windows/nested/readme.txt")), "Windows separators name the same destination on every host")
    if (length(shipped) == 3) {
        t |> equal(shipped[2], "windows/nested/readme.txt", "manifest stores normalized separators")
    }
    fwrite(path_join(root, "victim.txt"), "untouched")
    for (entry in ["README.md@../victim.txt", "README.md@nested/../../victim.txt", "README.md@{root}/absolute.txt", "README.md@nested/quote\".txt", "README.md@nested\\..\\..\\victim.txt", "@empty.txt", "README.md", "missing-external-source@missing.txt"]) {
        var inscope rejected : array<string>
        t |> equal(ship_external_files("pkg", bundle, [entry], rejected), 1, entry)
        t |> equal(length(rejected), 0, "rejected entry is not recorded as shipped")
    }
    t |> equal(fread(path_join(root, "victim.txt")), "untouched", "destinations cannot escape the bundle")
    mkdir(path_join(bundle, "directory"))
    var failed_copy : array<string>
    t |> equal(ship_external_files("pkg", bundle, ["README.md@directory"], failed_copy), 1, "copy errors propagate")
    t |> equal(length(failed_copy), 0)
    let cleanup = rmdir_rec_result(root)
    t |> success(cleanup is value && cleanup as value, "temp directory removed")
}

// Writes a releasable package (main.das + release_main) declaring `platform` as its only platform.
def private write_release_platform_pkg(src_dir, pkg_name, platform : string) {
    mkdir(src_dir)
    fwrite("{src_dir}/main.das", "options gen2\n[export]\ndef main() \{\n    print(\"hello\\n\")\n\}\n")
    fwrite("{src_dir}/.das_package", "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"{pkg_name}\")\n    package_platform(\"{platform}\")\n\}\n[export]\ndef release() \{\n    release_main(\"main.das\")\n\}\n")
}

[test(description = "release refuses a package declaring another platform before writing anything, and release wasm checks the declaration against emscripten", timeout = 1200)]
def test_release_platform_gate(t : T?) {
    t |> run("release refuses a package declaring another platform before writing anything") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_relgate"
        let out_dir = "{get_das_root()}/_daspkg_test_relgate_out"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        write_release_platform_pkg(tmp_root, "relgate", other_platform())
        t |> equal(RC_UNSUPPORTED_PLATFORM, cmd_release(tmp_root, out_dir))
        t |> success(!fexist(out_dir))
        force_rmdir(tmp_root)
    }
    t |> run("release wasm checks the declaration against emscripten, not the host") @(t : T?) {
        let tmp_root = "{get_das_root()}/_daspkg_test_relgate"
        let out_dir = "{get_das_root()}/_daspkg_test_relgate_out"
        let empty_lib = "{get_das_root()}/_daspkg_test_relgate_lib"
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        mkdir(empty_lib)
        write_release_platform_pkg(tmp_root, "relgate", get_platform_name())
        t |> equal(RC_UNSUPPORTED_PLATFORM, cmd_release_wasm(tmp_root, out_dir, empty_lib))
        write_release_platform_pkg(tmp_root, "relgate", "wasm")
        t |> equal(1, cmd_release_wasm(tmp_root, out_dir, empty_lib), "past the gate, the empty archive dir is the next refusal")
        force_rmdir(tmp_root)
        force_rmdir(out_dir)
        force_rmdir(empty_lib)
    }
}

[test(description = "the wasm release replaces the shell marker with the gate script and fails on a marker with no script")]
def test_release_inlines_wasm_gate(t : T?) {
    let MARKED = "<body>\n<!-- @wasm-gate -->\n<script>x()</script>\n</body>\n"
    let BARE = "<body>\n<script>x()</script>\n</body>\n"
    let GATE = "window.dasWasmGate = 1;\n"
    let GATED = "<body>\n<script>\n{GATE}</script>\n<script>x()</script>\n</body>\n"
    t |> run("the gate script replaces the shell's marker") @(t : T?) {
        let gate = path_join(test_temp_dir(), "wasmgate_gate.js")
        let page = path_join(test_temp_dir(), "wasmgate_page.html")
        fwrite(gate, GATE)
        fwrite(page, MARKED)
        t |> success(inline_wasm_gate(page, gate))
        t |> equal(GATED, fread(page))
    }
    t |> run("a shell without the marker is left alone, gate script or not") @(t : T?) {
        let gate = path_join(test_temp_dir(), "wasmgate_gate.js")
        let page = path_join(test_temp_dir(), "wasmgate_bare.html")
        fwrite(gate, GATE)
        fwrite(page, BARE)
        t |> success(inline_wasm_gate(page, gate))
        t |> equal(BARE, fread(page))
        let missing = path_join(test_temp_dir(), "wasmgate_missing.js")
        t |> success(inline_wasm_gate(page, missing), "no marker, so no gate is needed")
        t |> equal(BARE, fread(page))
    }
    t |> run("a marker with no gate script to put there fails the release") @(t : T?) {
        let page = path_join(test_temp_dir(), "wasmgate_orphan.html")
        fwrite(page, MARKED)
        t |> success(!inline_wasm_gate(page, path_join(test_temp_dir(), "wasmgate_missing.js")))
        t |> equal(MARKED, fread(page), "the page keeps its marker")
    }
}

[test(description = "a release forwards -load_module arguments, quoted for the shell, so a mounted external module builds and runs", timeout = 1200)]
def test_release_external_module_arguments(t : T?) {
    let tmp = create_temp_directory_result("daspkg module path ")
    if (!(tmp is value)){t |> failure("temporary module directory unavailable"); return }
    let root = tmp as value
    defer(){force_rmdir(root)}
    let module_dir = path_join(root, get_platform_name() == "windows" ? "external module" : "external module's $(not-a-command)")
    mkdir(module_dir)
    fwrite(path_join(module_dir, ".das_module"), "options gen2\nrequire daslib/fio\n[export]\ndef initialize(root : string) \{ register_native_path(\"release_fixture\", \"value\", root + \"/value.das\") \}\n")
    fwrite(path_join(module_dir, "value.das"), "options gen2\nlet RELEASE_FIXTURE_VALUE = 42\n")
    let script = path_join(root, "main.das")
    fwrite(script, "options gen2\nrequire release_fixture/value\n[export]\ndef main()\{print(\"mounted: \{RELEASE_FIXTURE_VALUE}\\n\")\}\n")
    let exe = get_host_binary()
    for (flag in ["-load_module", "-load-module", "--load_module", "--load-module"]) {
        let args = release_module_args([exe, flag, module_dir, "main.das", "--", "-load_module", "must-not-load"])
        var output : string
        let rc = run_cmd("\"{exe}\"{args} \"{script}\"", output)
        t |> equal(rc, 0, output)
        t |> success(find(output, "mounted: 42") >= 0, output)
    }
    t |> equal(release_module_args([exe, "main.das", "--", "-load_module", module_dir]), "")
    fwrite(path_join(root, "companion.das"), "options gen2\nrequire release_fixture/value\n[export]\ndef main()\{print(\"companion: \{RELEASE_FIXTURE_VALUE}\\n\")\}\n")
    fwrite(path_join(root, ".das_package"), "options gen2\nrequire daslib/daspkg\n[export]\ndef package()\{package_name(\"mounted_app\")}\n[export]\ndef release()\{release_main(\"main.das\"); release_program(\"mounted_companion\", \"companion.das\")}\n")
    let mounted = release_module_args([exe, "-load_module", module_dir])
    let payload = "\{\"kernels\":\{\"mounted_stub\":\{\"suffix\":\"stub\"\}\},\"provenance\":\{\"mode\":\"stub\"\}\}"
    fwrite(path_join(root, "stub_payload.json"), payload)
    fwrite(path_join(root, "stub_tuner.das"), "options gen2\nrequire daslib/fio\nrequire release_fixture/value\n[export]\ndef main()\{\n    assert(RELEASE_FIXTURE_VALUE == 42)\n    fwrite(get_env_variable(\"DAS_TUNE_MANIFEST\"), fread(get_env_variable(\"DASPKG_MOUNT_PAYLOAD\")))\n    print(\"mounted tuner: \{RELEASE_FIXTURE_VALUE}\\n\")\n\}\n")
    fwrite(script, "options gen2\nrequire release_fixture/value\nrequire daslib/tune\nrequire llvm/daslib/llvm_tune\n[tune_scope(name=\"mounted_scope\", tuner=\"stub_tuner.das\")]\nstruct MountedScope\{unused : int\}\n[export]\ndef main()\{print(\"mounted: \{RELEASE_FIXTURE_VALUE}\\n\")\}\n")
    let old_payload = get_env_variable("DASPKG_MOUNT_PAYLOAD")
    set_env_variable("DASPKG_MOUNT_PAYLOAD", path_join(root, "stub_payload.json"))
    defer(){set_env_variable("DASPKG_MOUNT_PAYLOAD", old_payload)}
    let out_dir = path_join(root, "out")
    var output : string
    let release_rc = run_cmd("\"{exe}\"{mounted} \"{get_das_root()}/utils/daspkg/main.das\" -- release --root \"{root}\" --out \"{out_dir}\"", output)
    t |> equal(release_rc, 0, output)
    if (release_rc == 0) {
        let minted = path_join(root, "main.tune.json")
        t |> success(fexist(minted), "the external-module tuner minted the sidecar")
        t |> equal(fread(minted), payload)
        let shipped = path_join(bundle_artifacts_dir(out_dir, "mounted_app"), "mounted_app.tune.json")
        t |> equal(fread(shipped), payload, "the tuner payload ships with the main executable")
        let companion = path_join(bundle_artifacts_dir(out_dir, "mounted_app"), get_platform_name() == "darwin" ? "mounted_companion" : "mounted_companion.exe")
        let companion_rc = run_cmd("\"{companion}\"", output)
        t |> equal(companion_rc, 0, output)
        t |> success(find(output, "companion: 42") >= 0, output)
        let app = bundle_exe_path(out_dir, "mounted_app")
        let run_rc = run_cmd("\"{app}\"", output)
        t |> equal(run_rc, 0, output)
        t |> success(find(output, "mounted: 42") >= 0, output)
    }
    let wasm_lib_dir = get_env_variable("DASPKG_TEST_WASM_LIB_DIR")
    if (!empty(wasm_lib_dir)) {
        fwrite(script, "options gen2\nrequire release_fixture/value\n[export]\ndef main()\{print(\"mounted: \{RELEASE_FIXTURE_VALUE}\\n\")\}\n")
        let wasm_out = path_join(root, "wasm_out")
        let wasm_rc = run_cmd("\"{exe}\"{mounted} \"{get_das_root()}/utils/daspkg/main.das\" -- release wasm --root \"{root}\" --out \"{wasm_out}\" --wasm-lib-dir \"{wasm_lib_dir}\"", output)
        t |> equal(wasm_rc, 0, output)
        t |> success(fexist(path_join(wasm_out, "mounted_app/mounted_app.wasm")), "wasm cross-compiler resolves the mounted namespace")
    }
}

[test(description = "release_module_args rejects a missing directory and unsafe shell characters and keeps repeated -load_module flags in order")]
def test_release_external_module_argument_errors(t : T?) {
    let missing = panic_text() {
        release_module_args(["daslang", "-load_module"])
    }
    t |> success(find(missing, "requires a module directory") >= 0, missing)
    if (get_platform_name() == "windows") {
        for (path in ["C:/module%name", "C:/module!name", "C:/module\"name", "C:/module\nname", "C:/module\rname"]) {
            let why = panic_text() {
                release_module_args(["daslang", "-load_module", path])
            }
            t |> success(find(why, "Unsupported shell character") >= 0, why)
            t |> success(find(why, path) >= 0, "the diagnostic identifies the rejected directory: {why}")
        }
        t |> equal(release_module_args(["daslang", "-load_module", "C:\\safe module\\path"]), " -load_module \"C:/safe module/path\"")
    }
    let repeated = release_module_args(["daslang", "-load_module", "first", "-load-module", "second", "--", "-load_module", "ignored"])
    let expected = get_platform_name() == "windows" ? " -load_module \"first\" -load_module \"second\"" : " -load_module 'first' -load_module 'second'"
    t |> equal(repeated, expected)
}

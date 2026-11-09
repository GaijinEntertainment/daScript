options gen2
options no_global_variables = false

require dastest/testing_boost public

require daslib/fio
require daslib/defer

require daslib/json_boost


require utils
require index
require commands
require daslib/result
require package_runner public
require _daspkg_test_common

[test(description = "read_manifest reads a valid .das_package and fails on a missing name, description or file")]
def test_read_manifest(t : T?) {
    t |> run("valid manifest from .das_package") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_manifest"
        mkdir(tmp_dir)
        fwrite("{tmp_dir}/.das_package", "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"mypkg\")\n    package_description(\"A test package\")\n\}\n")
        var manifest : PackageManifest
        t |> success(read_manifest(tmp_dir, manifest))
        t |> equal(manifest.name, "mypkg")
        t |> equal(manifest.description, "A test package")
        remove("{tmp_dir}/.das_package")
        rmdir(tmp_dir)
    }
    t |> run("missing name") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_manifest2"
        mkdir(tmp_dir)
        fwrite("{tmp_dir}/.das_package", "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_description(\"no name\")\n\}\n")
        var manifest : PackageManifest
        t |> success(!read_manifest(tmp_dir, manifest))
        remove("{tmp_dir}/.das_package")
        rmdir(tmp_dir)
    }
    t |> run("missing description") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_manifest3"
        mkdir(tmp_dir)
        fwrite("{tmp_dir}/.das_package", "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"mypkg\")\n\}\n")
        var manifest : PackageManifest
        t |> success(!read_manifest(tmp_dir, manifest))
        remove("{tmp_dir}/.das_package")
        rmdir(tmp_dir)
    }
    t |> run("no manifest file") @(t : T?) {
        var manifest : PackageManifest
        t |> success(!read_manifest("{get_das_root()}/_daspkg_nonexistent", manifest))
    }
}

// --- package_runner tests ---

let PKG_FIXTURE = "utils/daspkg/fixtures/test.das_package"

let PKG_BRANCH_FIXTURE = "utils/daspkg/fixtures/test_branch.das_package"

let PKG_REDIRECT_FIXTURE = "utils/daspkg/fixtures/test_redirect.das_package"

let PKG_SDK_AWARE_FIXTURE = "utils/daspkg/fixtures/test_sdk_aware.das_package"

let PKG_RICH_FIXTURE = "utils/daspkg/fixtures/test_rich.das_package"

let PKG_RELEASE_FIXTURE = "utils/daspkg/fixtures/test_release.das_package"

[test(description = "run_das_package_meta reads the package metadata from a manifest")]
def test_run_das_package_meta(t : T?) {
    t |> run("reads package metadata") @(t : T?) {
        var meta : PackageMeta
        let ok = run_das_package_meta(PKG_FIXTURE, meta)
        t |> success(ok)
        t |> equal("test-pkg", meta.pkg_name)
        t |> equal("Test Author", meta.author)
        t |> equal("A test package", meta.description)
        t |> equal("https://github.com/test/test-pkg", meta.source)
    }
}

[test(description = "a manifest resolves a version to its tag download")]
def test_run_das_package_resolve_tag(t : T?) {
    t |> run("resolves tag version") @(t : T?) {
        var res : ResolveResult
        let ok = run_das_package_resolve(PKG_FIXTURE, "0.5", "1.2.3", res)
        t |> success(ok)
        t |> equal("v1.2.3", res.tag)
        t |> equal("", res.branch)
    }
}

[test(description = "a manifest resolves a version to its branch download")]
def test_run_das_package_resolve_branch(t : T?) {
    t |> run("resolves branch download") @(t : T?) {
        var res : ResolveResult
        let ok = run_das_package_resolve(PKG_BRANCH_FIXTURE, "0.5", "1.0", res)
        t |> success(ok)
        t |> equal("", res.tag)
        t |> equal("main", res.branch)
    }
}

[test(description = "a manifest reports its dependencies")]
def test_run_das_package_deps(t : T?) {
    t |> run("reads dependencies") @(t : T?) {
        var deps : array<PackageDependency>
        let ok = run_das_package_deps(PKG_FIXTURE, "1.0", deps)
        t |> success(ok)
        t |> equal(2, length(deps))
        t |> equal("https://github.com/test/dep-a", deps[0].source)
        t |> equal(">=1.0", deps[0].version_constraint)
        t |> equal("https://github.com/test/dep-b", deps[1].source)
        t |> equal("", deps[1].version_constraint)
    }
}

[test(description = "a manifest reports its cmake build info")]
def test_run_das_package_build(t : T?) {
    t |> run("reads cmake build info") @(tt : T?) {
        var info : PackageBuildInfo
        let ok = run_das_package_build(PKG_FIXTURE, info)
        tt |> success(ok)
        tt |> equal(true, info.is_cmake)
    }
}

[test(description = "run_das_package_meta returns false for a missing manifest file")]
def test_run_das_package_missing_file(t : T?) {
    t |> run("returns false for missing file") @(t : T?) {
        var meta : PackageMeta
        let ok = run_das_package_meta("nonexistent.das_package", meta)
        t |> equal(false, ok)
    }
}

[test(description = "a manifest may omit its optional hook functions")]
def test_run_das_package_optional_functions(t : T?) {
    t |> run("succeeds when function is missing") @(tt : T?) {
        // test_branch.das_package has no build() function — should still succeed
        var info : PackageBuildInfo
        let ok = run_das_package_build(PKG_BRANCH_FIXTURE, info)
        tt |> success(ok)
        tt |> equal(false, info.is_cmake)
        tt |> equal(false, info.is_custom)
    }
}

[test(description = "a manifest can redirect resolution to a different repository")]
def test_run_das_package_resolve_redirect(t : T?) {
    t |> run("resolves redirect to different repo") @(t : T?) {
        var res : ResolveResult
        let ok = run_das_package_resolve(PKG_REDIRECT_FIXTURE, "", "", res)
        t |> success(ok)
        t |> equal("github.com/borisbat/daspkg-test-pure", res.source)
        t |> equal("v1.0", res.tag)
        t |> equal("", res.branch)
    }
}

[test(description = "an SDK-aware manifest resolves latest, a range and an explicit version to the right tags")]
def test_run_das_package_resolve_sdk_aware(t : T?) {
    t |> run("resolves latest to default tag") @(t : T?) {
        var res : ResolveResult
        let ok = run_das_package_resolve(PKG_SDK_AWARE_FIXTURE, "0.5", "", res)
        t |> success(ok)
        t |> equal("v2.0", res.tag)
    }
    t |> run("resolves 1.x to v1.5") @(t : T?) {
        var res : ResolveResult
        let ok = run_das_package_resolve(PKG_SDK_AWARE_FIXTURE, "0.5", "1.x", res)
        t |> success(ok)
        t |> equal("v1.5", res.tag)
    }
    t |> run("resolves explicit version") @(t : T?) {
        var res : ResolveResult
        let ok = run_das_package_resolve(PKG_SDK_AWARE_FIXTURE, "0.5", "3.0", res)
        t |> success(ok)
        t |> equal("v3.0", res.tag)
    }
}

// --- rich metadata tests ---

[test(description = "run_das_package_meta reads license, tags and min_sdk and leaves them empty when the manifest has none")]
def test_run_das_package_rich_meta(t : T?) {
    t |> run("reads license, tags, min_sdk") @(t : T?) {
        var meta : PackageMeta
        let ok = run_das_package_meta(PKG_RICH_FIXTURE, meta)
        t |> success(ok)
        t |> equal("rich-pkg", meta.pkg_name)
        t |> equal("testauthor", meta.author)
        t |> equal("A richly annotated test package", meta.description)
        t |> equal("MIT", meta.license)
        t |> equal(2, length(meta.tags))
        t |> equal("networking", meta.tags[0])
        t |> equal("http", meta.tags[1])
        t |> equal("0.4", meta.min_sdk)
    }
    t |> run("original fixture has no new fields") @(t : T?) {
        var meta : PackageMeta
        let ok = run_das_package_meta(PKG_FIXTURE, meta)
        t |> success(ok)
        t |> success(empty(meta.license))
        t |> equal(0, length(meta.tags))
        t |> success(empty(meta.min_sdk))
    }
}

// --- read_manifest with rich metadata ---

[test(description = "read_manifest reads license, tags and min_sdk")]
def test_read_manifest_rich(t : T?) {
    t |> run("reads license, tags, min_sdk from manifest") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_manifest_rich"
        mkdir(tmp_dir)
        fwrite("{tmp_dir}/.das_package", "options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"richpkg\")\n    package_description(\"Rich package\")\n    package_license(\"Apache-2.0\")\n    package_tag(\"ai\")\n    package_tag(\"ml\")\n    package_min_sdk(\"0.5\")\n\}\n")
        var manifest : PackageManifest
        t |> success(read_manifest(tmp_dir, manifest))
        t |> equal("richpkg", manifest.name)
        t |> equal("Apache-2.0", manifest.license)
        t |> equal(2, length(manifest.tags))
        t |> equal("ai", manifest.tags[0])
        t |> equal("ml", manifest.tags[1])
        t |> equal("0.5", manifest.min_sdk)
        remove("{tmp_dir}/.das_package")
        rmdir(tmp_dir)
    }
}

// --- release tests ---

[test(description = "a manifest reports its release info, and an absent release() gives an empty spec")]
def test_run_das_package_release(t : T?) {
    t |> run("reads release info from .das_package") @(tt : T?) {
        var info : PackageReleaseInfo
        let ok = run_das_package_release(PKG_RELEASE_FIXTURE, info)
        tt |> success(ok)
        tt |> equal("custom-bundle-name", info.name)
        tt |> equal("main.das", info.main_script)
        tt |> equal(2, length(info.include_globs))
        tt |> equal("*.png", info.include_globs[0])
        tt |> equal("data/**/*.json", info.include_globs[1])
        tt |> equal(1, length(info.include_if_missing_globs))
        tt |> equal("config/*.toml", info.include_if_missing_globs[0])
        tt |> equal(1, length(info.exclude_globs))
        tt |> equal("data/internal/**", info.exclude_globs[0])
        tt |> equal(2, length(info.forced_modules))
        tt |> equal("dasSQLITE", info.forced_modules[0])
        tt |> equal("dasGlfw", info.forced_modules[1])
        tt |> equal("emcmake cmake -S . -B _wasm && cmake --build _wasm", info.wasm_build_command)
        tt |> equal(2, length(info.wasm_archives))
        tt |> equal("_wasm/liblibDasModuleTest.a", info.wasm_archives[0])
        tt |> equal("_wasm/liblibTestApp.a", info.wasm_archives[1])
        // the two wasm-only knobs cross the same hand-written marshaller: a dropped copy would
        // cross-compile with the module mounted and the flag missing, silently
        tt |> equal(1, length(info.wasm_disabled_modules))
        tt |> equal("dasvulkan", info.wasm_disabled_modules[0])
        tt |> equal(1, length(info.emcc_args))
        tt |> equal("--profiling-funcs", info.emcc_args[0])
    }
    t |> run("returns true with empty spec when release() is missing") @(tt : T?) {
        var info : PackageReleaseInfo
        let ok = run_das_package_release(PKG_FIXTURE, info)
        tt |> success(ok)
        tt |> equal("", info.main_script)
        tt |> equal(0, length(info.include_globs))
    }
}

// --- platform declarations ---

let PKG_PLATFORMS_FIXTURE = "utils/daspkg/fixtures/test_platforms.das_package"

[test(description = "package_platform declarations are read and normalized, and a manifest without one has none")]
def test_run_das_package_platforms(t : T?) {
    t |> run("reads and normalizes package_platform / package_platforms") @(t : T?) {
        var meta : PackageMeta
        t |> success(run_das_package_meta(PKG_PLATFORMS_FIXTURE, meta))
        t |> equal(3, length(meta.platforms))
        t |> equal("windows", meta.platforms[0])
        t |> equal("emscripten", meta.platforms[1])
        t |> equal("darwin", meta.platforms[2])
    }
    t |> run("a manifest without a declaration has none") @(t : T?) {
        var meta : PackageMeta
        t |> success(run_das_package_meta(PKG_FIXTURE, meta))
        t |> equal(0, length(meta.platforms))
    }
    t |> run("vocabulary helpers") @(t : T?) {
        t |> equal("emscripten", normalize_platform_name(" WASM "))
        t |> equal("linux", normalize_platform_name("linux"))
        t |> equal("amiga", normalize_platform_name("amiga"))
        t |> success(is_known_platform("darwin"))
        t |> success(!is_known_platform("amiga"))
        let bad <- unknown_platforms(["windows", "amiga", "beos"])
        t |> equal(2, length(bad))
        t |> equal("amiga", bad[0])
    }
}

[test(description = "the platform gate passes an undeclared or matching host and refuses another host with the sweep skip code, which --force downgrades but an unknown name defeats")]
def test_platform_gate(t : T?) {
    let host = get_platform_name()
    let other = other_platform()
    t |> run("no declaration passes") @(t : T?) {
        let none : array<string>
        t |> equal(0, platform_gate("pkg", host, none, false))
    }
    t |> run("host among the declared passes") @(t : T?) {
        t |> equal(0, platform_gate("pkg", host, [other, host], false))
    }
    t |> run("host outside the declared refuses with the sweep's skip code") @(t : T?) {
        t |> equal(RC_UNSUPPORTED_PLATFORM, platform_gate("pkg", host, [other], false))
    }
    t |> run("--force turns the refusal into a warning") @(t : T?) {
        t |> equal(0, platform_gate("pkg", host, [other], true))
    }
    t |> run("an unknown name is an error even under --force") @(t : T?) {
        t |> equal(1, platform_gate("pkg", host, ["amiga"], true))
    }
}

[test(description = "the SDK gate passes a floor at or below this SDK and refuses a higher one, which --force downgrades but a malformed floor defeats")]
def test_sdk_gate(t : T?) {
    t |> run("no floor passes") @(t : T?) {
        t |> equal(0, sdk_gate("pkg", "", "0.6.4", false))
    }
    t |> run("a floor at or below the SDK passes") @(t : T?) {
        t |> equal(0, sdk_gate("pkg", "0.6.4", "0.6.4", false))
        t |> equal(0, sdk_gate("pkg", "0.4", "0.6.4", false))
    }
    t |> run("a floor above the SDK refuses") @(t : T?) {
        t |> equal(1, sdk_gate("pkg", "0.6.5", "0.6.4", false))
        t |> equal(1, sdk_gate("pkg", "1.0", "0.6.4", false))
    }
    t |> run("--force turns the refusal into a warning") @(t : T?) {
        t |> equal(0, sdk_gate("pkg", "0.6.5", "0.6.4", true))
    }
    t |> run("a floor that is not a version is an error even under --force") @(t : T?) {
        t |> equal(1, sdk_gate("pkg", "latest", "0.6.4", true))
    }
}

[test(description = "the manifest gate passes a directory with no manifest and fails one that does not compile, one with a too-high floor and one for another platform")]
def test_manifest_gate(t : T?) {
    let host = get_platform_name()
    t |> run("a directory without a manifest passes") @(t : T?) {
        let tmp_dir = scratch_dir(t, "daspkg-gate-")
        t |> equal(0, manifest_gate(tmp_dir, "pkg", host, false))
        force_rmdir(tmp_dir)
    }
    t |> run("a manifest that does not compile fails, even under --force") @(t : T?) {
        let tmp_dir = scratch_dir(t, "daspkg-gate-")
        fwrite("{tmp_dir}/.das_package", "this is not daslang\n")
        t |> equal(1, manifest_gate(tmp_dir, "pkg", host, false))
        t |> equal(1, manifest_gate(tmp_dir, "pkg", host, true))
        force_rmdir(tmp_dir)
    }
    t |> run("a floor above this daslang refuses") @(t : T?) {
        let tmp_dir = scratch_dir(t, "daspkg-gate-")
        let no_deps : array<string>
        write_passing_pkg(tmp_dir, "pkg", ["package_min_sdk(\"999.0\")"], no_deps)
        t |> equal(1, manifest_gate(tmp_dir, "pkg", host, false))
        t |> equal(0, manifest_gate(tmp_dir, "pkg", host, true))
        force_rmdir(tmp_dir)
    }
    t |> run("another platform is the sweep's skip code even when the floor also refuses") @(t : T?) {
        let tmp_dir = scratch_dir(t, "daspkg-gate-")
        let no_deps : array<string>
        write_passing_pkg(tmp_dir, "pkg", ["package_min_sdk(\"999.0\")", "package_platform(\"{other_platform()}\")"], no_deps)
        t |> equal(RC_UNSUPPORTED_PLATFORM, manifest_gate(tmp_dir, "pkg", host, false))
        force_rmdir(tmp_dir)
    }
    t |> run("the platform declaration still gates") @(t : T?) {
        let tmp_dir = scratch_dir(t, "daspkg-gate-")
        write_platform_pkg(tmp_dir, "pkg", other_platform())
        t |> equal(RC_UNSUPPORTED_PLATFORM, manifest_gate(tmp_dir, "pkg", host, false))
        force_rmdir(tmp_dir)
    }
}

[test(description = "read_manifest reads a platform declaration and fails on an unknown platform name")]
def test_read_manifest_platforms(t : T?) {
    t |> run("reads a declaration") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_manifest_platforms"
        let no_deps : array<string>
        write_passing_pkg(tmp_dir, "winpkg", ["package_description(\"d\")", "package_platform(\"windows\")"], no_deps)
        var manifest : PackageManifest
        t |> success(read_manifest(tmp_dir, manifest))
        t |> equal(1, length(manifest.platforms))
        t |> equal("windows", manifest.platforms[0])
        force_rmdir(tmp_dir)
    }
    t |> run("an unknown platform name fails the manifest") @(t : T?) {
        let tmp_dir = "{get_das_root()}/_daspkg_test_manifest_platforms"
        let no_deps : array<string>
        write_passing_pkg(tmp_dir, "badpkg", ["package_description(\"d\")", "package_platform(\"amiga\")"], no_deps)
        var manifest : PackageManifest
        t |> success(!read_manifest(tmp_dir, manifest))
        force_rmdir(tmp_dir)
    }
}

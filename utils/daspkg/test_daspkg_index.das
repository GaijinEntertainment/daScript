options gen2
options no_global_variables = false

require dastest/testing_boost public

require daslib/fio
require daslib/clargs
require daslib/defer
require strings
require daslib/strings_boost

require daslib/json
require daslib/json_boost


require utils
require index
require commands
require daslib/result
require package_runner public
require _daspkg_test_common

[test(description = "index_has_push_access accepts only the exact text true, newline tolerated")]
def test_index_has_push_access(t : T?) {
    t |> run("exact true") @(t : T?) {
        t |> success(index_has_push_access("true"))
    }
    t |> run("trailing newline tolerated") @(t : T?) {
        t |> success(index_has_push_access("true\n"))
    }
    t |> run("false is no access") @(t : T?) {
        t |> success(!index_has_push_access("false"))
    }
    t |> run("empty (gh error / no permissions field) is no access") @(t : T?) {
        t |> success(!index_has_push_access(""))
    }
    t |> run("only lowercase true accepted") @(t : T?) {
        t |> success(!index_has_push_access("True"))
    }
}

[test(description = "index_push_target pushes to origin with write access and to a fork with a login-qualified head otherwise")]
def test_index_push_target(t : T?) {
    t |> run("write access → origin + bare branch") @(t : T?) {
        let tgt = index_push_target(true, "octocat", "introduce-minmaxheap")
        t |> equal(tgt.remote, "origin")
        t |> equal(tgt.head_ref, "introduce-minmaxheap")
    }
    t |> run("no access → fork + login-qualified head") @(t : T?) {
        let tgt = index_push_target(false, "JohnathanKG", "introduce-minmaxheap")
        t |> equal(tgt.remote, "fork")
        t |> equal(tgt.head_ref, "JohnathanKG:introduce-minmaxheap")
    }
    t |> run("login ignored when write access") @(t : T?) {
        let tgt = index_push_target(true, "someone", "withdraw-foo")
        t |> equal(tgt.head_ref, "withdraw-foo")
    }
}

[test(description = "add_to_index_json adds a package, keeps the others sorted, and returns an empty index for invalid JSON")]
def test_add_to_index_json(t : T?) {
    t |> run("adds new package") @(t : T?) {
        let result = add_to_index_json("[]", "mypkg", "github.com/user/mypkg", "A cool package")
        t |> success(!empty(result))
        var error : string
        var js = read_json(result, error)
        t |> success(js != null)
        unsafe {
            t |> equal(length(js.value as _array), 1)
            t |> equal((js.value as _array)[0]?.name ?? "", "mypkg")
            t |> equal((js.value as _array)[0]?.url ?? "", "github.com/user/mypkg")
            t |> equal((js.value as _array)[0]?.description ?? "", "A cool package")
        }
    }
    t |> run("preserves existing and sorts") @(t : T?) {
        let input = add_to_index_json("[]", "pkg2", "github.com/user/pkg2", "Second")
        let result = add_to_index_json(input, "pkg1", "github.com/user/pkg1", "First")
        var error : string
        var js = read_json(result, error)
        t |> success(js != null)
        unsafe {
            t |> equal(length(js.value as _array), 2)
            // pkg1 should come before pkg2 (sorted)
            t |> equal((js.value as _array)[0]?.name ?? "", "pkg1")
            t |> equal((js.value as _array)[1]?.name ?? "", "pkg2")
        }
    }
    t |> run("invalid JSON returns empty") @(t : T?) {
        let result = add_to_index_json("not json", "pkg", "url", "desc")
        t |> success(empty(result))
    }
}

[test(description = "remove_from_index_json removes one entry, keeps the others, ignores an absent name and returns empty for invalid JSON")]
def test_remove_from_index_json(t : T?) {
    t |> run("removes existing package") @(t : T?) {
        let input = add_to_index_json("[]", "mypkg", "github.com/user/mypkg", "desc")
        let result = remove_from_index_json(input, "mypkg")
        t |> success(!empty(result))
        var error : string
        var js = read_json(result, error)
        t |> success(js != null)
        unsafe(t |> equal(length(js.value as _array), 0))
    }
    t |> run("preserves other entries") @(t : T?) {
        var input = add_to_index_json("[]", "pkg1", "github.com/user/pkg1", "First")
        input = add_to_index_json(input, "pkg2", "github.com/user/pkg2", "Second")
        let result = remove_from_index_json(input, "pkg1")
        var error : string
        var js = read_json(result, error)
        t |> success(js != null)
        unsafe {
            t |> equal(length(js.value as _array), 1)
            t |> equal((js.value as _array)[0]?.name ?? "", "pkg2")
        }
    }
    t |> run("removing nonexistent is no-op") @(t : T?) {
        let input = add_to_index_json("[]", "pkg1", "github.com/user/pkg1", "desc")
        let result = remove_from_index_json(input, "nonexistent")
        var error : string
        var js = read_json(result, error)
        t |> success(js != null)
        unsafe {
            t |> equal(length(js.value as _array), 1)
            t |> equal((js.value as _array)[0]?.name ?? "", "pkg1")
        }
    }
    t |> run("invalid JSON returns empty") @(t : T?) {
        let result = remove_from_index_json("bad", "pkg")
        t |> success(empty(result))
    }
}

// --- index parsing with rich fields ---

[test(description = "parse_index_json reads every enriched field and defaults the omitted ones")]
def test_parse_index_json_rich(t : T?) {
    t |> run("parses all enriched fields") @(t : T?) {
        let json_text = "[\{\"name\":\"mypkg\",\"url\":\"github.com/user/mypkg\",\"description\":\"desc\",\"author\":\"alice\",\"license\":\"MIT\",\"tags\":[\"ai\",\"http\"],\"min_sdk\":\"0.4\",\"latest_version\":\"v1.0\",\"last_updated\":\"2025-01-01\",\"has_native\":true,\"dependencies\":[\"dep-a\"]\}]"
        var index : table<string; IndexEntry>
        t |> success(parse_index_json(json_text, index))
        t |> success(index |> key_exists("mypkg"))
        unsafe {
            let entry & = index["mypkg"]
            t |> equal("github.com/user/mypkg", entry.url)
            t |> equal("desc", entry.description)
            t |> equal("alice", entry.author)
            t |> equal("MIT", entry.license)
            t |> equal(2, length(entry.tags))
            t |> equal("ai", entry.tags[0])
            t |> equal("http", entry.tags[1])
            t |> equal("0.4", entry.min_sdk)
            t |> equal("v1.0", entry.latest_version)
            t |> equal("2025-01-01", entry.last_updated)
            t |> success(entry.has_native)
            t |> equal(1, length(entry.dependencies))
            t |> equal("dep-a", entry.dependencies[0])
        }
    }
    t |> run("omitted fields default properly") @(t : T?) {
        let json_text = "[\{\"name\":\"bare\",\"url\":\"github.com/user/bare\",\"description\":\"minimal\"\}]"
        var index : table<string; IndexEntry>
        t |> success(parse_index_json(json_text, index))
        unsafe {
            let entry & = index["bare"]
            t |> success(empty(entry.author))
            t |> success(empty(entry.license))
            t |> equal(0, length(entry.tags))
            t |> success(empty(entry.min_sdk))
            t |> success(!entry.has_native)
        }
    }
}

// --- index serialization roundtrip ---

[test(description = "serialize_index round-trips all fields and omits empty optional ones")]
def test_serialize_index_rich(t : T?) {
    t |> run("roundtrip preserves all fields") @(t : T?) {
        let json_text = "[\{\"name\":\"pkg1\",\"url\":\"github.com/u/pkg1\",\"description\":\"first\",\"author\":\"bob\",\"license\":\"Apache-2.0\",\"tags\":[\"game\"],\"min_sdk\":\"0.3\",\"latest_version\":\"v2.0\",\"last_updated\":\"2025-06-01\",\"has_native\":true,\"dependencies\":[\"dep1\"]\}]"
        var index : table<string; IndexEntry>
        t |> success(parse_index_json(json_text, index))
        let serialized = serialize_index(index)
        t |> success(!empty(serialized))
        // re-parse
        var index2 : table<string; IndexEntry>
        t |> success(parse_index_json(serialized, index2))
        t |> success(index2 |> key_exists("pkg1"))
        unsafe {
            let e & = index2["pkg1"]
            t |> equal("bob", e.author)
            t |> equal("Apache-2.0", e.license)
            t |> equal(1, length(e.tags))
            t |> equal("game", e.tags[0])
            t |> equal("0.3", e.min_sdk)
            t |> equal("v2.0", e.latest_version)
            t |> equal("2025-06-01", e.last_updated)
            t |> success(e.has_native)
            t |> equal(1, length(e.dependencies))
        }
    }
    t |> run("omits empty optional fields") @(t : T?) {
        let json_text = "[\{\"name\":\"slim\",\"url\":\"github.com/u/slim\",\"description\":\"minimal\"\}]"
        var index : table<string; IndexEntry>
        t |> success(parse_index_json(json_text, index))
        let serialized = serialize_index(index)
        // should NOT contain "author", "license", etc. keys
        t |> success(find(serialized, "\"author\"") < 0)
        t |> success(find(serialized, "\"license\"") < 0)
        t |> success(find(serialized, "\"tags\"") < 0)
        t |> success(find(serialized, "\"has_native\"") < 0)
    }
}

// --- add_to_index_json_rich ---

[test(description = "add_to_index_json carries the full manifest into the new entry")]
def test_add_to_index_json_rich(t : T?) {
    t |> run("adds package with full manifest") @(t : T?) {
        var manifest = PackageManifest(
            name = "rich",
            description = "A rich package",
            author = "alice",
            license = "MIT",
            min_sdk = "0.4",
            has_native = true
        )
        manifest.tags |> push_clone("ai")
        manifest.dependencies |> push_clone("dep-x")
        let result = add_to_index_json_rich("[]", "rich", manifest, "github.com/alice/rich")
        t |> success(!empty(result))
        var index : table<string; IndexEntry>
        t |> success(parse_index_json(result, index))
        unsafe {
            let e & = index["rich"]
            t |> equal("alice", e.author)
            t |> equal("MIT", e.license)
            t |> equal(1, length(e.tags))
            t |> equal("ai", e.tags[0])
            t |> equal("0.4", e.min_sdk)
            t |> success(e.has_native)
            t |> equal(1, length(e.dependencies))
        }
    }
}

// --- search tests ---

[test(description = "search scoring and filters rank and select index entries")]
def test_search_matches(t : T?) {
    t |> run("search scoring and filters") @(t : T?) {
        // build a small index
        let json_text = "[\{\"name\":\"das-http\",\"url\":\"github.com/u/das-http\",\"description\":\"HTTP client\",\"author\":\"alice\",\"tags\":[\"networking\",\"http\"]\},\{\"name\":\"das-imgui\",\"url\":\"github.com/u/das-imgui\",\"description\":\"ImGui bindings\",\"tags\":[\"gui\"]\}]"
        var index : table<string; IndexEntry>
        parse_index_json(json_text, index)
        unsafe {
            // name substring match
            t |> success(matches_entry("das-http", index["das-http"], "http") < 100)
            // description match
            t |> success(matches_entry("das-imgui", index["das-imgui"], "bindings") < 100)
            // tag: filter — matching tag
            t |> success(matches_entry("das-http", index["das-http"], "tag:networking") < 100)
            // tag: filter — non-matching tag
            t |> equal(100, matches_entry("das-imgui", index["das-imgui"], "tag:networking"))
            // no match returns 100
            t |> equal(100, matches_entry("das-http", index["das-http"], "zzzzzzzzz"))
            // author match
            t |> success(matches_entry("das-http", index["das-http"], "alice") < 100)
        }
    }
}

// --- README generation ---

[test(description = "generate_readme renders tags into markdown and still renders an empty index")]
def test_generate_readme(t : T?) {
    t |> run("generates markdown with tags") @(t : T?) {
        let json_text = "[\{\"name\":\"mypkg\",\"url\":\"github.com/u/mypkg\",\"description\":\"My package\",\"author\":\"bob\",\"license\":\"MIT\",\"tags\":[\"ai\"]\}]"
        let readme = generate_index_readme(json_text)
        t |> success(!empty(readme))
        t |> success(find(readme, "# daspkg package index") >= 0)
        t |> success(find(readme, "mypkg") >= 0)
        t |> success(find(readme, "## Packages by Tag") >= 0)
        t |> success(find(readme, "**ai**") >= 0)
        t |> success(find(readme, "## Package Index") >= 0)
        t |> success(find(readme, "| Package | Description | Install |") >= 0)
        t |> success(find(readme, "**Author:** bob") >= 0)
        t |> success(find(readme, "**License:** MIT") >= 0)
        t |> success(find(readme, "daspkg install mypkg") >= 0)
    }
    t |> run("empty index produces valid readme") @(t : T?) {
        let readme = generate_index_readme("[]")
        t |> success(!empty(readme))
        t |> success(find(readme, "0 package(s) registered") >= 0)
    }
}

[test(description = "the index sweep turns an entry declaring only other platforms into a skip row, never an install", timeout = 1200)]
def test_index_sweep_skips_other_platforms(t : T?) {
    t |> run("an index entry declaring other platforms only is a skip row, never an install") @(t : T?) {
        var cmake_out : string
        if (run_cmd("cmake --version", cmake_out) != 0) {
            t |> skip("cmake not on PATH")
            return
        }
        let tmp = "{get_das_root()}/_daspkg_test_sweep"
        force_rmdir(tmp)
        mkdir(tmp)
        mkdir("{tmp}/root")
        mkdir("{tmp}/out")
        let entry = "[\{\"name\":\"elsewhere\",\"url\":\"github.com/nobody/elsewhere\",\"description\":\"d\",\"platforms\":[\"{other_platform()}\"]\}]"
        fwrite("{tmp}/packages.json", entry)
        var output : string
        let rc = run_cmd("cmake -DDASLANG=\"{get_das_root()}/bin/daslang\" -DROOT=\"{tmp}/root\" -DOUT=\"{tmp}/out\" -DPACKAGES_JSON=\"{tmp}/packages.json\" -P \"{get_das_root()}/utils/daspkg/index_sweep.cmake\"", output)
        t |> equal(0, rc, output)
        t |> success(find(output, "skip elsewhere:") >= 0, output)
        t |> success(!fexist("{tmp}/root/modules/elsewhere"))
        force_rmdir(tmp)
    }
}

def private sweep_fixture_descriptor(package : string; modules : array<string>) : string {
    let rows = [for (name in modules);
        "    register_native_path(\"{package}\", \"{name}\", \"\{project_path\}/{name}.das\")"]
    let head = "options gen2\nrequire daslib/fio\n\n[export]\n"
    return "{head}def initialize(project_path : string) \{\n{join(rows, "\n")}\n\}\n"
}

struct private SweepCompileRun {
    rc : int
    output : string
}

def private run_index_compile(root : string; args : array<string>) : SweepCompileRun {
    let tool = "{get_das_root()}/utils/daspkg/index_compile.das"
    var inscope cmd <- [get_command_line_arguments()[0], "-no-module-cache",
        "-project_root", root, tool, "--"]
    cmd |> push_from(args)
    var run : SweepCompileRun
    run.rc = run_and_capture(cmd, run.output, 600.0)
    return run
}

[test(description = "the sweep's compile step compiles the modules the named package registers and no other package's: a broken one fails it by name, a package registering none passes, and no package name is an error", timeout = 1200)]
def test_index_compile_registered_modules(t : T?) {
    let dir_r = create_temp_directory_result("daspkg_index_compile")
    t |> success(dir_r is value, "temp fixture directory created")
    if (!(dir_r is value)) {
        return
    }
    let root = replace(unsafe(dir_r.value), "\\", "/")
    let pkg = "{root}/modules/sweepfix"
    let other = "{root}/modules/otherfix"
    let nothing = "{root}/modules/emptyfix"
    t |> success(mkdir_rec(pkg) && mkdir_rec(other) && mkdir_rec(nothing))
    let gen2 = "options gen2\n\n"
    t |> success(fwrite("{pkg}/base.das", "{gen2}def public base_answer() : int => 41\n"))
    t |> success(fwrite("{pkg}/good.das",
        "{gen2}require sweepfix/base\n\ndef public good_answer() : int => base_answer() + 1\n"))
    t |> success(fwrite("{pkg}/bad.das", "{gen2}def public bad_answer() : int => no_such_name\n"))
    t |> success(fwrite("{other}/broken.das", "{gen2}def public broken() : int => no_such_name\n"))
    t |> success(fwrite("{other}/.das_module", sweep_fixture_descriptor("otherfix", ["broken"])))
    let no_modules : array<string>
    t |> success(fwrite("{nothing}/.das_module", sweep_fixture_descriptor("emptyfix", no_modules)))
    let sweepfix <- ["--package", "sweepfix", "--project-root", root]
    t |> success(fwrite("{pkg}/.das_module",
        sweep_fixture_descriptor("sweepfix", ["base", "good", "bad"])))
    let broken = run_index_compile(root, sweepfix)
    t |> equal(1, broken.rc, broken.output)
    t |> success(find(broken.output, "bad.das does not compile (exit") >= 0, broken.output)
    t |> success(find(broken.output, "error[") >= 0, "the compiler's error line is quoted")
    t |> success(find(broken.output, "good.das does not compile") < 0, broken.output)
    t |> success(find(broken.output, "3 registered module(s), 1 failed") >= 0, broken.output)
    t |> success(fwrite("{pkg}/.das_module",
        sweep_fixture_descriptor("sweepfix", ["base", "good"])))
    let clean = run_index_compile(root, sweepfix)
    t |> equal(0, clean.rc, "good.das resolves its package's base under the project root, " +
        "and otherfix's broken module is not this package's: {clean.output}")
    let empty_run = run_index_compile(root, ["--package", "emptyfix", "--project-root", root])
    t |> equal(0, empty_run.rc, empty_run.output)
    let no_package : array<string>
    let unnamed = run_index_compile(root, no_package)
    t |> equal(1, unnamed.rc, unnamed.output)
    t |> success(find(unnamed.output, "--package is required") >= 0, unnamed.output)
    t |> success(rmdir_rec(root))
}

[test(description = "index entries parse, serialize, render and refresh their platforms, and an entry without platforms stays without them")]
def test_index_platforms(t : T?) {
    t |> run("parses, serializes and renders platforms") @(t : T?) {
        let json_text = "[\{\"name\":\"winpkg\",\"url\":\"github.com/u/winpkg\",\"description\":\"windows only\",\"platforms\":[\"windows\",\"emscripten\"]\}]"
        var index : table<string; IndexEntry>
        t |> success(parse_index_json(json_text, index))
        unsafe {
            let e & = index["winpkg"]
            t |> equal(2, length(e.platforms))
            t |> equal("windows", e.platforms[0])
            t |> equal("emscripten", e.platforms[1])
        }
        let serialized = serialize_index(index)
        t |> success(find(serialized, "\"platforms\"") >= 0)
        var index2 : table<string; IndexEntry>
        t |> success(parse_index_json(serialized, index2))
        t |> equal(2, unsafe(length(index2["winpkg"].platforms)))
        let readme = generate_index_readme(json_text)
        t |> success(find(readme, "**Platforms:** windows, emscripten") >= 0)
    }
    t |> run("an entry without platforms stays without them") @(t : T?) {
        let json_text = "[\{\"name\":\"bare\",\"url\":\"github.com/u/bare\",\"description\":\"minimal\"\}]"
        var index : table<string; IndexEntry>
        t |> success(parse_index_json(json_text, index))
        t |> equal(0, unsafe(length(index["bare"].platforms)))
        t |> success(find(serialize_index(index), "\"platforms\"") < 0)
        t |> success(find(generate_index_readme(json_text), "**Platforms:**") < 0)
    }
    t |> run("a refresh replaces the entry's platforms with the manifest's, an empty declaration included") @(t : T?) {
        var entry : IndexEntry
        entry.platforms |> push_clone("windows")
        var restricted = PackageManifest(name = "p", description = "d")
        restricted.platforms |> push_clone("linux")
        apply_manifest_to_entry(restricted, entry)
        t |> equal(1, length(entry.platforms))
        t |> equal("linux", entry.platforms[0])
        let unrestricted = PackageManifest(name = "p", description = "d")
        apply_manifest_to_entry(unrestricted, entry)
        t |> equal(0, length(entry.platforms))
    }
    t |> run("introduce carries the manifest's platforms into the entry") @(t : T?) {
        var manifest = PackageManifest(name = "winpkg", description = "windows only")
        manifest.platforms |> push_clone("windows")
        let result = add_to_index_json_rich("[]", "winpkg", manifest, "github.com/u/winpkg")
        var index : table<string; IndexEntry>
        t |> success(parse_index_json(result, index))
        unsafe {
            t |> equal(1, length(index["winpkg"].platforms))
            t |> equal("windows", index["winpkg"].platforms[0])
        }
    }
}

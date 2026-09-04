options gen2

require dastest/testing_boost public
require strings
require daslib/strings_boost
require daslib/fio
require gate

// End-to-end walker coverage against planted fixture repos, spawned exactly as CI runs
// them: the pilot gate's rejection branches, all.das's rc wiring, main.das's gate-red arm.

def write_at(t : T?; root, rel, text : string) {
    let p = path_join(root, rel)
    let d = dir_name(p)
    if (!empty(d)) {
        mkdir_rec(d)
    }
    t |> success(fwrite(p, text), "wrote {rel}")
}

def run_in(cwd_root : string; args : array<string>) : tuple<rc : int; out : string> {
    let prev = getcwd()
    if (!chdir(cwd_root)) {
        return (rc = -1, out = "chdir failed: {cwd_root}")
    }
    var out : string
    let rc = run_and_capture(args, out, 120.0)
    chdir(prev)
    return (rc = rc, out = out)
}

def spawn_at(cwd_root : string; args : array<string>) : tuple<rc : int; out : string> {
    var full : array<string>
    full |> push(walker_self_binary())
    full |> push_from(args)
    return run_in(cwd_root, full)
}

def make_pilot_fixture(t : T?; tree, root : string) {
    // uppercase on purpose: the green run proves the case-insensitive install scan
    write_at(t, root, "CMakeLists.txt", "INSTALL(FILES src/utils/good-tool/main.das DESTINATION utils/good-tool)\n")
    write_at(t, root, "utils/CMakeLists.txt", "set(DAS_UTILS\n    good-tool\n)\nset(DAS_UTILS_SHIPPED_EXES good-tool)\n")
    write_at(t, root, "utils/good-tool/main.das", "options gen2\n")
    write_at(t, root, "utils/good-tool/REVIEW.md", "stays in DAS_UTILS_SHIPPED_EXES\n")
    write_at(t, root, "utils/internal/tidy-tool/main.das", "options gen2\n")
    write_at(t, root, "doc/source/reference/utils/good_tool.rst", "page\n")
    write_at(t, root, "doc/source/reference/utils.rst", "   utils/good_tool.rst\n")
    write_at(t, root, "install/CLAUDE.md", "- `utils/good-tool/` - tool\n- `bin/` - exes (`good-tool.exe`)\n")
    var err : string
    t |> success(copy_file(path_join(tree, "utils/REVIEW.das"), path_join(root, "utils/REVIEW.das"), true, err), "copied the real gate")
}

[test]
def test_pilot_gate_fixtures(t : T?) {
    let tree = getcwd()
    if (!fexist(path_join(tree, "utils/REVIEW.das"))) {
        t |> skip("not run from the repo root")
        return
    }
    let fixture_r = create_temp_directory_result("review_pilot")
    t |> success(fixture_r is value, "temp fixture dir created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    make_pilot_fixture(t, tree, fixture)

    let green = spawn_at(fixture, [path_join(fixture, "utils/REVIEW.das")])
    t |> equal(0, green.rc, "consistent fixture is green: {green.out}")
    t |> success(find(green.out, "REVIEW.das utils: OK") >= 0, "green verdict printed")

    // one violation per check class
    write_at(t, fixture, "utils/internal/Bad_Name/x.txt", "x\n")
    write_at(t, fixture, "utils/rogue-tool/main.das", "options gen2\n")
    write_at(t, fixture, "utils/good-tool/notes.md", "see utils/internal for helpers\n")
    write_at(t, fixture, "utils/CMakeLists.txt", "set(DAS_UTILS\n    good-tool\n)\nset(DAS_UTILS_SHIPPED_EXES good-tool ghost internal/sneak)\n")
    write_at(t, fixture, "install/CLAUDE.md", "- `utils/good-tool/` - tool\n- `bin/` - exes (`good-tool.exe`, `gone.exe`)\n")

    let red = spawn_at(fixture, [path_join(fixture, "utils/REVIEW.das")])
    t |> equal(1, red.rc, "planted fixture is red")
    for (needle in ["not kebab-case", "has no install rule", "no doc/source/reference/utils/rogue_tool.rst",
                    "no toctree row utils/rogue_tool.rst", "no layout row for external tool utils/rogue-tool/",
                    "references utils/internal", "must record the DAS_UTILS_SHIPPED_EXES decision",
                    "does not name ghost.exe", "has no DAS_UTILS entry", "is a path",
                    "is not an external tool directory", "names gone.exe"]) {
        t |> success(find(red.out, needle) >= 0, "finding fired: {needle}")
    }

    t |> success(rmdir_rec_result(fixture) is value, "fixture removed")
}

// The image rail the stamp and tripwire checks read: an IMAGE_VERSION line, two layout
// constants, a layout helper, a `_prepare` one-liner, and the two closure entry points. The
// `PROBE_TAIL` line sits between the one-liner and the next def on purpose - it is outside the
// closure only if a `def ... => expr` window ends on its own header line.
def dasllama_image_text(header_bytes, pad_body, tail : string) : string {
    return "options gen2\n\nlet IMAGE_VERSION = 7\n\nlet IMAGE_PAGE = 16384l\nlet private IMAGE_HEADER_BYTES = {header_bytes}\n\ndef pad_to_page(cur : uint64) : uint64 \{\n    return {pad_body}\n\}\n\ndef private plane_map_prepare() : bool => true\n\nlet PROBE_TAIL = {tail}\n\ndef build_image() \{\n\}\n\ndef parse_image() \{\n\}\n"
}

// A guarded struct serializer plus the bulk payload form the tripwire check licenses: the
// bulk one writes two fields in the call spelling and carries no verify, and takes an array,
// so it is not a meta serializer.
let DASLLAMA_META_GREEN = "options gen2\n\nstruct Thing \{\n    a : int\n    b : array<int>\n\}\n\ndef private serialize_meta(var arch : Archive; var t : Thing) \{\n    verify(count_meta_fields(t) == 2)\n    serialize(arch, t.a)\n    serialize_pod_array(arch, t.b)\n\}\n\ndef private serialize_bulk(var arch : Archive; var a : array<int>) \{\n    serialize_pod_array(arch, a)\n    serialize_pod_array(arch, a)\n\}\n"

// the same struct serializer with the tripwire dropped - both field writes are call-spelled,
// which the pipe-form-only count missed
let DASLLAMA_META_RED = "options gen2\n\nstruct Thing \{\n    a : int\n    b : array<int>\n\}\n\ndef private serialize_meta(var arch : Archive; var t : Thing) \{\n    serialize(arch, t.a)\n    serialize_pod_array(arch, t.b)\n\}\n"

def make_dasllama_fixture(t : T?; tree, root : string) {
    write_at(t, root, "modules/dasLLAMA/dasllama/dasllama.das", "options gen2\n\ndef covered_fn() \{\n\}\n")
    write_at(t, root, "modules/dasLLAMA/dasllama/dasllama_audio_embedder.das", "enum AudioKind \{\n    none\n    toyfam\n\}\n")
    write_at(t, root, "modules/dasLLAMA/dasllama/dasllama_vision_embedder.das", "enum VisionKind \{\n    none\n    toyvis\n\}\n")
    write_at(t, root, "modules/dasLLAMA/dasllama/dasllama_toyfam.das", "struct ToyfamThing \{\n\}\n\ntypedef ToyfamFn = int\n\ndef set_toyfam_q8(on : bool) \{\n\}\n")
    write_at(t, root, "modules/dasLLAMA/dasllama/dasllama_toyvis.das", "struct ToyvisTower \{\n\}\n")
    write_at(t, root, "modules/dasLLAMA/dasllama/dasllama_image.das", dasllama_image_text("64l", "cur", "1"))
    write_at(t, root, "modules/dasLLAMA/dasllama/dasllama_meta.das", DASLLAMA_META_GREEN)
    write_at(t, root, "modules/dasLLAMA/.das_module",
        "\"dasllama\", \"dasllama_audio_embedder\", \"dasllama_vision_embedder\", \"dasllama_toyfam\", \"dasllama_toyvis\", \"dasllama_image\", \"dasllama_meta\"\n")
    write_at(t, root, "modules/dasLLAMA/REVIEW.md", "rules\n")
    write_at(t, root, "tutorials/dasLLAMA/01_demo.das", "covered_fn()\n// prose beside the call stays harmless\n")
    write_at(t, root, "doc/source/reference/tutorials/dasLLAMA_01_demo.rst", "``covered_fn`` narrated\n")
    var err : string
    t |> success(copy_file(path_join(tree, "modules/dasLLAMA/REVIEW.das"), path_join(root, "modules/dasLLAMA/REVIEW.das"), true, err), "copied the real dasllama gate")
}

// The stamp is over the fixture's OWN closure, so the shipped constants cannot match it. The
// re-stamp takes the value the finding itself prints - which is also what proves the finding
// prints a usable one.
def restamp_dasllama_gate(t : T?; root : string) {
    let gate = path_join(root, "modules/dasLLAMA/REVIEW.das")
    let probe = spawn_at(root, [gate])
    t |> equal(1, probe.rc, "the planted closure does not match the shipped stamp: {probe.out}")
    let marker = "IMAGE_LAYOUT_STAMP_HASH = "
    let at = find(probe.out, marker)
    t |> success(at >= 0, "the stamp finding prints the hash to re-stamp with: {probe.out}")
    return if (at < 0)
    let from = at + length(marker)
    let hash = slice(probe.out, from, find(probe.out, "ul", from))
    var lines : array<string>
    for (line in split(fread(gate), "\n")) {
        if (line |> starts_with("let private IMAGE_LAYOUT_STAMP_HASH = ")) {
            lines |> push("let private IMAGE_LAYOUT_STAMP_HASH = {hash}ul")
        } elif (line |> starts_with("let private IMAGE_LAYOUT_STAMP_VERSION = ")) {
            lines |> push("let private IMAGE_LAYOUT_STAMP_VERSION = 7")
        } else {
            lines |> push(line)
        }
    }
    t |> success(fwrite(gate, join(lines, "\n")), "re-stamped the fixture's gate copy")
}

[test]
def test_dasllama_gate_fixtures(t : T?) {
    let tree = getcwd()
    if (!fexist(path_join(tree, "modules/dasLLAMA/REVIEW.das"))) {
        t |> skip("not run from the repo root")
        return
    }
    let fixture_r = create_temp_directory_result("review_dasllama")
    t |> success(fixture_r is value, "temp fixture dir created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    make_dasllama_fixture(t, tree, fixture)
    restamp_dasllama_gate(t, fixture)

    let green = spawn_at(fixture, [path_join(fixture, "modules/dasLLAMA/REVIEW.das")])
    t |> equal(0, green.rc, "covered fixture is green: {green.out}")
    t |> success(find(green.out, "REVIEW.das dasllama: OK") >= 0, "green verdict printed")

    // a line after a `def ... => expr` one-liner is outside the layout closure - the window
    // ends on the header, so the stamp does not move
    write_at(t, fixture, "modules/dasLLAMA/dasllama/dasllama_image.das", dasllama_image_text("64l", "cur", "2"))
    let after_tail = spawn_at(fixture, [path_join(fixture, "modules/dasLLAMA/REVIEW.das")])
    t |> equal(0, after_tail.rc, "a line the one-liner's window must not absorb left the stamp alone: {after_tail.out}")

    // one violation per miss class - the needle list is the census
    write_at(t, fixture, "modules/dasLLAMA/dasllama/dasllama.das",
        "options gen2\n\ndef covered_fn() \{\n\}\n\ndef hidden_fn() \{\n\}\n\ndef bound_fn() \{\n\}\n")
    write_at(t, fixture, "tutorials/dasLLAMA/02_demo.das", "// bound_fnx is a different identifier\n// hidden_fn taught only in this comment must not count\n")
    write_at(t, fixture, "modules/dasLLAMA/dasllama/dasllama_audio_embedder.das", "enum AudioKind \{\n    none\n    toyfam\n    ghostfam\n\}\n")
    write_at(t, fixture, "modules/dasLLAMA/benchmarks/asr/bench_probe.das", "set_toyfam_q8(true)\nvar fn : ToyfamFn\n")
    write_at(t, fixture, "tutorials/dasLLAMA/03_vis.das", "var tw : ToyvisTower\n")
    write_at(t, fixture, "modules/dasLLAMA/dasllama/dasllama_unlisted.das", "options gen2\n")
    write_at(t, fixture, "modules/dasLLAMA/.das_module",
        "\"dasllama\", \"dasllama_audio_embedder\", \"dasllama_vision_embedder\", \"dasllama_toyfam\", \"dasllama_gone\", \"dasllama_image\", \"dasllama_meta\"\n// \"dasllama_toyvis\" commented out must not count\n")
    // a moved layout constant and a changed layout helper, with the stamp left where it was
    write_at(t, fixture, "modules/dasLLAMA/dasllama/dasllama_image.das", dasllama_image_text("128l", "cur + 1ul", "2"))
    write_at(t, fixture, "modules/dasLLAMA/dasllama/dasllama_meta.das", DASLLAMA_META_RED)

    let red = spawn_at(fixture, [path_join(fixture, "modules/dasLLAMA/REVIEW.das")])
    t |> equal(1, red.rc, "planted fixture is red")
    for (needle in ["dasllama_image.das: the image layout closure changed",
                    "dasllama_meta.das:8: hand-listed Archive serializer with 2 field writes",
                    "facade def hidden_fn appears in no tutorials/dasLLAMA/*.das",
                    "facade def hidden_fn appears in no doc/source/reference/tutorials/dasLLAMA_*.rst",
                    "facade def bound_fn appears in no tutorials/dasLLAMA/*.das",
                    "bench_probe.das: names set_toyfam_q8, which the dasllama_toyfam family owns",
                    "bench_probe.das: names ToyfamFn, which the dasllama_toyfam family owns",
                    "03_vis.das: names ToyvisTower, which the dasllama_toyvis family owns",
                    "AudioKind arm dasllama_ghostfam has no modules/dasLLAMA/dasllama/dasllama_ghostfam.das",
                    "dasllama_unlisted.das: absent from modules/dasLLAMA/.das_module",
                    "dasllama_toyvis.das: absent from modules/dasLLAMA/.das_module",
                    "names dasllama_gone, but no census dir (modules/dasLLAMA/dasllama) holds dasllama_gone.das"]) {
        t |> success(find(red.out, needle) >= 0, "finding fired: {needle}")
    }

    t |> success(rmdir_rec_result(fixture) is value, "fixture removed")
}

def make_vulkan_fixture(t : T?; tree, root : string) {
    write_at(t, root, "modules/dasVulkan/daslib/vulkan_toy.das", "options gen2\n")
    write_at(t, root, "modules/dasVulkan/daslib/vulkan_toy_boost.das", "options gen2\n")
    write_at(t, root, "modules/dasVulkan/.das_module",
        "\"vulkan\", \"Module_dasVULKAN\", \"vulkan_toy\", \"vulkan_toy_boost\"\n")
    write_at(t, root, "modules/dasVulkan/REVIEW.md", "rules\n")
    var err : string
    t |> success(copy_file(path_join(tree, "modules/dasVulkan/REVIEW.das"), path_join(root, "modules/dasVulkan/REVIEW.das"), true, err), "copied the real dasvulkan gate")
}

[test]
def test_vulkan_gate_fixtures(t : T?) {
    let tree = getcwd()
    if (!fexist(path_join(tree, "modules/dasVulkan/REVIEW.das"))) {
        t |> skip("not run from the repo root")
        return
    }
    let fixture_r = create_temp_directory_result("review_vulkan")
    t |> success(fixture_r is value, "temp fixture dir created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    make_vulkan_fixture(t, tree, fixture)

    let green = spawn_at(fixture, [path_join(fixture, "modules/dasVulkan/REVIEW.das")])
    t |> equal(0, green.rc, "covered fixture is green (category + DLL-class tokens ignored): {green.out}")
    t |> success(find(green.out, "REVIEW.das dasvulkan: OK") >= 0, "green verdict printed")

    write_at(t, fixture, "modules/dasVulkan/daslib/vulkan_unlisted.das", "options gen2\n")
    write_at(t, fixture, "modules/dasVulkan/.das_module",
        "\"vulkan\", \"Module_dasVULKAN\", \"vulkan_toy\", \"vulkan_gone\"\n// \"vulkan_toy_boost\" commented out must not count\n")

    let red = spawn_at(fixture, [path_join(fixture, "modules/dasVulkan/REVIEW.das")])
    t |> equal(1, red.rc, "planted fixture is red")
    for (needle in ["vulkan_unlisted.das: absent from modules/dasVulkan/.das_module",
                    "vulkan_toy_boost.das: absent from modules/dasVulkan/.das_module",
                    "names vulkan_gone, but no census dir (modules/dasVulkan/daslib) holds vulkan_gone.das"]) {
        t |> success(find(red.out, needle) >= 0, "finding fired: {needle}")
    }

    t |> success(rmdir_rec_result(fixture) is value, "fixture removed")
}

// Every write here lands in a temp copy: the tracked fixture dir is only ever READ, and the
// name never reaches a traced variable a write line mentions.
let LLVM_TESTS_GREEN = "options gen2\n\nlet private WORK_DEFAULTS = \"tune_defaults\"\n\ndef private copy_one(name, dstdir : string) \{\n    let text = fread(\"\{get_this_module_dir()\}/\{name\}\")\n    fwrite(path_join(dstdir, name), text)\n\}\n\ndef private stage(work : string) \{\n    let dst = path_join(work, WORK_DEFAULTS)\n    mkdir_rec(dst)\n    copy_one(\"arm64.tune-defaults.json\", dst)\n    remove(path_join(dst, \"arm64.tune-defaults.json\"))\n\}\n\ndef private read_tracked() : string \{\n    return fread(\"\{get_this_module_dir()\}/llvm_tune_profiles_defaults/arm64.tune-defaults.json\")\n\}\n"

def make_llvm_fixture(t : T?; tree, root : string) {
    write_at(t, root, "modules/dasLLVM/daslib/llvm_toy.das", "options gen2\n")
    write_at(t, root, "modules/dasLLVM/bindings/llvm_toy_bind.das", "options gen2\n")
    write_at(t, root, "modules/dasLLVM/.das_module", "\"llvm\", \"llvm_toy\", \"llvm_toy_bind\"\n")
    write_at(t, root, "modules/dasLLVM/REVIEW.md", "rules\n")
    write_at(t, root, "modules/dasLLVM/tests/llvm_tune_profiles_defaults/arm64.tune-defaults.json", "\{\}\n")
    write_at(t, root, "modules/dasLLVM/tests/llvm_toy_profiles.das", LLVM_TESTS_GREEN)
    var err : string
    t |> success(copy_file(path_join(tree, "modules/dasLLVM/REVIEW.das"), path_join(root, "modules/dasLLVM/REVIEW.das"), true, err), "copied the real dasllvm gate")
}

[test]
def test_llvm_gate_fixtures(t : T?) {
    let tree = getcwd()
    if (!fexist(path_join(tree, "modules/dasLLVM/REVIEW.das"))) {
        t |> skip("not run from the repo root")
        return
    }
    let fixture_r = create_temp_directory_result("review_llvm")
    t |> success(fixture_r is value, "temp fixture dir created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    make_llvm_fixture(t, tree, fixture)

    let green = spawn_at(fixture, [path_join(fixture, "modules/dasLLVM/REVIEW.das")])
    t |> equal(0, green.rc, "temp-copy fixture is green (the tracked dir is read, never written): {green.out}")
    t |> success(find(green.out, "REVIEW.das dasLLVM: OK") >= 0, "green verdict printed")

    // one violation per way a write reaches the tracked dir, one file each so the finding's
    // path names which way fired
    write_at(t, fixture, "modules/dasLLVM/tests/llvm_red_direct.das",
        "options gen2\ndef private plant(body : string) \{\n    fwrite(\"\{get_this_module_dir()\}/llvm_tune_profiles_defaults/x.json\", body)\n\}\n")
    write_at(t, fixture, "modules/dasLLVM/tests/llvm_red_traced.das",
        "options gen2\nlet private TRACKED = \"llvm_tune_profiles_defaults\"\ndef private plant() \{\n    remove(path_join(get_this_module_dir(), TRACKED))\n\}\n")
    write_at(t, fixture, "modules/dasLLVM/tests/llvm_red_helper.das",
        "options gen2\ndef private copy_one(name, dstdir : string) \{\n    fwrite(path_join(dstdir, name), fread(name))\n\}\ndef private plant() \{\n    copy_one(\"x.json\", \"\{get_this_module_dir()\}/llvm_tune_profiles_defaults\")\n\}\n")
    write_at(t, fixture, "modules/dasLLVM/tests/llvm_red_fopen.das",
        "options gen2\ndef private plant() \{\n    fopen(\"\{get_this_module_dir()\}/llvm_tune_profiles_defaults/x.json\", \"wb\") $(f) \{\n    \}\n\}\n")
    write_at(t, fixture, "modules/dasLLVM/daslib/llvm_unlisted.das", "options gen2\n")
    write_at(t, fixture, "modules/dasLLVM/.das_module",
        "\"llvm\", \"llvm_toy\", \"llvm_gone\"\n// \"llvm_toy_bind\" commented out must not count\n")

    let red = spawn_at(fixture, [path_join(fixture, "modules/dasLLVM/REVIEW.das")])
    t |> equal(1, red.rc, "planted fixture is red")
    let tracked_msg = "writes or removes a path under modules/dasLLVM/tests/llvm_tune_profiles_defaults, which git tracks"
    for (needle in ["llvm_red_direct.das:3: {tracked_msg}",
                    "llvm_red_traced.das:4: {tracked_msg}",
                    "llvm_red_helper.das:6: {tracked_msg}",
                    "llvm_red_fopen.das:3: {tracked_msg}",
                    "llvm_unlisted.das: absent from modules/dasLLVM/.das_module",
                    "llvm_toy_bind.das: absent from modules/dasLLVM/.das_module",
                    "names llvm_gone, but no census dir (modules/dasLLVM/daslib, modules/dasLLVM/bindings) holds llvm_gone.das"]) {
        t |> success(find(red.out, needle) >= 0, "finding fired: {needle}")
    }

    t |> success(rmdir_rec_result(fixture) is value, "fixture removed")
}

def make_server_fixture(t : T?; tree, root : string) {
    write_at(t, root, "utils/dasllama-server/openai_server.das",
        "options gen2\n\nclass X \{\n    def onInit \{\n        GET(\"/v1/models\") <| $() \{\n        \}\n        POST(\"/gc\") <| $() \{\n        \}\n        STREAM(\"/sse\") <| $() \{\n        \}\n        ANY(\"*\") <| $() \{\n        \}\n    \}\n\}\n")
    write_at(t, root, "utils/dasllama-server/README.md", "routes: `/v1/models` and `/gc` and `/sse`\n")
    write_at(t, root, "doc/source/reference/utils/dasllama_server.rst", "``/v1/models`` and ``/gc`` and ``/sse``\n")
    write_at(t, root, "utils/dasllama-server/control.html", "<script>fetch(\"/gc\")</script>\n")
    var err : string
    t |> success(copy_file(path_join(tree, "utils/dasllama-server/REVIEW.das"), path_join(root, "utils/dasllama-server/REVIEW.das"), true, err), "copied the real server gate")
}

[test]
def test_server_gate_fixtures(t : T?) {
    let tree = getcwd()
    if (!fexist(path_join(tree, "utils/dasllama-server/REVIEW.das"))) {
        t |> skip("not run from the repo root")
        return
    }
    let fixture_r = create_temp_directory_result("review_server")
    t |> success(fixture_r is value, "temp fixture dir created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    make_server_fixture(t, tree, fixture)

    let green = spawn_at(fixture, [path_join(fixture, "utils/dasllama-server/REVIEW.das")])
    t |> equal(0, green.rc, "consistent fixture is green: {green.out}")
    t |> success(find(green.out, "REVIEW.das dasllama-server: OK") >= 0, "green verdict printed")

    // one violation per direction - the needle list is the census
    write_at(t, fixture, "utils/dasllama-server/openai_server.das",
        "options gen2\n\nclass X \{\n    def onInit \{\n        GET(\"/v1/models\") <| $() \{\n        \}\n        POST(\"/gc\") <| $() \{\n        \}\n        STREAM(\"/sse\") <| $() \{\n        \}\n        POST(\"/vad\") <| $() \{\n        \}\n        ANY(\"/anyroute\") <| $() \{\n        \}\n        ANY(\"*\") <| $() \{\n        \}\n    \}\n\}\n")
    write_at(t, fixture, "utils/dasllama-server/README.md", "routes: `/v1/models` and `/gc` and `/sse` and `/dead`\n")
    write_at(t, fixture, "doc/source/reference/utils/dasllama_server.rst", "``/v1/models`` and ``/gc`` and ``/sse`` and ``/rstdead``\n")
    write_at(t, fixture, "utils/dasllama-server/control.html", "<script>fetch(\"/gc\");fetch(\"/nope\")</script>\n")

    let red = spawn_at(fixture, [path_join(fixture, "utils/dasllama-server/REVIEW.das")])
    t |> equal(1, red.rc, "planted fixture is red")
    for (needle in ["served route /vad has no `/vad` mention",
                    "served route /vad has no ``/vad`` mention",
                    "served route /anyroute has no `/anyroute` mention",
                    "README.md: documents route /dead, which the server does not register",
                    "dasllama_server.rst: documents route /rstdead, which the server does not register",
                    "the control page consumes route /nope, which the server does not register"]) {
        t |> success(find(red.out, needle) >= 0, "finding fired: {needle}")
    }

    t |> success(rmdir_rec_result(fixture) is value, "fixture removed")
}

[test]
def test_all_and_main_walker_wiring(t : T?) {
    let tree = getcwd()
    let all_das = path_join(tree, "utils/internal/review-md/all.das")
    let main_das = path_join(tree, "utils/internal/review-md/main.das")
    if (!fexist(all_das)) {
        t |> skip("not run from the repo root")
        return
    }
    let fixture_r = create_temp_directory_result("review_walk")
    t |> success(fixture_r is value, "temp fixture dir created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    let git = run_in(fixture, ["git", "init", "-q"])
    if (git.rc != 0) {
        t |> skip("git init unavailable: {git.out}")
        return
    }
    let green_gate = "options gen2\n\n[export]\ndef main() : int \{\n    return 0\n\}\n"
    let red_gate = "options gen2\n\n[export]\ndef main() : int \{\n    return 1\n\}\n"
    write_at(t, fixture, "a/REVIEW.md", "rules\n")
    write_at(t, fixture, "a/REVIEW.das", green_gate)

    let ok = spawn_at(fixture, [all_das])
    t |> equal(0, ok.rc, "green tree passes: {ok.out}")
    t |> success(find(ok.out, "1 gate(s), 0 red, 0 orphan(s)") >= 0, "summary line printed")

    write_at(t, fixture, "b/REVIEW.das", green_gate)   // gate with no checklist
    let orphaned = spawn_at(fixture, [all_das])
    t |> equal(1, orphaned.rc, "orphan turns the walk red")
    t |> success(find(orphaned.out, "b/REVIEW.das: REVIEW.das without a sibling REVIEW.md") >= 0, "orphan named")

    write_at(t, fixture, "a/REVIEW.das", red_gate)
    let failing = spawn_at(fixture, [all_das])
    t |> equal(1, failing.rc, "red gate turns the walk red")
    t |> success(find(failing.out, "1 red") >= 0, "red count in summary")

    // main.das: positional changed path, gate red -> exit 1 and no listing
    let gated = spawn_at(fixture, [main_das, "--", "a/x.das"])
    t |> equal(1, gated.rc, "red gate blocks the listing")
    t |> success(find(gated.out, "gate(s) red") >= 0, "fail-fix message printed")

    let listed = spawn_at(fixture, [main_das, "--", "a/x.das", "--list-only"])
    t |> equal(0, listed.rc, "--list-only ignores the red gate")
    t |> success(find(listed.out, "a/REVIEW.md") >= 0, "checklist listed")

    write_at(t, fixture, "a/REVIEW.das", green_gate)
    let clean = spawn_at(fixture, [main_das, "--", "a/x.das"])
    t |> equal(0, clean.rc, "green gate lets the listing through")
    t |> success(find(clean.out, "a/REVIEW.md") >= 0, "checklist listed after green gate")

    // a changed orphan REVIEW.das is a structural defect — red even under --list-only
    let orphan_hit = spawn_at(fixture, [main_das, "--", "b/REVIEW.das"])
    t |> equal(1, orphan_hit.rc, "changed orphan gate fails the walk")
    t |> success(find(orphan_hit.out, "without a sibling REVIEW.md") >= 0, "orphan named by main.das")
    let orphan_list = spawn_at(fixture, [main_das, "--", "b/REVIEW.das", "--list-only"])
    t |> equal(1, orphan_list.rc, "--list-only does not bypass the orphan check")

    t |> success(rmdir_rec_result(fixture) is value, "fixture removed")
}

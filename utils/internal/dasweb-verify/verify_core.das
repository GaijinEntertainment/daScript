options gen2
options indenting = 4
options rtti

module verify_core public

require daslib/ast
require daslib/rtti
require daslib/fio
require daslib/json_boost

//! Tier 1 of the curated-sample verifier: parse the playground's shipped
//! manifest (data.json) and compile-check every sample with the host binary's
//! own compiler — the same in-process rail the MCP compile_check subtool uses.
//! Zero network; the browser/nightly leg against daslang.io is a separate tool.

struct SampleEntry {
    name : string
    files : array<string>
}

struct SamplesManifest {
    examples : array<SampleEntry>
}

struct CheckResult {
    name : string
    entry : string
    ok : bool
    message : string
}

//! Parse the playground manifest. Fails closed: a missing/unparseable file, an
//! empty sample list, or a sample without files yields an error naming the
//! offender and an empty list.
def load_manifest(path : string; var error : string&) : array<SampleEntry> {
    error = ""
    var empty_list : array<SampleEntry>
    if (!stat(path).is_reg) {
        error = "manifest not found: {path}"
        return <- empty_list
    }
    var manifest = SamplesManifest()
    if (!sscan_json(fread(path), manifest)) {
        error = "manifest does not parse as JSON: {path}"
        return <- empty_list
    }
    if (empty(manifest.examples)) {
        error = "manifest lists no samples: {path}"
        return <- empty_list
    }
    for (e in manifest.examples) {
        if (empty(e.files)) {
            error = "sample \"{e.name}\" lists no files"
            return <- empty_list
        }
    }
    return <- manifest.examples
}

// The dasImgui showcase samples are generated into the staged site tree by
// web/stage_playground_imgui_samples.cmake — no committed copies under the
// samples root. The verifier compiles their canonical sources instead (in their
// real directories, so path_tracer_lab's cross-tree require resolves). This
// mapping mirrors that script; a change to one without the other is a defect.
let GENERATED_SAMPLES : table<string; string> <- {
    "examples/furier.das" => "examples/graphics/furier_opengl_imgui_example.das",
    "examples/path_tracer_lab/main.das" => "examples/graphics/path_tracer_lab_opengl_imgui_example.das",
    "examples/path_tracer_lab/path_tracer.das" => "examples/pathTracer/path_tracer.das",
    "examples/physarum_lab.das" => "examples/graphics/physarum_lab_opengl_imgui_example.das"
}

//! Resolve one manifest-listed file: committed under root, or the canonical
//! source of a stage-time-generated sample under repo_root. Empty + error set
//! when neither exists.
def resolve_listed_file(root : string; repo_root : string; f : string; var error : string&) : string {
    let committed = path_join(root, f)
    if (stat(committed).is_reg) {
        return committed
    }
    let canonical = GENERATED_SAMPLES?[f] ?? ""
    if (empty(canonical)) {
        error = "listed file not found: {f}"
        return ""
    }
    let cpath = path_join(repo_root, canonical)
    if (!stat(cpath).is_reg) {
        error = "generated sample's canonical source not found: {f} -> {canonical}"
        return ""
    }
    return cpath
}

//! Compile-check one sample: every listed file must resolve, and the entry
//! (first file) must compile and simulate. Companion files are reached through
//! the entry's own requires.
def check_sample(root : string; repo_root : string; sample : SampleEntry) : CheckResult {
    var result = CheckResult(name = sample.name, entry = sample.files[0])
    var entry_path = ""
    for (f in sample.files) {
        let resolved = resolve_listed_file(root, repo_root, f, result.message)
        if (empty(resolved)) {
            return result
        }
        if (empty(entry_path)) {
            entry_path = resolved
        }
    }
    compile_das_file(entry_path, result)
    return result
}

def private compile_das_file(path : string; var result : CheckResult) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.ignore_shared_modules = true
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    result.message = "compilation failed:\n{issues}"
                } else {
                    simulate(program) $(sok; _ctx; serrors) {
                        if (!sok) {
                            result.message = "simulation failed:\n{serrors}"
                        } else {
                            result.ok = true
                        }
                    }
                }
            }
        }
    }
}

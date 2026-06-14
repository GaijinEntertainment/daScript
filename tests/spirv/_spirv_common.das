options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

// Shared fixtures + gates for the dasSpirv test suite. The Phase-1 `[compute_shader] square`
// lives here so its emitted blob (`square_spv`) has a single source shared by the opcode-shape,
// census, and spirv-val tests. validate_spirv shells out to SPIRV-Tools' spirv-val (the real
// correctness oracle); it soft-skips when the tool is absent locally and CI enforces presence.

module _spirv_common shared public

require spirv/spirv_shader
require spirv/spirv_grammar
require spirv/spirv_builtins public
require daslib/fio

// ===== Phase-1 fixture: data[i] = i*i, captured into the `square_spv : array<uint>` global =====
var @ssbo @binding = 0 data : array<uint>

// [marker(no_coverage)] keeps daslib/coverage from instrumenting the shader body: it is
// compile-time SPIR-V source, not runtime code, and the injected coverageData/ExprCall nodes
// are not lowerable by the emitter (they'd trip the annotation patch under --cov-path).
[compute_shader(local_size_x=64, name="square_spv"), marker(no_coverage)]
def square {
    let i = gl_GlobalInvocationID.x
    data[i] = i * i
}

// A fresh, caller-owned copy of the captured words (the global itself must not be mutated/moved).
def public square_words : array<uint> {
    var w : array<uint>
    w |> push_from(square_spv)
    return <- w
}

// ===== spirv-val gate =====
struct SpirvValResult {
    ran : bool      // false => tool not found (soft-skip locally; CI provides it)
    ok  : bool
    msg : string
}

def private tool_runs(exe : string) : bool {
    var out : string
    return run_and_capture([exe, "--version"], out, 10.0) == 0
}

def private find_spirv_val : string {
    var cands : array<string>
    if (has_env_variable("VULKAN_SDK")) {
        let sdk = get_env_variable("VULKAN_SDK")
        cands |> push("{sdk}/Bin/spirv-val")           // Windows SDK layout
        cands |> push("{sdk}/Bin/spirv-val.exe")
        cands |> push("{sdk}/bin/spirv-val")           // Linux SDK layout
    }
    cands |> push("spirv-val")                          // PATH (CI)
    cands |> push("spirv-val.exe")
    var found = ""
    for (p in cands) {
        if (tool_runs(p)) {
            found = p
            break
        }
    }
    delete cands
    return found
}

def public validate_spirv(words : array<uint>; target_env : string = "vulkan1.1") : SpirvValResult {
    let exe = find_spirv_val()
    if (empty(exe)) return SpirvValResult(ran = false, ok = false, msg = "spirv-val not found")
    let tf = create_temp_file_result("das_spirv", ".spv")
    if (tf is error) return SpirvValResult(ran = true, ok = false, msg = "temp file: {tf as error}")
    let path = tf as value
    var wrote = false
    fopen(path, "wb") $(f) {
        if (f != null) {
            fwrite(f, words)
            wrote = true
        }
    }
    if (!wrote) {
        remove_result(path)
        return SpirvValResult(ran = true, ok = false, msg = "write failed: {path}")
    }
    var out : string
    let rc = run_and_capture([exe, "--target-env", target_env, path], out, 30.0)
    remove_result(path)
    return SpirvValResult(ran = true, ok = rc == 0, msg = out)
}

// ===== opcode census: the EXACT set generate_spirv emits for the Phase-1 square =====
// Gate B (stronger than LCOV): a meta-test asserts the emitted set equals this declared set in
// BOTH directions — catches an opcode emitted but never asserted, and a declared opcode that
// silently stopped being emitted.
def public phase1_emitter_opcodes : table<uint> {
    var s : table<uint>
    for (op in [
        SpvOp.Capability, SpvOp.MemoryModel, SpvOp.EntryPoint, SpvOp.ExecutionMode,
        SpvOp.Decorate, SpvOp.MemberDecorate,
        SpvOp.TypeInt, SpvOp.TypeVector, SpvOp.TypeVoid, SpvOp.TypeFunction,
        SpvOp.TypeRuntimeArray, SpvOp.TypeStruct, SpvOp.TypePointer,
        SpvOp.Constant, SpvOp.Variable,
        SpvOp.Function, SpvOp.Label, SpvOp.AccessChain, SpvOp.Load, SpvOp.IMul, SpvOp.Store,
        SpvOp.Return, SpvOp.FunctionEnd
    ]) {
        s |> insert(uint(op))
    }
    return <- s
}

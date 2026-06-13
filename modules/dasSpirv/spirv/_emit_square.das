options gen2

// PHASE-1 end-to-end scaffold (temporary): the real [compute_shader] square, lowered to SPIR-V
// by the AST emitter via the annotation's fixup, captured into the `square_spv` global. main reads
// that global, asserts the opcode shape, dumps the disassembly, and writes the .spv for spirv-val.
// Oracle: structurally equivalent to the hand-built _square_typed.spv + spirv-val clean.

require spirv_shader
require spirv_dis
require spirv_grammar
require daslib/fio

var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="square_spv")]
def square {
    let i = gl_GlobalInvocationID.x
    data[i] = i * i
}

[export]
def main {
    var out_path = "emit_square.spv"
    let cargs <- get_command_line_arguments()
    if (length(cargs) > 0) {
        out_path = cargs[length(cargs) - 1]
    }

    // square_spv is the captured SPIR-V words global; copy into a mutable local for the helpers
    var words : array<uint>
    words |> push_from(square_spv)

    var ok = true
    ok = ok && length(words) > 5
    ok = ok && words[0] == SPV_MAGIC
    ok = ok && count_op(words, SpvOp.EntryPoint) == 1
    ok = ok && has_op(words, SpvOp.ExecutionMode)
    ok = ok && count_op(words, SpvOp.Variable) == 2          // gid (Input) + data (StorageBuffer)
    ok = ok && count_op(words, SpvOp.AccessChain) == 2       // gid.x + data[i]
    ok = ok && count_op(words, SpvOp.Load) == 1              // load gid.x
    ok = ok && count_op(words, SpvOp.IMul) == 1              // i * i
    ok = ok && count_op(words, SpvOp.Store) == 1             // data[i] = ...
    ok = ok && has_op(words, SpvOp.Return)
    ok = ok && op_has_operand(words, SpvOp.Decorate, uint(SpvBuiltIn.GlobalInvocationId))

    print("{dump(words)}\n---\n")
    print("emit square: {ok ? "PASS" : "FAIL"}\n")
    print("words: {length(words)}, id bound: {words[3]}\n")

    fopen(out_path, "wb") <| $(f) {
        if (f != null) {
            fwrite(f, words)
        }
    }
    print("wrote {out_path}\n")
    delete words
}

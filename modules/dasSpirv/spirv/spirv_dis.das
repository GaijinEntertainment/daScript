options gen2

// Minimal SPIR-V disassembler + opcode-census helpers. The binary is a self-delimiting word
// stream: word0 of each instruction is (wordCount << 16) | opcode, so the whole module walks
// with zero per-opcode knowledge. Symbolic opcode names come from spirv_grammar's table.
// Used by tests (opcode assertions + census) and for eyeballing emitter output.

module spirv_dis shared public

require spirv_grammar
require spirv_builder
require strings

// Walk instructions (header skipped). The block gets the opcode and a [opStart, opStart+opCount)
// window into `words` for its operands — zero allocation per instruction.
def public foreach_inst(words : array<uint>; blk : block<(opcode : uint; words : array<uint>; opStart : int; opCount : int) : void>) {
    var i = 5                       // skip the 5-word header
    let n = length(words)
    while (i < n) {
        let w0 = words[i]
        let wc = int(w0 >> 16u)
        let op = w0 & 0xFFFFu
        // malformed/truncated: wc==0 would loop forever; i+wc>n would run operands past the
        // buffer end (out-of-bounds reads in op_has_operand/dump indexing w[s+k]).
        if (wc == 0 || i + wc > n) {
            break
        }
        invoke(blk, op, words, i + 1, wc - 1)
        i += wc
    }
}

def public count_op(words : array<uint>; op : SpvOp) : int {
    var c = 0
    foreach_inst(words) $(opcode : uint; _w : array<uint>; _s, _n : int) {
        if (opcode == uint(op)) {
            c++
        }
    }
    return c
}

def public has_op(words : array<uint>; op : SpvOp) : bool {
    return count_op(words, op) > 0
}

// The set of distinct opcodes present — the per-module input to the opcode census meta-test.
def public opcode_set(words : array<uint>) : table<uint> {
    var s : table<uint>
    foreach_inst(words) $(opcode : uint; _w : array<uint>; _st, _n : int) {
        if (!key_exists(s, opcode)) {
            s |> insert(opcode)
        }
    }
    return <- s
}

// Does any instruction with `op` carry `value` among its operands? Cheap shape assertion
// (e.g. OpExecutionMode … LocalSize present; OpDecorate … Binding 0).
def public op_has_operand(words : array<uint>; op : SpvOp; value : uint) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(op)) {
            for (k in range(n)) {
                if (w[s + k] == value) {
                    found = true
                }
            }
        }
    }
    return found
}

// Does any instruction with `op` carry `first` immediately followed by `second` among its
// operands? Tighter than op_has_operand for decoration / exec-mode shape checks (e.g. Decorate
// … BuiltIn GlobalInvocationId, ExecutionMode … LocalSize 64) — a bare value that merely
// collides with some target id elsewhere won't match.
def public op_has_operand_pair(words : array<uint>; op : SpvOp; first : uint; second : uint) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(op)) {
            for (k in range(n - 1)) {
                if (w[s + k] == first && w[s + k + 1] == second) {
                    found = true
                }
            }
        }
    }
    return found
}

// First operand of the first instruction with `op` (e.g. OpEntryPoint's execution model). Use this
// instead of op_has_operand when the operand's POSITION matters: an EntryPoint's model value can
// collide with a later operand (Vertex == 0 matches the name's NUL-terminator word, ids, etc.).
def public op_first_operand(words : array<uint>; op : SpvOp) : tuple<ok : bool; value : uint> {
    var ok = false
    var value = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(op) && n > 0 && !ok) {
            ok = true
            value = w[s]
        }
    }
    return (ok = ok, value = value)
}

// Does any instruction with `op` carry `value` at operand index `idx` (0-based, after word0)?
// Positional — use this when the operand's POSITION matters and a bare value would false-positive
// (e.g. OpVariable's storage class at index 2: op_has_operand would also match the result-type/id
// operands, and small storage-class values like Output collide with ordinary ids).
def public op_has_operand_at(words : array<uint>; op : SpvOp; idx : int; value : uint) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(op) && idx >= 0 && idx < n && w[s + idx] == value) {
            found = true
        }
    }
    return found
}

// How many instructions with `op` carry `value` at operand index `idx` (0-based, after word0)?
// The counting form of op_has_operand_at — e.g. count OpDecorate with Block (decoration at idx 1) to
// assert there is EXACTLY one Block-decorated struct.
def public count_op_operand_at(words : array<uint>; op : SpvOp; idx : int; value : uint) : int {
    var count = 0
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(op) && idx >= 0 && idx < n && w[s + idx] == value) {
            count++
        }
    }
    return count
}

def private memory_access_extra_words(mask : uint) : int {
    var n = 0
    if ((mask & uint(SpvMemoryAccess.Aligned)) != 0u) {
        n++
    }
    if ((mask & uint(SpvMemoryAccess.MakePointerAvailable)) != 0u) {
        n++
    }
    if ((mask & uint(SpvMemoryAccess.MakePointerVisible)) != 0u) {
        n++
    }
    return n
}

//! Downgrades a finished module for a device without the four-wide decode: the scalar callback
//! serves every load afterwards. True when anything was removed.
[arch(at="../ARCHITECTURE.md#cm2-decode-vector")]
def public strip_decode_vector(var words : array<uint>) : bool {
    var out : array<uint>
    out |> reserve(length(words))
    for (i in range(5)) {
        out |> push(words[i])
    }
    let ext_words <- string_words("SPV_NV_cooperative_matrix_decode_vector")
    var stripped = false
    var i = 5
    let n = length(words)
    while (i < n) {
        let w0 = words[i]
        let wc = int(w0 >> 16u)
        let op = w0 & 0xFFFFu
        if (wc == 0 || i + wc > n) {
            break
        }
        var drop = false
        var drop_operand_idx = -1
        if (op == uint(SpvOp.Capability) && wc == 2 && words[i + 1] == uint(SpvCapability.CooperativeMatrixDecodeVectorNV)) {
            drop = true
        } elif (op == uint(SpvOp.Extension) && wc == 1 + length(ext_words)) {
            var same = true
            for (k in range(length(ext_words))) {
                if (words[i + 1 + k] != ext_words[k]) {
                    same = false
                }
            }
            drop = same
        } elif (op == uint(SpvOp.CooperativeMatrixLoadTensorNV)) {
            let mi = 6 + memory_access_extra_words(words[i + 6])
            if (mi < wc) {
                let ta = words[i + 1 + mi]
                if ((ta & uint(SpvTensorAddressingOperands.DecodeVectorFunc)) != 0u) {
                    var pos = mi + 1
                    if ((ta & uint(SpvTensorAddressingOperands.TensorView)) != 0u) {
                        pos++
                    }
                    if ((ta & uint(SpvTensorAddressingOperands.DecodeFunc)) != 0u) {
                        pos++
                    }
                    drop_operand_idx = pos
                    let nwc = uint(wc - 1)
                    out |> push((nwc << 16u) | op)
                    for (k in range(wc - 1)) {
                        if (k == mi) {
                            out |> push(ta & ~uint(SpvTensorAddressingOperands.DecodeVectorFunc))
                        } elif (k != drop_operand_idx) {
                            out |> push(words[i + 1 + k])
                        }
                    }
                    stripped = true
                    i += wc
                    continue
                }
            }
        }
        if (drop) {
            stripped = true
        } else {
            for (k in range(wc)) {
                out |> push(words[i + k])
            }
        }
        i += wc
    }
    if (stripped) {
        delete words
        words <- out
    } else {
        delete out
    }
    return stripped
}

// Operand-role classifier for the symbolic dump: is operand `idx` (0-based, after the opcode name) of
// `opName` a LITERAL rather than an <id>? Lets a dump be id-canonicalized (ids -> first-appearance
// placeholders) for id-renaming-insensitive golden comparison, without conflating a literal whose value
// happens to equal an id number (e.g. OpTypeFloat's width 32 vs result-id 32). Covers exactly the opcode
// set dasSpirv emits; any other opcode treats all operands as ids (the census gate separately pins the
// emitted set, so unknown opcodes cannot reach here). OpEntryPoint is all-literal: its model word, name
// words, and ids (id_main + interface globals) are all allocated in the unchanged module-assembly half,
// so they are stable across the port and compare raw.
def public op_operand_is_literal(opName : string; idx : int) : bool {
    // OpExtension's operand words encode the extension-name string (LSB-first bytes, NUL-terminated),
    // so every operand is a literal -- group it with the other all-operand-literal opcodes.
    if (opName == "OpEntryPoint" || opName == "OpCapability" || opName == "OpMemoryModel" || opName == "OpExtension") return true
    if (opName == "OpExecutionMode" || opName == "OpDecorate" || opName == "OpMemberDecorate" || opName == "OpExtInstImport") return idx >= 1
    if (opName == "OpConstant" || opName == "OpSpecConstant") return idx == 2
    // GroupNonUniform reduces: resultType(0) result(1) scope-id(2) GroupOperation(3 = literal) value(4)
    if (opName == "OpGroupNonUniformIAdd" || opName == "OpGroupNonUniformFAdd" ||
            opName == "OpGroupNonUniformSMin" || opName == "OpGroupNonUniformUMin" || opName == "OpGroupNonUniformFMin" ||
            opName == "OpGroupNonUniformSMax" || opName == "OpGroupNonUniformUMax" || opName == "OpGroupNonUniformFMax") return idx == 3
    if (opName == "OpTypeInt") return idx == 1 || idx == 2
    if (opName == "OpTypeFloat") return idx == 1
    if (opName == "OpTypeVector" || opName == "OpTypeMatrix") return idx == 2
    if (opName == "OpTypePointer") return idx == 1
    if (opName == "OpTypeImage") return idx >= 2
    if (opName == "OpVariable" || opName == "OpFunction") return idx == 2
    if (opName == "OpSelectionMerge") return idx == 1
    if (opName == "OpLoopMerge") return idx == 2
    if (opName == "OpExtInst") return idx == 3
    if (opName == "OpCompositeExtract") return idx >= 3
    if (opName == "OpVectorShuffle") return idx >= 4
    // operands: resultType(0) resultId(1) image(2) coord(3) imageOperandsMask(4 = literal) lod-id(5)
    if (opName == "OpImageSampleExplicitLod" || opName == "OpImageFetch") return idx == 4
    return false
}

// Symbolic dump for eyeballing / diffing against external spirv-dis. Operands are printed as
// raw decimal words (a result-id-aware pretty printer is a later refinement).
def public dump(words : array<uint>) : string {
    if (length(words) < 5) {
        return "; (empty or truncated SPIR-V)\n"
    }
    let out = build_string() $(var wr) {
        wr |> write("; SPIR-V module: {length(words)} words, id-bound {words[3]}\n")
        foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
            let nm = SPV_OP_NAMES?[opcode] ?? "Op?({opcode})"
            wr |> write(nm)
            for (k in range(n)) {
                wr |> write(" {w[s + k]}")
            }
            wr |> write("\n")
        }
    }
    return out
}

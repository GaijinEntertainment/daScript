options gen2

// Minimal SPIR-V disassembler + opcode-census helpers. The binary is a self-delimiting word
// stream: word0 of each instruction is (wordCount << 16) | opcode, so the whole module walks
// with zero per-opcode knowledge. Symbolic opcode names come from spirv_grammar's table.
// Used by tests (opcode assertions + census) and for eyeballing emitter output.

module spirv_dis shared public

require spirv_grammar
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
        if (opcode == uint(op) && idx < n && w[s + idx] == value) {
            found = true
        }
    }
    return found
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

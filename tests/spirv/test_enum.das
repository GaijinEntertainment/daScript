options gen2
options indenting = 4

// An enum has no SPIR-V type of its own: it lowers to the OpTypeInt of the underlying integer it was
// declared over, and every trace of the enum is gone by the time the words are written. So what this
// pins is an ABSENCE as much as a presence -- the entry constant is an OpConstant of the plain
// integer type carrying the entry's own VALUE (not its ordinal), the compare is a plain OpIEqual, and
// `int(regime)` costs nothing at all, which is why the conversion opcodes are asserted to be absent
// rather than counted.
//
// The uint8-based enum is here for the width/signedness half of that claim: it must land on
// OpTypeInt 8 0, and its entry constant must be a constant OF THAT TYPE -- a 32-bit constant handed
// to an 8-bit operand is exactly the mismatch spirv-val exists to catch.

require dastest/testing_boost public
require _spirv_common               // validate_spirv
require spirv/spirv_shader          // [compute_shader] annotation
require spirv/spirv_builtins public // gl_GlobalInvocationID
require spirv/spirv_dis             // count_op, op_has_operand, op_has_operand_at
require spirv/spirv_grammar         // SpvOp, SpvCapability

// Non-contiguous values on purpose: an entry's value is what must reach the constant, and ordinals
// would agree with it if they were 0,1,2.
enum Regime {
    dry = 3
    humid = 7
    frozen = 11
}

enum Flag : uint8 {
    off = 0
    on = 5
}

struct Cell {
    regime : Regime
    depth : int
}

var @ssbo @binding = 0 enum_data : array<int>

def regime_weight(r : Regime) : int {
    if (r == Regime.humid) return 2
    return int(r)
}

// An enum-typed ternary is an OpSelect over the integer: the result type has to
// peel to that integer or the lane count comes out zero and the select is refused.
def regime_pick(hot : bool) : Regime {
    return hot ? Regime.frozen : Regime.dry
}

// A `var` local of enum type -- a Function-storage OpVariable whose pointee is the plain integer.
def cell_weight(c : Cell) : int {
    var r = c.regime
    if (r != Regime.dry) {
        r = Regime.humid
    }
    return regime_weight(r) + c.depth
}

def cell_regime(c : Cell) : Regime {
    return c.regime
}

[compute_shader(local_size_x=64, name="enum_spv"), marker(no_coverage)]
def enum_shader {
    let gi = gl_GlobalInvocationID.x
    enum_data[gi] = regime_weight(regime_pick(enum_data[gi] > 0))
}

// The engine's target shape: a struct member typed by the enum itself rather than by an int.
[compute_shader(local_size_x=64, name="enum_struct_spv"), marker(no_coverage)]
def enum_struct_shader {
    let gi = gl_GlobalInvocationID.x
    let c = Cell(regime = Regime.humid, depth = enum_data[gi])
    enum_data[gi] = cell_weight(c) + int(cell_regime(c))
}

def flag_on(f : Flag) : bool {
    return f == Flag.on
}

[compute_shader(local_size_x=64, name="enum_u8_spv"), marker(no_coverage)]
def enum_u8_shader {
    let gi = gl_GlobalInvocationID.x
    enum_data[gi] = flag_on(Flag.on) ? 1 : 0
}

// The id OpTypeInt <width> <signedness> declares (0 if absent). Operands are [result-id, width,
// signedness], so the id sits at 0 -- which op_has_operand_pair, whose answer is a bool, cannot give.
def private int_type_id(words : array<uint>; width, signedness : uint) : uint {
    var id = 0u
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.TypeInt) && n >= 3 && w[s + 1] == width && w[s + 2] == signedness) {
            id = w[s]
        }
    }
    return id
}

// Is there an OpConstant of `type_id` whose value is `value`? Operands are [result-type, result-id,
// value], so both the type and the literal have to match on the same instruction.
def private has_const_of_type(words : array<uint>; type_id, value : uint) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode == uint(SpvOp.Constant) && n >= 3 && w[s] == type_id && w[s + 2] == value) {
            found = true
        }
    }
    return found
}

// No enum reaches SPIR-V as anything but its integer, so no width/class conversion may appear: these
// fixtures index with the uint invocation id and convert nothing else, which is what makes a zero
// count meaningful instead of merely small.
def private check_no_converts(t : T?; words : array<uint>; what : string) {
    t |> equal(count_op(words, SpvOp.SConvert), 0, "{what}: no OpSConvert")
    t |> equal(count_op(words, SpvOp.UConvert), 0, "{what}: no OpUConvert")
    t |> equal(count_op(words, SpvOp.Bitcast), 0, "{what}: no OpBitcast")
}

def private check_validates(t : T?; words : array<uint>; what : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{what}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test]
def test_enum_value_rail(t : T?) {
    t |> run("enum: lowers to its underlying int") <| @(t : T?) {
        var words <- clone_to_move(enum_spv)
        t |> equal(words[0], SPV_MAGIC)
        let i32 = int_type_id(words, 32u, 1u)
        t |> success(i32 != 0u, "declares OpTypeInt 32 1")
        t |> success(has_const_of_type(words, i32, 11u), "Regime.frozen is the int constant 11")
        t |> success(has_const_of_type(words, i32, 7u), "Regime.humid is the int constant 7")
        t |> equal(count_op(words, SpvOp.IEqual), 1, "the enum compare is one OpIEqual")
        t |> equal(count_op(words, SpvOp.Select), 1, "the enum ternary is one OpSelect")
        check_no_converts(t, words, "enum")
        check_validates(t, words, "enum")
        delete words
    }
}

[test]
def test_enum_struct_member(t : T?) {
    t |> run("enum: struct member, local and result") <| @(t : T?) {
        var words <- clone_to_move(enum_struct_spv)
        let i32 = int_type_id(words, 32u, 1u)
        t |> success(i32 != 0u, "declares OpTypeInt 32 1")
        // Cell is { Regime, int } — both members are the SAME int type id, so the struct carries no
        // enum-shaped member and the host layout of an enum field is an int32's.
        var cell_members_int = false
        foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
            if (opcode == uint(SpvOp.TypeStruct) && n == 3 && w[s + 1] == i32 && w[s + 2] == i32) {
                cell_members_int = true
            }
        }
        t |> success(cell_members_int, "Cell lowers to OpTypeStruct \{ int, int \}")
        t |> success(count_op(words, SpvOp.IEqual) + count_op(words, SpvOp.INotEqual) >= 2,
            "both enum compares are integer compares")
        check_no_converts(t, words, "enum struct")
        check_validates(t, words, "enum struct")
        delete words
    }
}

[test]
def test_enum_uint8(t : T?) {
    t |> run("enum over uint8: lowers to OpTypeInt 8 0") <| @(t : T?) {
        var words <- clone_to_move(enum_u8_spv)
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.Int8)),
            "OpCapability Int8")
        let u8 = int_type_id(words, 8u, 0u)
        t |> success(u8 != 0u, "declares OpTypeInt 8 0")
        t |> success(has_const_of_type(words, u8, 5u), "Flag.on is an 8-bit constant 5")
        check_validates(t, words, "enum uint8")
        delete words
    }
}

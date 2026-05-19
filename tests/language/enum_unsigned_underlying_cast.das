options gen2
require dastest/testing_boost public

// Casts of uint8/uint16/uint64-backed enums must zero-extend, not sign-extend.
// Signed-underlying enums must keep their existing sign-extending bit-reinterpret
// semantics (e.g. uint(int8_value=-50) stays 0xffffffce).

// Natural opcode byte — uint8 is the canonical choice for compact packed enums.
enum Opcode : uint8 {
    NOP = 0u8
    PING = 50u8
    HEARTBEAT = 200u8       // > 127, triggers the bug
    SHUTDOWN = 254u8
}

// Like a port id or compact channel — uint16 representable.
enum Channel : uint16 {
    SYS = 100ul
    USER = 5000ul
    BROADCAST = 60000ul     // > 32767, triggers the bug
}

// Signed counterparts — behaviour preserved.
enum Signed8 : int8 {
    LOW = 50
    NEGATIVE = -50
}

enum Signed16 : int16 {
    LOW = 1000l
    NEGATIVE = -1000l
}

[test]
def test_unsigned_underlying_zero_extends(t : T?) {
    t |> run("uint8-backed enum cast preserves byte value") @(t : T?) {
        t |> equal(uint(Opcode.HEARTBEAT), 200u)
        t |> equal(int(Opcode.HEARTBEAT), 200)
        t |> equal(int64(Opcode.SHUTDOWN), 254l)
        t |> equal(uint64(Opcode.SHUTDOWN), 254ul)
    }
    t |> run("uint16-backed enum cast preserves half-word value") @(t : T?) {
        t |> equal(uint(Channel.BROADCAST), 60000u)
        t |> equal(int(Channel.BROADCAST), 60000)
        t |> equal(int64(Channel.BROADCAST), 60000l)
    }
}

[test]
def test_signed_underlying_unchanged(t : T?) {
    t |> run("int8-backed enum cast keeps sign-reinterpret") @(t : T?) {
        t |> equal(int(Signed8.NEGATIVE), -50)
        t |> equal(uint(Signed8.NEGATIVE), 0xffffffceu)
    }
    t |> run("int16-backed enum cast keeps sign-reinterpret") @(t : T?) {
        t |> equal(int(Signed16.NEGATIVE), -1000)
    }
}

// Runtime path — defeats const folding so we exercise the SimNode side.
[test]
def test_runtime_iteration(t : T?) {
    t |> run("runtime: for-loop sum of uint8 enums") @(t : T?) {
        var arr : array<Opcode>
        arr |> push(Opcode.PING)
        arr |> push(Opcode.HEARTBEAT)
        var sum = 0u
        for (op in arr) {
            sum += uint(op)
        }
        t |> equal(sum, 250u)               // 50 + 200
    }
}

// Struct field access — exercises SimNode_GetField lowering.
struct Pkt {
    op : Opcode
}

[test]
def test_struct_field_cast(t : T?) {
    t |> run("struct field of uint8 enum, cast to uint") @(t : T?) {
        let p = Pkt(op = Opcode.HEARTBEAT)
        t |> equal(uint(p.op), 200u)
    }
}

// String interpolation goes through WalkEnumeration8 / Enum() value-name lookup.
[test]
def test_string_interpolation_names(t : T?) {
    t |> run("uint8 enum interpolates value name") @(t : T?) {
        t |> equal("{Opcode.HEARTBEAT}", "HEARTBEAT")
    }
    t |> run("uint16 enum interpolates value name") @(t : T?) {
        t |> equal("{Channel.BROADCAST}", "BROADCAST")
    }
}

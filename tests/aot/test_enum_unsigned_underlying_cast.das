options gen2
require dastest/testing_boost public

// AOT mirror of tests/language/enum_unsigned_underlying_cast.das — exercises the same
// uint8/uint16-backed enum cast cases through the AOT codegen path. Post-fix the generated
// C++ should call enum8u_to_uint/enum16u_to_uint (zero-extending) rather than the signed
// helpers.

enum Opcode : uint8 {
    NOP = 0u8
    PING = 50u8
    HEARTBEAT = 200u8
    SHUTDOWN = 254u8
}

enum Channel : uint16 {
    SYS = 100ul
    USER = 5000ul
    BROADCAST = 60000ul
}

enum Signed8 : int8 {
    LOW = 50
    NEGATIVE = -50
}

enum Signed16 : int16 {
    LOW = 1000l
    NEGATIVE = -1000l
}

[test]
def test_aot_unsigned_underlying_zero_extends(t : T?) {
    t |> run("aot: uint8-backed enum cast preserves byte value") @(t : T?) {
        t |> equal(uint(Opcode.HEARTBEAT), 200u)
        t |> equal(int(Opcode.HEARTBEAT), 200)
        t |> equal(int64(Opcode.SHUTDOWN), 254l)
    }
    t |> run("aot: uint16-backed enum cast preserves half-word value") @(t : T?) {
        t |> equal(uint(Channel.BROADCAST), 60000u)
        t |> equal(int(Channel.BROADCAST), 60000)
    }
}

[test]
def test_aot_signed_underlying_unchanged(t : T?) {
    t |> run("aot: int8-backed enum cast keeps sign-reinterpret") @(t : T?) {
        t |> equal(int(Signed8.NEGATIVE), -50)
        t |> equal(uint(Signed8.NEGATIVE), 0xffffffceu)
    }
    t |> run("aot: int16-backed enum cast keeps sign-reinterpret") @(t : T?) {
        t |> equal(int(Signed16.NEGATIVE), -1000)
    }
}

[test]
def test_aot_runtime_iteration(t : T?) {
    t |> run("aot: runtime for-loop sum of uint8 enums") @(t : T?) {
        var arr : array<Opcode>
        arr |> push(Opcode.PING)
        arr |> push(Opcode.HEARTBEAT)
        var sum = 0u
        for (op in arr) {
            sum += uint(op)
        }
        t |> equal(sum, 250u)
    }
}

struct Pkt {
    op : Opcode
}

[test]
def test_aot_struct_field_cast(t : T?) {
    t |> run("aot: struct field of uint8 enum, cast to uint") @(t : T?) {
        let p = Pkt(op = Opcode.HEARTBEAT)
        t |> equal(uint(p.op), 200u)
    }
}

[test]
def test_aot_string_interpolation_names(t : T?) {
    t |> run("aot: uint8 enum interpolates value name") @(t : T?) {
        t |> equal("{Opcode.HEARTBEAT}", "HEARTBEAT")
    }
    t |> run("aot: uint16 enum interpolates value name") @(t : T?) {
        t |> equal("{Channel.BROADCAST}", "BROADCAST")
    }
}

require dastest/testing_boost

require llvm/llvm_jit
require daslib/fuzzer
require daslib/faker

[jit]
def int_to_float ( x : int )
    return float(x)

[jit]
def uint_to_float ( x : uint )
    return float(x)

[jit]
def float_to_float ( x : float )
    return float(x)

[jit]
def double_to_float ( x : double )
    return float(x)

[test]
def test_intrin_float ( t:T? )
    t |> run("float(int)") <| @ ( t : T? )
        t |> success ( is_jit_function <| @@int_to_float )
        var fake <- Faker()
        fuzz <|
            let x = fake |> random_int
            t |> strictEqual ( float(x), int_to_float(x) )
    t |> run("float(uint)") <| @ ( t : T? )
        t |> success ( is_jit_function <| @@uint_to_float )
        var fake <- Faker()
        fuzz <|
            let x = fake |> random_uint
            t |> strictEqual ( float(x), uint_to_float(x) )
    t |> run("float(float)") <| @ ( t : T? )
        t |> success ( is_jit_function <| @@float_to_float )
        var fake <- Faker()
        fuzz <|
            let x = fake |> random_float
            t |> strictEqual ( float(x), float_to_float(x) )
    t |> run("float(double)") <| @ ( t : T? )
        t |> success ( is_jit_function <| @@double_to_float )
        var fake <- Faker()
        fuzz <|
            let x = fake |> random_double
            t |> strictEqual ( float(x), double_to_float(x) )

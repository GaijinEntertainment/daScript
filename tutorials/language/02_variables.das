// Tutorial 2: Variables and Basic Types
//
// This tutorial covers:
//   - var (mutable) vs let (immutable) variables
//   - Basic types: int, float, bool, string, double
//   - Type inference and explicit type annotations
//   - No implicit type conversions (strict typing)
//   - Type casting with int(), float(), string()
//
// Run: daslang.exe tutorials/language/02_variables.das

options gen2

[export]
def main {

    // === var vs let ===

    // var declares a mutable variable — its value can change.
    var score = 0
    score = 100
    print("score = {score}\n")

    // let declares an immutable variable — it cannot be reassigned.
    let maxScore = 999
    // maxScore = 0    // ERROR: cannot modify a constant
    print("maxScore = {maxScore}\n")

    // === Type inference ===

    // The compiler infers the type from the value on the right.
    let i = 42          // int
    let f = 3.14        // float (not double!)
    let b = true        // bool
    let s = "hello"     // string
    print("i={i}  f={f}  b={b}  s={s}\n")

    // === Explicit type annotations ===

    // You can specify the type explicitly with : Type
    let x : int = 10
    let y : float = 2.5
    let z : double = 1.0lf         // 'lf' suffix for double literals
    let flag : bool = false
    let greeting : string = "hi"
    print("x={x}  y={y}  z={z}  flag={flag}  greeting={greeting}\n")

    // === No implicit type conversions ===

    // daslang is strict: you CANNOT mix int and float in arithmetic.
    // let bad = i + f           // ERROR: int + float
    // You must explicitly cast one side to match the other.
    let result = float(i) + f    // cast int to float
    print("float({i}) + {f} = {result}\n")

    let truncated = int(f)       // cast float to int (truncates)
    print("int({f}) = {truncated}\n")

    // === string() converts many types to string ===
    let numStr = string(42)
    let fltStr = string(3.14)
    print("string(42) = \"{numStr}\"  string(3.14) = \"{fltStr}\"\n")

    // NOTE: int("123") does NOT work — use to_int from 'require strings'

    // === bool is NOT interchangeable with int ===
    // let wrong = bool(1)      // ERROR: no bool(int) cast
    let right = 1 != 0          // use comparison instead
    print("1 != 0 is {right}\n")

    // === Hex literals are uint by default ===
    let h = 0xFF                // this is uint, not int
    let hi = int(0xFF)          // cast to int explicitly
    print("0xFF as uint = {h}, as int = {hi}\n")

    // === Zero values ===
    // When you declare a variable with type but no initializer,
    // it is zero-initialized.
    let zeroInt : int
    let zeroFloat : float
    let zeroBool : bool
    let zeroString : string
    print("zeros: int={zeroInt}  float={zeroFloat}  bool={zeroBool}  string=\"{zeroString}\"\n")

    // === 64-bit and unsigned types ===
    let bigInt : int64 = 9_000_000_000l   // 'l' suffix for int64
    let bigUint : uint64 = 18_000_000_000ul
    print("int64={bigInt}  uint64={bigUint}\n")
}

// output:
// score = 100
// maxScore = 999
// i=42  f=3.14  b=true  s=hello
// x=10  y=2.5  z=1  flag=false  greeting=hi
// float(42) + 3.14 = 45.14
// int(3.14) = 3
// string(42) = "42"  string(3.14) = "3.14"
// 1 != 0 is true
// 0xFF as uint = 0xff, as int = 255
// zeros: int=0  float=0  bool=false  string=""
// int64=9000000000  uint64=0x430e23400

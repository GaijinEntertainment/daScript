// Regression for issue #3065 (fuzzer, AOT): binding a local reference to a temporary
// value -- here a freshly-built array `[1]` -- has no addressable storage to point at,
// so the reference would dangle. The front-end used to let this through under `unsafe`
// (in the fuzzer it was the finally of an unsafe block that suppressed the check), after
// which it crashed the interpreter at runtime and emitted ill-formed C++ (`&temporary`)
// under AOT. It is now a hard compile error regardless of `unsafe`.
options gen2
expect 31019

[export]
def main {
    unsafe {
        let r & := [1]
    }
}

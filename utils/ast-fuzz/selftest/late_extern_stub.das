options gen2
require dasbind public

// dasbind marks this function stub=true and infer never enters the body (calls are
// retargeted to the DasBindFunction proxy), so the zero-arg int() below legitimately
// keeps a null func - the post-infer verify must skip it, like every pass it guards.
[extern(cdecl, late, name="NoSuchFunction", library="NoSuchLibrary")]
def late_extern_stub() : int {
    return int()
}

[export]
def main {
    print("ok\n")
}

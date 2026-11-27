options gen2
options log_nodes = true

struct FusedLink {
    pad : int
    x : int
}

[sideeffects]
def fused_add(a, b : int) : int => a + b

[sideeffects, unsafe_deref]
def fused_field_less(a, b : FusedLink?) : bool => a.x < b.x

[sideeffects, unsafe_deref]
def fused_field_through_reference(p : FusedLink?&) : int => p.x

[sideeffects]
def fused_if_less(a, b : int) : int {
    if (a < b) {
        return 1
    } else {
        return 2
    }
}

[sideeffects]
def fused_element_copy(var dst : array<int>; src : array<int>; i, j : int) {
    dst[i + 1] = src[j + 1]
}

[export]
def main {
    var first = FusedLink(x = 1)
    var second = FusedLink(x = 2)
    var p = unsafe(addr(first))
    var dst <- [1, 2, 3]
    let src <- [4, 5, 6]
    fused_element_copy(dst, src, 0, 1)
    let less = fused_field_less(p, unsafe(addr(second)))
    let field = fused_field_through_reference(p)
    print("fused {fused_add(1, 2)} {less} {field} {fused_if_less(1, 2)} {dst[1]}\n")
}

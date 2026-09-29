options gen2
options no_aot

require dastest/testing_boost public
require llvm/daslib/llvm_jit_plan

def private salt(layouts : uint64) : uint64 {
    return jit_env_salt(3, 0, false, false, "", true, true, JitSanitize(), layouts)
}

[test]
def test_layout_term(t : T?) {
    let base = handled_layout_term("ast", "ExprBlock", "list", 64ul)
    t |> run("a field that moved is another term") @(t : T?) {
        t |> success(handled_layout_term("ast", "ExprBlock", "list", 72ul) != base)
    }
    t |> run("the field, the type and the module each name the term") @(t : T?) {
        t |> success(handled_layout_term("ast", "ExprBlock", "finalList", 64ul) != base)
        t |> success(handled_layout_term("ast", "ExprLet", "list", 64ul) != base)
        t |> success(handled_layout_term("rtti", "ExprBlock", "list", 64ul) != base)
    }
    t |> run("a type's size is not the offset of a field of that value") @(t : T?) {
        t |> success(handled_layout_term("ast", "ExprBlock", "", 64ul) != base)
    }
}

[test]
def test_layout_hash(t : T?) {
    t |> run("the running binary's layouts hash the same on every walk") @(t : T?) {
        let first = handled_layout_hash()
        t |> success(first != 0ul, "the walk found handled types")
        t |> equal(handled_layout_hash(), first)
    }
}

[test]
def test_salt_folds_layouts(t : T?) {
    let here = handled_layout_hash()
    t |> run("the salt a compile takes is the salt over this binary's layouts") @(t : T?) {
        t |> equal(jit_env_salt(3, 0, false, false, "", true, true, JitSanitize()), salt(here))
    }
    t |> run("other layouts are another salt") @(t : T?) {
        t |> success(salt(here + 1ul) != salt(here))
    }
}

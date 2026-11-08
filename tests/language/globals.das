options gen2
options no_unused_function_arguments = false

require dastest/testing_boost public
require _fixup_init_macro
require _global_var_annotation_helper

var {
    go_some_a <- ["test"]
}

def go_test_a() : array<string> {
    assert(length(go_some_a) == 1)
    return <- clone(go_some_a)
}

let {
    go_some_0 <- go_test_a()
}

[export]
def go_test_0() : array<string> => <- clone(go_some_0)

[test(description = "a global initialized by a call that reads an earlier global sees that global already initialized")]
def test_global_order(t : T?) {
    t |> run("global init order with clone") @(t : T?) {
        t |> equal(length(go_some_a), 1)
        t |> equal(length(go_some_0), 1)
        let cloned <- go_test_0()
        t |> equal(length(cloned), 1)
    }
}

struct GpiFoo {
    i : int = 5
    a : array<int>
}

var g_gpi : GpiFoo? = new GpiFoo()

var g_gpi_pg = new GpiFoo()

[test(description = "a global pointer initialized with new, typed or inferred, points at a struct that carries its field defaults")]
def test_global_ptr_init(t : T?) {
    t |> run("global pointer struct access") @(t : T?) {
        push(g_gpi.a, 10)
        t |> equal(g_gpi.i, 5)
    }
    t |> run("global pointer default init") @(t : T?) {
        t |> equal(g_gpi_pg.i, 5)
    }
}

// a global init installed by an annotation's fixup() - after infer -
// gets a gated re-infer before foldUnsafe (compileDaScript,
// ast_parse.cpp); dasSpirv's shader-blob capture relies on it
[fixup_init_capture(name="fixup_captured")]
def annotated_stub {
    pass
}

[test(description = "a call-shaped global init that an annotation installs in fixup, after infer, still resolves and evaluates")]
def test_fixup_global_init(t : T?) {
    t |> run("call-shaped global init set in annotation fixup resolves + evaluates") <| @(t : T?) {
        t |> equal(fixup_captured |> length, 4)
        t |> equal(fixup_captured[0], 7u)
        t |> equal(fixup_captured[1], 11u)
        t |> equal(fixup_captured[2], 13u)
        t |> equal(fixup_captured[3], 0u)   // annotated_stub arity, read at fixup time
    }
}

var io_order : array<string>

[init(before="middle")]
def io_a {
    io_order |> push("a")
}

[init(tag="middle")]
def io_b {
    io_order |> push("b")
}

[init(tag="middle")]
def io_c {
    io_order |> push("c")
}

[init(after="middle")]
def io_d {
    io_order |> push("d")
}

// d -> b -> a and d -> c -> a: the legal orders are [d, c, b, a] and
// [d, b, c, a]
[test(description = "[init] functions run in the order their before/after tags demand, with the two tagged peers in either order")]
def test_init_order(t : T?) {
    t |> success(io_order[0] == "d")
    t |> success(io_order[1] == "b" || io_order[1] == "c")
    t |> success(io_order[2] == "b" || io_order[2] == "c")
    t |> success(io_order[3] == "a")
    t |> success(io_order[1] != io_order[2])
}

// the CheckColorAnnotation lint_macro fails the compile if any
// `assert_color_*` global lacks @color
var @color assert_color_single = 1

var {
    @color assert_color_block_a = 2
    @color assert_color_block_b = 3
}

[test(description = "a @name annotation on a global reaches the variable in both the single and the block declaration form")]
def test_global_var_annotation(t : T?) {
    t |> run("globals carry their field annotations") @(t : T?) {
        t |> equal(assert_color_single, 1)
        t |> equal(assert_color_block_a, 2)
        t |> equal(assert_color_block_b, 3)
    }
}

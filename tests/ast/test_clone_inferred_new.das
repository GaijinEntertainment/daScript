options gen2

require dastest/testing_boost public
require _clone_inferred_new_macros

class Foo {
    x : int = 13
}

def make_foo() : Foo? => new Foo()

def target_clone() : Foo? => null

[test(description = "a clone of an inferred new expression retains its constructor and initializes Foo.x", standalone = "the infer macro rewrites functions named make_foo and target_clone after inference and would alter matching functions in a shared program")]
def test_clone_of_inferred_new(t : T?) {
    var foo = target_clone()
    t |> success(foo != null, "the cloned 'new Foo()' runs")
    if (foo != null) {
        t |> equal(13, foo.x, "the initializer of Foo runs")
        unsafe {
            delete foo
        }
    }
}

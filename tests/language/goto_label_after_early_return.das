options gen2
options rtti
options no_coverage
options disable_auto_inline

require daslib/ast
require daslib/ast_boost
require dastest/testing_boost public
require daslib/strings_boost
require _optimized_body_helper

var skip = true
var done = false

def jump_past_an_early_return() : int {
    var r = 0
    if (skip) {
        goto label 0
    }
    if (done) {
        return -1
    }
    r = 100
    label 0:
    r++
    return r
}

def fold_after_the_label() : int {
    var r = 0
    if (skip) {
        goto label 0
    }
    r = 100
    label 0:
    if (done) {
        return -1
    }
    r++
    return r
}

[test]
def test_forward_goto_to_a_label_after_an_early_return(t : T?) {
    t |> equal(jump_past_an_early_return(), 1)
    t |> equal(fold_after_the_label(), 1)
    ast_gc_guard() {
        let before = body_of(t, "jump_past_an_early_return")
        t |> success(!(before |> contains("else")), "an if before the label stays unfolded, got:\n{before}")
        let after = body_of(t, "fold_after_the_label")
        t |> success(after |> contains("else"), "an if after the label still folds into if/else, got:\n{after}")
    }
}

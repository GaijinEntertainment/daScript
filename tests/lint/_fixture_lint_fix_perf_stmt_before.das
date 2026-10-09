//fmt:ignore-file
options gen2
options auto_inline_functions = false
options rtti

require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/ast_match

struct Sized {
    @exact_size count : int
    @exact_size rows : array<int>
}

def make() : array<int> {
    return <- [1, 2, 3]
}

def moved() : array<int> {
    var inscope result <- make()
    return <- result
}

def cloned(src : array<int>) : array<int> {
    var copy := src
    return <- copy
}

def moved_commented() : array<int> {
    var result <- make()
    // a comment between keeps the variable
    return <- result
}

def total(var values : array<int>) : int {
    var sum = 0
    for (i in range(length(values))) {
        values[i] = values[i] * 2
        sum += values[i]
    }
    return sum
}

def bump_all(var arr : array<int>; value : int) {
    for (i in range(length(arr))) {
        arr[i] = arr[i] + 1
    }
    let arr_elem = value
    for (k in range(length(arr))) {
        arr[k] += arr_elem
    }
}

def escapes(var arr : array<int>) {
    for (i in range(length(arr))) {
        arr[i] = length(arr)
    }
}

def copy_all(src : array<int>; var dst : array<int>) {
    for (s in src) {
        dst |> push(s)
    }
    for (s in src) {
        dst |> push(s) // one at a time
    }
}

def fresh() : int {
    var a : array<int>
    a <- make()
    var inscope b : array<int>
    b <- make()
    return length(a) + length(b)
}

def not_fresh() : int {
    var c : array<int>
    let n = 1
    c <- make()
    return length(c) + n
}

def width(@exact_size n : int) : int {
    return n
}

[macro_function]
def single_splice(var stmts : array<Expression?>; var src : Expression?) {
    var copy = clone_expression(src)
    stmts |> push <| qmacro_expr() {
        return $e(copy)
    }
}

[macro_function]
def splice_later(var stmts : array<Expression?>; var src : Expression?) {
    var copy = clone_expression(src)
    src = null
    stmts |> push <| qmacro_expr() {
        return $e(copy)
    }
}

[macro_function]
def into_annotated(var src : Expression?) {
    var res = peel_lambda_rename_var(clone_expression(src), "name")
    var ce = clone_expression(src)
    var res2 = peel_lambda_rename_var(ce, "name")
}

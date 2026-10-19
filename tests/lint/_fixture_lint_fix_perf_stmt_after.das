//fmt:ignore-file
options gen2
options auto_inline_functions = false
options rtti

require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/ast_match

struct Sized {
    count : int
    @exact_size rows : array<int>
}

def make() : array<int> {
    return <- [1, 2, 3]
}

def moved() : array<int> {
    return <- make()
}

def cloned(src : array<int>) : array<int> {
    return clone_to_move(src)
}

def moved_commented() : array<int> {
    var result <- make()
    // a comment between keeps the variable
    return <- result
}

def total(var values : array<int>) : int {
    var sum = 0
    for (value in values) {
        value = value * 2
        sum += value
    }
    return sum
}

def bump_all(var arr : array<int>; value : int) {
    for (arr_elem2 in arr) {
        arr_elem2 = arr_elem2 + 1
    }
    let arr_elem = value
    for (arr_elem2 in arr) {
        arr_elem2 += arr_elem
    }
}

def escapes(var arr : array<int>) {
    for (i in range(length(arr))) {
        arr[i] = length(arr)
    }
}

def copy_all(src : array<int>; var dst : array<int>) {
    dst |> push_from(src)
    for (s in src) {
        dst |> push(s) // one at a time
    }
}

def fresh() : int {
    var a : array<int> <- make()
    var inscope b : array<int> <- make()
    return length(a) + length(b)
}

def not_fresh() : int {
    var c : array<int>
    let n = 1
    c <- make()
    return length(c) + n
}

def width(n : int) : int {
    return n
}

[macro_function]
def single_splice(var stmts : array<Expression?>; var src : Expression?) {
    stmts |> push <| qmacro_expr() {
        return $e(src)
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
    var res = peel_lambda_rename_var(src, "name")
    var res2 = peel_lambda_rename_var(src, "name")
}

def grown(v : int) : int {
    return v + 1
}

def via_call(var arr : array<int>) {
    for (i in range(length(arr))) {
        arr[i] = grown(arr[i])
    }
}

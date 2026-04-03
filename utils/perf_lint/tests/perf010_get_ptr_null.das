options gen2
// PERF010: get_ptr(x) == null should be x == null
//
// Problem: get_ptr() is unnecessary for null comparison on smart_ptr.
//
// Bad pattern:
//   if (get_ptr(ptr) == null) { ... }
//
// Good pattern:
//   if (ptr == null) { ... }

require daslib/ast_boost

// bad: get_ptr for null check (1 warning)
def bad_get_ptr_null(expr : ExpressionPtr) {
    if (get_ptr(expr) == null) {
        print("null\n")
    }
}

// good: direct null check
def good_null_check(expr : ExpressionPtr) {
    if (expr == null) {
        print("null\n")
    }
}

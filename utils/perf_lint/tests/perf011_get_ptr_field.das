options gen2
// PERF011: get_ptr(x).field should be x.field
//
// Problem: get_ptr() is unnecessary for field access on smart_ptr.
//
// Bad pattern:
//   let name = get_ptr(expr).__rtti
//
// Good pattern:
//   let name = expr.__rtti

require daslib/ast_boost

// bad: get_ptr for field access (1 warning)
def bad_get_ptr_field(expr : ExpressionPtr) {
    let rtti = get_ptr(expr).__rtti
}

// good: direct field access
def good_field_access(expr : ExpressionPtr) {
    let rtti = expr.__rtti
}

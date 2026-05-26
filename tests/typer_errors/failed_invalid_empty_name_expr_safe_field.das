// Regression test for invalid_empty_name (30296) on ExprSafeField.
// Cascading 30224 fires because the ExprSafeField target isn't a
// variant/pointer-to-tuple/structure/handle.
options gen2
expect 30296, 30224

require _helper_empty_names

[export]
def main {
    let _ = make_empty_safe_field()
}

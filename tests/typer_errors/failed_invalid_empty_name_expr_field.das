// Regression test for invalid_empty_name (30296) on ExprField.
// Cascading 30928 "can't get field '' of int const" fires downstream of
// the empty-name preVisit error.
options gen2
expect 30296, 30928

require _helper_empty_names

[export]
def main {
    let _ = make_empty_field()
}

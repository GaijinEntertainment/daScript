options gen2

require _fixture_typedef_used_scope
require _fixture_typedef_used_facade

[export]
def use_it() : int {
    var r = scope_marker()
    with (module _fixture_typedef_used_scope) {
        let made = TargetAl()
        r += made.x
    }
    return r
}
